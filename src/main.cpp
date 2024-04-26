#include <grpcpp/grpcpp.h>
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