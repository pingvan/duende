#include "service.hpp"
#include <iostream>
#include "components/database/services/authservice/authservice.hpp"
#include "hash.hpp"
#include "token.hpp"

void auth_service::save_user(authservice::User &user, std::string password) {
    user.set_salt(generate_salt(10));
    user.set_hashed_password(generate_hash(password + user.salt()));
    user.set_id(database::authservice_database_api::add_user(
        user.email(), user.username(), "", user.hashed_password(), user.salt()
    ));
}

grpc::Status auth_service::Login(
    grpc::ServerContext *context,
    const authservice::LoginRequest *request,
    authservice::LoginResponse *response
) {
    authservice::User user;
    authservice::Payload payload;
    token_service token_service;
    database::authservice_database_api connector;
    int id;
    std::string password = request->password();

    if (request->has_username()) {
        std::string username = request->username();
        user.set_username(username);
        payload.set_username(username);

        id = connector.get_user_id(username);
        std::string email = connector.get_user_email(id);
        user.set_email(email);
        payload.set_email(email);
    }
    if (request->has_email()) {
        std::string email = request->email();
        user.set_email(email);
        payload.set_email(email);

        id = connector.get_user_id(email);
        std::string username = connector.get_user_nickname(id);
        user.set_username(username);
        payload.set_username(username);
    }

    response->set_id(id);

    if (id == -1) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }
    user.set_id(id);
    payload.set_user_id(id);

    std::string hashed_password = connector.get_hash_salt(id).first;
    std::string salt = connector.get_hash_salt(id).second;
    user.set_hashed_password(generate_hash(password + salt));
    if (hashed_password != user.hashed_password()) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Incorrect password"
        );
    }

    return grpc::Status::OK;
}

grpc::Status auth_service::Signup(
    grpc::ServerContext *context,
    const authservice::SignupRequest *request,
    authservice::SignupResponse *response
) {
    std::string username = request->username();
    std::string email = request->email();
    std::string password = request->password();
    std::string password_confirmation = request->password_confirmation();
    database::authservice_database_api connector;

    if (!username_is_valid(username).ok()) {
        return username_is_valid(username);
    }
    if (!email_is_valid(email).ok()) {
        return email_is_valid(email);
    }
    if (!password_is_valid(password).ok()) {
        return password_is_valid(password);
    }
    if (connector.is_nickname_used(username)) {
        return grpc::Status(
            grpc::StatusCode::ALREADY_EXISTS, "Username already registered"
        );
    }
    if (connector.is_email_used(email)) {
        return grpc::Status(
            grpc::StatusCode::ALREADY_EXISTS, "Email already registered"
        );
    }
    if (password != password_confirmation) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Passwords do not match"
        );
    }

    authservice::User user;
    user.set_username(username);
    user.set_email(email);

    save_user(user, password);

    token_service token_service;
    authservice::Payload payload;
    payload.set_username(username);
    payload.set_email(email);
    payload.set_user_id(user.id());

    authservice::Tokens *tokens = token_service.generate_tokens(payload);
    token_service.save_refresh_token(user.id(), tokens->refresh());
    authservice::AccessToken *access_token = new authservice::AccessToken();
    *access_token = tokens->access();
    response->set_allocated_access_token(access_token);
    response->set_id(user.id());

    return grpc::Status::OK;
}

grpc::Status auth_service::Logout(
    grpc::ServerContext *context,
    const authservice::LogoutRequest *request,
    authservice::LogoutResponse *response
) {
    token_service token_service;
    authservice::RefreshToken token;
    token.set_token("");
    token_service.save_refresh_token(request->id(), token);
    return grpc::Status::OK;
}

authservice::RefreshToken *auth_service_data::get_refresh_token(int user_id) {
    database::authservice_database_api connector;
    std::string token = connector.get_refresh_token(user_id);
    authservice::RefreshToken *refresh_token = new authservice::RefreshToken();
    refresh_token->set_token(token);
    return refresh_token;
}

grpc::Status auth_service_data::GetUserPrewiew(
    grpc::ServerContext *context,
    const authservice::RequestWithUserId *request,
    common::UserPreview *response
) {
    database::authservice_database_api::get_user_preview(request->user_id())
        .convert_to_proto(response);
    return grpc::Status::OK;
}

grpc::Status auth_service_data::GetUserFullPage(
    grpc::ServerContext *context,
    const authservice::RequestWithUserId *request,
    common::UserFullPage *response
) {
    database::authservice_database_api::get_user_full_page(request->user_id())
        .convert_to_proto(response);
    return grpc::Status::OK;
}

grpc::Status auth_service_data::UpdateAccessToken(
    grpc::ServerContext *context,
    const authservice::RequestWithUserId *request,
    authservice::AccessToken *response
) {
    token_service token_service;
    authservice::RefreshToken *refresh_token =
        get_refresh_token(request->user_id());

    if (token_service.is_refresh_token_active(refresh_token->token())
            .error_code() == grpc::StatusCode::UNAUTHENTICATED) {
        return grpc::Status(
            grpc::StatusCode::UNAUTHENTICATED, "Refresh token is invalid"
        );
    }

    authservice::AccessToken *access_token =
        token_service.refresh_access_token(*refresh_token);

    response->set_token(access_token->token());

    return grpc::Status::OK;
}

grpc::Status auth_service_data::CreateTokens(
    grpc::ServerContext *context,
    const authservice::RequestWithUserId *request,
    authservice::AccessToken *response
) {
    token_service token_service;
    authservice::Payload payload;
    payload.set_user_id(request->user_id());
    payload.set_email(
        database::authservice_database_api::get_user_email(request->user_id())
    );
    payload.set_username(database::authservice_database_api::get_user_nickname(
        request->user_id()
    ));
    authservice::Tokens *tokens = token_service.generate_tokens(payload);

    token_service.save_refresh_token(request->user_id(), tokens->refresh());

    response->set_token(tokens->access().token());

    return grpc::Status::OK;
}