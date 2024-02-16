#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <memory>

#include <grpcpp/grpcpp.h>
#include "authservice.grpc.pb.h"
#include "authservice.pb.h"

struct client {
private:
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<authservice::AuthService::Stub> authorization_stub;
public:
    explicit client(const std::shared_ptr<grpc::Channel>& new_channel);

    void login(const std::string &client_login, const std::string &password);
    void registration(const std::string &client_login, const std::string &password);
};

#endif