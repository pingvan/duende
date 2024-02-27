#include "server.hpp"
#include <iostream>
#include "data.hpp"

grpc::Status auth_service::Login(
    grpc::ServerContext *context,
    const authservice::LoginRequest *request,
    authservice::LoginResponse *response
) {
    std::string username = request->username();
    std::string password = request->password();

    if (users.find(username) == users.end()) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }
    if (users[username].second != password) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Incorrect password"
        );
    }

    std::cout << request->username() << " logged in" << std::endl;

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

    std::cout << request->username() << " signed in" << std::endl;

    return grpc::Status::OK;
}