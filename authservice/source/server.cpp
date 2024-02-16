#include "server.hpp"
#include <iostream>

grpc::Status auth_service::Login(grpc::ServerContext *context, const authservice::LoginRequest *request, authservice::LoginResponse *response) {
    std::cout << "Login" << std::endl;
    return grpc::Status::OK;
}

grpc::Status auth_service::Signin(grpc::ServerContext *context, const authservice::SigninRequest *request, authservice::SigninResponse *response) {
    std::cout << "Registration" << std::endl;
    return grpc::Status::OK;
}