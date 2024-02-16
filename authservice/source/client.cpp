#include "client.hpp"

client::client(const std::shared_ptr<grpc::Channel>& new_channel)
: channel(new_channel),
      authorization_stub(authservice::AuthService::NewStub(new_channel))
{}

void client::login(const std::string &client_login, const std::string &password) {
    grpc::ClientContext context;
    authservice::LoginRequest request;
    auto *response = new authservice::LoginResponse;
    request.set_username(client_login);
    request.set_password(password);
    grpc::Status status = authorization_stub->Login(&context, request, &response);
    if (status.ok()) {
        std::cout << "Login successful" << std::endl;
    } else {
        std::cout << "Login failed" << std::endl;
    }
}

void client::registration(const std::string &client_login, const std::string &password) {
    grpc::ClientContext context;
    authservice::SigninRequest request;
    auto *response = new authservice::SigninResponse;
    request.set_username(client_login);
    request.set_password(password);
    grpc::Status status = authorization_stub->Signin(&context, request, &response);
    if (status.ok()) {
        std::cout << "Registration successful" << std::endl;
    } else {
        std::cout << "Registration failed" << std::endl;
    }
}