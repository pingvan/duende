#pragma once

#include <grpcpp/grpcpp.h>

struct interceptor : public grpc::experimental::Interceptor {
public:

    interceptor() = default;

    void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override {
        if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::PRE_SEND_INITIAL_METADATA)) {
            std::cout << "Token attached\n";
        }
        methods->Proceed();
    }
};

struct interceptorFactory : public grpc::experimental::ClientInterceptorFactoryInterface {
public:
    grpc::experimental::Interceptor* CreateClientInterceptor(grpc::experimental::ClientRpcInfo* info) override {
        return new interceptor();
    }
};