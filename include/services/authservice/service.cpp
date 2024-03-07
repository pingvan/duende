#include "service.hpp"
#include "data.hpp"
#include "hash.hpp"
#include "token.hpp"
#include "components/database/database-connector.hpp"

int auth_service::save_user(authservice::User &user, std::string password, authservice::Tokens *tokens) {
    database::connector connector;
    user.set_salt(generate_salt(10));
    std::string hashed_password = generate_hash(password + user.salt());
    return connector.add_user(user.email(), user.username(), tokens->access().token(), tokens->refresh().token(), hashed_password, user.salt());
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
    if (request->has_username()) {
        user.set_username(request->username());
        payload.set_username(request->username())
        id = connector.get_user_id();
        // find other info
    }
    if (request->has_email()) {
        user.set_email(request->email());
        payload.set_email(request->email());
        id = connector.get_user_id();
    }
    user.set_id(id);
    payload.set_id(id);
    std::string password = request->password();

    if (id == -1) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }
    std::string hashed_password = get_hash_salt(id).first;
    std::string salt = get_hash_salt(id).second;
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
    user.set_username(username);
    user.set_email(email);
    user.set_id(123);
    authservice::User user;
    token_service token_service;
    database::connector connector;

    if (!username_is_valid(username).ok()) {
        return username_is_valid(username);
    }
    if (!email_is_valid(email).ok()) {
        return email_is_valid(email);
    }
    if (!password_is_valid(password).ok()) {
        return password_is_valid(password);
    }

    if (is_nickname_used(username) || is_email_used(email)) {
        return grpc::Status(
            grpc::StatusCode::ALREADY_EXISTS, "User already exists"
        );
    }
    if (password != password_confirmation) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Passwords do not match"
        );
    }

    authservice::UserDTO *user_dto = new authservice::UserDTO();
    authservice::Tokens *tokens = new authservice::Tokens();
    user_dto = token_service.get_user_dto(user);
    tokens = token_service.generate_tokens(user_dto);
    token_service.save_refresh_token(user.id(), tokens->refresh());

    response->set_allocated_user_dto(user_dto);
    response->set_allocated_tokens(tokens);

    save_user(user, password, tokens);

    return grpc::Status::OK;
}

grpc::Status auth_service::Logout(
    grpc::ServerContext *context,
    const authservice::LogoutRequest *request,
    authservice::LogoutResponse *response
) {
    const authservice::RefreshToken token = request->token();
    token_service token_service;
    return token_service.delete_refresh_token(token);
}

bool exist (something) {
    for (database) {
        return true;
    }
    return false;
}

something find (something) {
    for (database) {
        return something;
    }
    return shit;
}

if (exist(something)) {
    find(something)
}