#pragma once

#include <grpcpp/grpcpp.h>

#include <memory>
#include <unordered_map>

#include "components/database/services/chat_service/chat_service.hpp"

#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.pb.h"


struct chat_service_ final : public chat_service::ChatHandler::Service {
    [[nodiscard]] static std::string make_timestamp();
    grpc::Status HandleChat(grpc::ServerContext* context, grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>* stream) override;
    void send_to_chat(const chat_service::Message& msg);
private:
    std::unordered_map<int, std::shared_ptr<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>> users_online;
    // std::unordered_map<int, grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests> *> users_online;
    // std::unordered_map<int, std::weak_ptr<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>> users_online_weak;
    // std::unordered_map<int, std::weak_ptr<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>> users_online_weak_created_locally;
};

struct chat_data_service final : public chat_service::ChatDataService::Service {
    grpc::Status GetUserChats(grpc::ServerContext* context, const chat_service::GetChatsRequest* request, chat_service::UserChats* response) override;
    grpc::Status GetLastMessages(grpc::ServerContext *context, const chat_service::GetChatMessagesRequest *request, chat_service::LastMessages *responst) override;
};
