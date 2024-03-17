#include "client.hpp"

client::client(const std::shared_ptr<grpc::Channel> &new_channel)
    : channel(new_channel),
      authorization_stub(authservice::AuthService::NewStub(new_channel)) {
}

grpc::Status
client::login(const std::string &client_login, const std::string &password) {
    grpc::ClientContext context;
    authservice::LoginRequest request;
    auto *response = new authservice::LoginResponse;
    request.set_username(client_login);
    request.set_password(password);
    
    return authorization_stub->Login(&context, request, response);
}

grpc::Status client::registration(
    const std::string &client_login,
    const std::string &email,
    const std::string &password,
    const std::string &password_confirmation
) {
    grpc::ClientContext context;
    authservice::SignupRequest request;
    auto *response = new authservice::SignupResponse;
    request.set_username(client_login);
    request.set_email(email);
    request.set_password(password);
    request.set_password_confirmation(password_confirmation);

    return authorization_stub->Signup(&context, request, response);
}