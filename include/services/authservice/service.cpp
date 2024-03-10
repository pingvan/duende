#include "service.hpp"
#include "hash.hpp"
#include "token.hpp"
#include "components/database/database-connector.hpp"

int auth_service::save_user(authservice::User &user, std::string password, authservice::Tokens *tokens) {
    database::connector connector;
    user.set_salt(generate_salt(10));
    std::string hashed_password = generate_hash(password + user.salt());
    return connector.add_user(user.email(), user.username(), tokens->refresh().token(), hashed_password, user.salt());
}

grpc::Status auth_service::Login(
    grpc::ServerContext *context,
    const authservice::LoginRequest *request,
    authservice::LoginResponse *response
) {
    authservice::User user;
    authservice::Payload payload;
    token_service token_service;
    database::connector connector;
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

    if (id == -1) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }
    user.set_id(id);
    payload.set_user_id(id);

    std::string hashed_password = connector.get_hash_salt(id).first;
    std::string salt = connector.get_hash_salt(id).second;
    user.set_hashed_password(
        generate_hash(password + salt)
    );
    if (hashed_password != user.hashed_password()) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Incorrect password"
        );
    }

    authservice::Tokens *tokens = token_service.generate_tokens(payload);
    token_service.save_refresh_token(user.id(), tokens->refresh());

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
    authservice::User user;
    user.set_username(username);
    user.set_email(email);
    user.set_id(123);
    token_service token_service;
    database::connector connector;
    authservice::Payload payload;
    payload.set_username(username);
    payload.set_email(email);
    payload.set_user_id(user.id());

    if (!username_is_valid(username).ok()) {
        return username_is_valid(username);
    }
    if (!email_is_valid(email).ok()) {
        return email_is_valid(email);
    }
    if (!password_is_valid(password).ok()) {
        return password_is_valid(password);
    }
    if (connector.is_nickname_used(username) || connector.is_email_used(email)) {
        return grpc::Status(
            grpc::StatusCode::ALREADY_EXISTS, "User already exists"
        );
    }
    if (password != password_confirmation) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Passwords do not match"
        );
    }

    authservice::Tokens *tokens = token_service.generate_tokens(payload);
    token_service.save_refresh_token(user.id(), tokens->refresh());
    response->set_allocated_tokens(tokens);
    save_user(user, password, tokens);
    return grpc::Status::OK;
}

grpc::Status auth_service::Logout(
    grpc::ServerContext *context,
    const authservice::LogoutRequest *request,
    authservice::LogoutResponse *response
) {
    database::connector connector;
    std::string token = "";
    connector.update_refresh_token(request->id(), token);
    return grpc::Status::OK;
}
