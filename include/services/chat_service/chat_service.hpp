#pragma once

#include <grpcpp/grpcpp.h>

#include <memory>
#include <unordered_map>

#include "components/database/services/chat_service/chat_service.hpp"

#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.pb.h"


struct chat_service_ final : public chat_service::chat_handler::Service {
    [[nodiscard]] static std::string make_timestamp();
    grpc::Status handle_chat(grpc::ServerContext* context, grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>* stream) override;
    void send_to_chat(const chat_service::Message& msg);
private:
    std::unordered_map<int, std::shared_ptr<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>> users_online;
};
