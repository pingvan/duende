#include <grpcpp/grpcpp.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "services/authservice/service.hpp"
#include "services/profile_service/profile_server.hpp"
#include "services/invitation_service/invitation_server.hpp"
#include "services/chat_service/chat_service.hpp"
#include "services/search_service/search_service.hpp"

std::string server_address("0.0.0.0:50051");

struct interceptor : public grpc::experimental::Interceptor {

    interceptor() = default;

    void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override {
        std::cout << "Intercepting\n";
        if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::POST_RECV_INITIAL_METADATA)) {
            std::string token;
            for (auto& pair : *methods->GetRecvInitialMetadata()) {
                std::cout << "Key: " << pair.first << " Value: " << std::string(pair.second.begin(), pair.second.end()) << '\n';
                if (pair.first == "token") {
                    token = std::string(pair.second.begin(), pair.second.end());
                    break;
                }
            }
            std::cout << "Token: " << token << '\n';

            if (token == "access_token" || token == "") {
                std::cout << " token is valid " << token << "\n";
                methods->Proceed();
            } else {
                std::cout << " token is no valid " << token << "\n";
                // grpc::Status status(grpc::StatusCode::UNAUTHENTICATED, "you are UNAUTHENTICATED");
                // methods->ModifySendStatus(status);
            }
        }
        else {
            methods->Proceed();
        }
    }
};

struct interceptorFactory : public grpc::experimental::ServerInterceptorFactoryInterface {
public:
    grpc::experimental::Interceptor* CreateServerInterceptor(grpc::experimental::ServerRpcInfo* info) override {
        return new interceptor();
    }
};

void read(const std::string &path, std::string& content) {
    // std::cout << "Reading file: " << path << "\n";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << '\n';
        std::exit(1);
    }
    std::string line;
    while (std::getline(file, line)) {
        // std::cout << line << '\n';
        content += line + '\n';
    }
    // std::cout << "File read successfully\n";
}

void run_server() {
    auth_service auth_service;
    auth_service_data auth_service_data;
    ProfileServiceImpl profile_service;
    chat_service_ chat_service;
    chat_data_service chat_data_service;
    SearchService search_service;

    std::string key;
    std::string cert;
    std::string root;

    auto key_path = std::filesystem::path("src/cert/server.key");
    auto cert_path = std::filesystem::path("src/cert/server.crt");
    auto root_path = std::filesystem::path("src/cert/ca.crt");

    auto current_path = std::filesystem::current_path();
    auto parent_path = current_path.parent_path();

    read(parent_path / key_path, key);
    read(parent_path / cert_path, cert);
    read(parent_path / root_path, root);

    auto key_cert_pair = grpc::SslServerCredentialsOptions::PemKeyCertPair{
        key, cert
    };

    grpc::SslServerCredentialsOptions ssl_options;
    ssl_options.pem_root_certs = root;
    ssl_options.pem_key_cert_pairs.push_back(key_cert_pair);

    auto server_credentials = grpc::SslServerCredentials(ssl_options);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, server_credentials);
    builder.RegisterService(&auth_service);
    builder.RegisterService(&profile_service);
    builder.RegisterService(&chat_service); 
    builder.RegisterService(&auth_service_data);
    builder.RegisterService(&chat_data_service);
    builder.RegisterService(&search_service);

    std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>> creators;
    creators.push_back(std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>(new interceptorFactory()));
    builder.experimental().SetInterceptorCreators(std::move(creators));

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << '\n';
    server->Wait();
}

int main() {
    run_server();
}