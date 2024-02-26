#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include "authservice.grpc.pb.h"
#include "authservice.pb.h"

struct client {
private:
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<authservice::AuthService::Stub> authorization_stub;

public:
    explicit client(const std::shared_ptr<grpc::Channel> &new_channel);

    grpc::Status
    login(const std::string &client_login, const std::string &password);
    grpc::Status registration(
        const std::string &client_login,
        const std::string &email,
        const std::string &password,
        const std::string &password_confirmation
    );

    template <typename Response>
    grpc::Status error_processing(Response *response);
};

#endif