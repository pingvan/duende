#pragma once

#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>

#include "services/authservice/token.hpp"

class Processor : public grpc::AuthMetadataProcessor {
public:
    grpc::Status Process(const grpc::AuthMetadataProcessor::InputMetadata& auth_metadata,
                        grpc::AuthContext* context,
                        grpc::AuthMetadataProcessor::OutputMetadata* consumed_auth_metadata,
                        grpc::AuthMetadataProcessor::OutputMetadata* response_metadata) override {
        auto auth_header = auth_metadata.find("authorization");
        if (auth_header == auth_metadata.end()) {
            return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Authorization header missing.");
        }

        std::string auth_value = auth_header->second.data();
        const std::string bearer_prefix = "Bearer ";
        if (auth_value.substr(0, bearer_prefix.size()) != bearer_prefix) {
            return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Bearer token missing.");
        }
        std::string token = auth_value.substr(bearer_prefix.size());

        if (m_token_service.is_access_token_active(token).error_code() == grpc::StatusCode::UNAUTHENTICATED) {
            return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Invalid or expired token. Refresh access token.");
        }   

      return grpc::Status::OK;
    }

    bool IsBlocking() const override { return false; }

private:
    token_service m_token_service;
};