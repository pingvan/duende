#include <grpcpp/grpcpp.h>
#include <components/database/database-connector.hpp>
#include "services/authservice/service.hpp"
#include "services/profile_service/profile_server.hpp"
#include "services/invitation_service/invitation_server.hpp"
#include "services/chat_service/chat_server.hpp"

std::string server_address("0.0.0.0:50051");

void run_server() {
    auth_service auth_service;
    ProfileServiceImpl profile_service;
    // InvitationServiceImpl invitation_service;
    // ChatServiceImpl chat_service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&auth_service);
    builder.RegisterService(&profile_service);
    // builder.RegisterService(&invitation_service);
    // builder.RegisterService(&chat_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << '\n';
    server->Wait();
}

int main() {
    run_server();
}