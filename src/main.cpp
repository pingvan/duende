#include <grpcpp/grpcpp.h>
#include <components/database/database-connector.hpp>
#include "services/authservice/service.hpp"
#include "services/profile_service/profile_server.cpp"

std::string server_address("0.0.0.0:50051");

void run_server() {
    auth_service auth_service;
    ProfileServiceImpl profile_service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&auth_service);
    builder.RegisterService(&profile_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << '\n';
    server->Wait();
}

int main() {
    // run_server();
    int id  = 0;
    try {
        id = database::connector::add_user("ba", "bb", "cc", "ss", "dd", "ee");
    } catch (std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    std::cout << id << '\n';
}