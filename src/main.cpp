#include <grpcpp/grpcpp.h>
#include "services/authservice/service.hpp"
#include "services/profile_service/profile_server.hpp"
#include "services/invitation_service/invitation_server.hpp"
#include "services/chat_service/chat_service.hpp"
#include "services/search_service/search_service.hpp"

std::string server_address("0.0.0.0:50051");

void run_server() {
    auth_service auth_service;
    auth_service_data auth_service_data;
    ProfileServiceImpl profile_service;
    chat_service_ chat_service;
    chat_data_service chat_data_service;
    SearchService search_service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&auth_service);
    builder.RegisterService(&profile_service);
    builder.RegisterService(&chat_service);
    builder.RegisterService(&auth_service_data);
    builder.RegisterService(&chat_data_service);
    builder.RegisterService(&search_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << '\n';
    server->Wait();
}

int main() {
    run_server();
}