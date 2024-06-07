#include <grpcpp/grpcpp.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "services/authservice/service.hpp"
#include "services/profile_service/profile_server.hpp"
#include "services/invitation_service/invitation_server.hpp"
#include "services/chat_service/chat_service.hpp"
#include "services/search_service/search_service.hpp"
#include "services/users_interaction_service/users_interaction_service.hpp"
#include "services/recommendation_service/recommendation_service.hpp"

static const std::string server_address("0.0.0.0:50051");

std::string ReadFile(const std::filesystem::path &path) {
    auto cert_path = std::filesystem::current_path().parent_path() / path;
    std::ifstream file(cert_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << cert_path << std::endl;
        std::exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::shared_ptr<grpc::ServerCredentials> GetServerCredentials() {
    grpc::SslServerCredentialsOptions ssl_opts;
    ssl_opts.pem_root_certs = ReadFile("certs/ca.crt");
    ssl_opts.pem_key_cert_pairs.push_back({
    ReadFile("certs/server.key"),
    ReadFile("certs/server.crt")
  });
  return grpc::SslServerCredentials(ssl_opts);
}

void run_server() {
    std::shared_ptr<grpc::Channel> chat_channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());

    auth_service auth_service;
    auth_service_data auth_service_data;
    ProfileServiceImpl profile_service;
    chat_service_ chat_service;
    chat_data_service chat_data_service;
    SearchService search_service;
    invitation_service invitation_service(chat_channel);
    invitation_data_service invitation_data_service;
    UsersInteractionServiceImpl users_interaction_service;
    // RecommendationServiceImpl recommendation_service;
    ProfileDataService profile_service_data;
    // recommendation_service.Init();
    // recommendation_service.computed = true;
    // recommendation_service.recomputeSimularity();

    // std::string key;
    // std::string cert;
    // std::string root;

    // auto key_path = std::filesystem::path("src/cert/server.key");
    // auto cert_path = std::filesystem::path("src/cert/server.crt");
    // auto root_path = std::filesystem::path("src/cert/ca.crt");

    auto current_path = std::filesystem::current_path();

    std::cout << "current path: " << current_path << std::endl;

    auto parent_path = current_path.parent_path();

    // read(parent_path / key_path, key);
    // read(parent_path / cert_path, cert);
    // read(parent_path / root_path, root);

    // auto key_cert_pair = grpc::SslServerCredentialsOptions::PemKeyCertPair{
    //     key, cert
    // };

    // grpc::SslServerCredentialsOptions ssl_options;
    // ssl_options.pem_root_certs = root;
    // ssl_options.pem_key_cert_pairs.push_back(key_cert_pair);

    // auto server_credentials = grpc::SslServerCredentials(ssl_options);
    RecommendationServiceImpl recommendation_service;
    recommendation_service.Init();


    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, GetServerCredentials());
    builder.RegisterService(&auth_service);
    builder.RegisterService(&profile_service);
    builder.RegisterService(&chat_service); 
    builder.RegisterService(&auth_service_data);
    builder.RegisterService(&chat_data_service);
    builder.RegisterService(&search_service);
    builder.RegisterService(&invitation_service);
    builder.RegisterService(&invitation_data_service);
    builder.RegisterService(&users_interaction_service);
    // builder.RegisterService(&recommendation_service);
    builder.RegisterService(&profile_service_data);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << '\n';
    server->Wait();
}

int main() {
    run_server();
}