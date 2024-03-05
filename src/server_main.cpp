#include <grpcpp/grpcpp.h>
#include "services/authservice/service.hpp"

std::string server_address("0.0.0.0:50051");

int main() {
    auth_service service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << '\n';
    server->Wait();
}