#ifndef SERVER_HPP_
#define SERVER_HPP_

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
};

#endif