#pragma once

#include <grpcpp/grpcpp.h>
#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"

struct auth_service final : public authservice::AuthService::Service {
    grpc::Status Login(
        grpc::ServerContext *context,
        const authservice::LoginRequest *request,
        authservice::LoginResponse *response
    ) override;
    grpc::Status Signin(
        grpc::ServerContext *context,
        const authservice::SigninRequest *request,
        authservice::SigninResponse *response
    ) override;
    grpc::Status Logout(
        grpc::ServerContext *context,
        const authservice::LogoutRequest *request,
        authservice::LogoutResponse *response
    ) override;
    grpc::Status save_user(authservice::User &user, std::string password);
    grpc::Status password_is_valid(const std::string &password);
    grpc::Status username_is_valid(const std::string &username);
    grpc::Status email_is_valid(const std::string &email);
};
