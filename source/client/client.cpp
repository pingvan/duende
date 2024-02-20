#include "client.hpp"

client::client(const std::shared_ptr<grpc::Channel> &new_channel)
    : channel(new_channel),
      authorization_stub(authservice::AuthService::NewStub(new_channel)) {
}

template <typename Response>
grpc::Status client::error_processing(Response *response) {
    if (response->has_error()) {
        std::cout << "Error: " << response->error().error_message()
                  << std::endl;
        return grpc::Status::CANCELLED;
    }
    return grpc::Status::OK;
}

grpc::Status
client::login(const std::string &client_login, const std::string &password) {
    grpc::ClientContext context;
    authservice::LoginRequest request;
    auto *response = new authservice::LoginResponse;
    request.set_username(client_login);
    request.set_password(password);

    authorization_stub->Login(&context, request, response);
    return error_processing(response);
}

grpc::Status client::registration(
    const std::string &client_login,
    const std::string &email,
    const std::string &password,
    const std::string &password_confirmation
) {
    grpc::ClientContext context;
    authservice::SigninRequest request;
    auto *response = new authservice::SigninResponse;
    request.set_username(client_login);
    request.set_email(email);
    request.set_password(password);
    request.set_password_confirmation(password_confirmation);

    authorization_stub->Signin(&context, request, response);
    return error_processing(response);
}