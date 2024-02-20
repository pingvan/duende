#include <iostream>

#include "server.hpp"
#include "data.hpp"
#include "errors.hpp"

using namespace authservice;

template <typename Response>
void set_error(
    ServerError_ErrorType error_type,
    ServerError *error,
    Response *response
) {
    error->set_type(error_type);
    error->set_error_message(error_messages[error_type]);
    response->set_allocated_error(error);
}

grpc::Status auth_service::Login(
    grpc::ServerContext *context,
    const LoginRequest *request,
    LoginResponse *response
) {
    std::string username = request->username();
    std::string password = request->password();
    auto *error = new ServerError;
    ServerError_ErrorType error_type =
        ServerError_ErrorType::ServerError_ErrorType_NO_ERROR;

    if (users.find(username) == users.end()) {
        error_type =
            ServerError_ErrorType::ServerError_ErrorType_USER_DOES_NOT_EXIST;
    } else if (users[username].second != password) {
        error_type =
            ServerError_ErrorType::ServerError_ErrorType_INCORRECT_PASSWORD;
    }

    if (error_type != ServerError_ErrorType::ServerError_ErrorType_NO_ERROR) {
        set_error(error_type, error, response);
    } else {
        std::cout << request->username() << " logged in" << std::endl;
    }

    return grpc::Status::OK;
}

grpc::Status auth_service::Signin(
    grpc::ServerContext *context,
    const SigninRequest *request,
    SigninResponse *response
) {
    std::string username = request->username();
    std::string email = request->email();
    std::string password = request->password();
    std::string password_confirmation = request->password_confirmation();
    auto *error = new ServerError;
    ServerError_ErrorType error_type =
        ServerError_ErrorType::ServerError_ErrorType_NO_ERROR;

    if (users.find(username) != users.end()) {
        ServerError_ErrorType error_type =
            ServerError_ErrorType::ServerError_ErrorType_USER_ALREADY_EXISTS;
    }
    if (password != password_confirmation) {
        error_type =
            ServerError_ErrorType::ServerError_ErrorType_PASSWORD_NOT_CONFIRMED;
    }

    if (error_type != ServerError_ErrorType::ServerError_ErrorType_NO_ERROR) {
        set_error(error_type, error, response);
    } else {
        users[username] = std::make_pair(email, password);
        std::cout << request->username() << " signed in" << std::endl;
    }

    return grpc::Status::OK;
}