#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <grpcpp/grpcpp.h>
#include "authservice.pb.h"
#include "authservice.grpc.pb.h"

struct auth_service final : public authservice::AuthService::Service {
    grpc::Status Login(grpc::ServerContext *context, const authservice::LoginRequest *request, authservice::LoginResponse *response) override;
    grpc::Status Signin(grpc::ServerContext *context, const authservice::SigninRequest *request, authservice::SigninResponse *response) override;
};

#endif