#include "server.hpp"
#include <iostream>
#include "data.hpp"
#include "token.hpp"
#include "hash.hpp"


grpc::Status auth_service::save_user(authservice::User &user, std::string password) {
    std::unordered_map<std::string, std::string> user_data;
    std::string salt = generate_salt(5);
    user.set_hashed_password(generate_hash(password + salt));
    user.set_salt(salt);

    user_data["email"] = user.email();
    user_data["id"] = std::to_string(user.id());
    user_data["hashed_password"] = user.hashed_password();
    user_data["salt"] = user.salt();

    users[user.username()] = user_data;

    return grpc::Status::OK;
}

grpc::Status auth_service::Login(
    grpc::ServerContext *context,
    const authservice::LoginRequest *request,
    authservice::LoginResponse *response
) {
    authservice::User user;
    if (request->has_username()) {
        user.set_username(request->username());
        for (auto const &[key, val] : users) {
            if (key == user.username()) {
                user.set_email(val.at("email"));
                user.set_id(std::stoi(val.at("id")));
                user.set_salt(val.at("salt"));
                break;
            }
        }
    }
    if (request->has_email()) {
        user.set_email(request->email());
        for (auto const &[key, val] : users) {
            if (val.at("email") == user.email()) {
                user.set_username(key);
                user.set_id(std::stoi(val.at("id")));
                user.set_salt(val.at("salt"));
                break;
            }
        }
    }
    std::string password = request->password();

    if (users.find(user.username()) == users.end()) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }
    user.set_hashed_password(generate_hash(password + users[user.username()]["salt"]));
    if (users[user.username()]["hashed_password"] != user.hashed_password()) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Incorrect password"
        );
    }

    authservice::UserDTO *user_dto = new authservice::UserDTO();
    authservice::Tokens *tokens = new authservice::Tokens();
    user_dto = token_service::get_user_dto(user);
    tokens = token_service::generate_tokens(user_dto);
    token_service::save_token(user.id(), tokens->refresh());

    std::cout << user.username() << " logged in" << std::endl;

    return grpc::Status::OK;
}

grpc::Status auth_service::Signin(
    grpc::ServerContext *context,
    const authservice::SigninRequest *request,
    authservice::SigninResponse *response
) {
    std::string username = request->username();
    std::string email = request->email();
    std::string password = request->password();
    std::string password_confirmation = request->password_confirmation();
    authservice::User user;
    user.set_username(username);
    user.set_email(email);
    user.set_id(123);


    if (!username_is_valid(username).ok()) {
        return username_is_valid(username);
    }
    if (!email_is_valid(email).ok()) {
        return email_is_valid(email);
    }
    if (!password_is_valid(password).ok()) {
        return password_is_valid(password);
    }

    if (users.find(username) != users.end()) {
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
    user_dto = token_service::get_user_dto(user);
    tokens = token_service::generate_tokens(user_dto);
    token_service::save_token(user.id(), tokens->refresh());

    response->set_allocated_user_dto(user_dto);
    response->set_allocated_tokens(tokens);

    save_user(user, password);

    std::cout << user.username() << " signed in" << std::endl;

    return grpc::Status::OK;
}

grpc::Status auth_service::Logout(
    grpc::ServerContext *context,
    const authservice::LogoutRequest *request,
    authservice::LogoutResponse *response
) {
    const authservice::RefreshToken token = request->token();
    return token_service::delete_refresh_token(token);
}