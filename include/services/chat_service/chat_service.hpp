#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>
#include <unordered_map>
#include "components/aes/aes.hpp"
#include "components/database/services/chat_service/chat_service.hpp"
#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.pb.h"
#include "services/users_interaction_service/users_interaction.grpc.pb.h"
#include "services/users_interaction_service/users_interaction.pb.h"
#include "services/users_interaction_service/users_interaction_service.hpp"

struct chat_service_ final : public chat_service::ChatHandler::Service {
    [[nodiscard]] static std::string make_timestamp();
    [[nodiscard]] static chat_service::AcceptMessageResponse *
    make_accept_message_response(
        const chat_service::MessageSend &msg,
        const std::string &timestamp
    );
    grpc::Status HandleChat(
        grpc::ServerContext *context,
        grpc::ServerReaderWriter<
            chat_service::ChatResponses,
            chat_service::ChatRequests> *stream
    ) override;
    grpc::Status InvitesToChat(
        grpc::ServerContext *context,
        grpc::ServerReaderWriter<
            chat_service::InvitesToChatResponses,
            chat_service::InvitesToChatRequests> *stream
    ) override;
    grpc::Status CreatePrivateChat(
        grpc::ServerContext *context,
        const chat_service::CreatePrivateChatRequest *request,
        chat_service::ChatPreview *response
    ) override;
    grpc::Status CreateGroupChat(
        grpc::ServerContext *context,
        const chat_service::CreateGroupChatRequest *request,
        chat_service::ChatPreview *response
    ) override;
    grpc::Status CheckUserPrivileges(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        chat_service::UserPrivilegeResponse *response
    ) override;
    grpc::Status ChangeChatName(
        grpc::ServerContext *context,
        const chat_service::ChangeChatNameRequest *request,
        chat_service::OperationStatusResponse *response
    ) override;
    grpc::Status ChangeChatDescription(
        grpc::ServerContext *context,
        const chat_service::ChangeChatDescriptionRequest *request,
        chat_service::OperationStatusResponse *response
    ) override;
    grpc::Status ChangeChatCover(
        grpc::ServerContext *context,
        const chat_service::ChangeChatCoverRequest *request,
        chat_service::OperationStatusResponse *response
    ) override;
    grpc::Status AddPrivilegesToUser(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        chat_service::OperationStatusResponse *response
    ) override;
    grpc::Status RemovePrivilegesFromUser(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        chat_service::OperationStatusResponse *response
    ) override;
    grpc::Status AddUserToChat(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        chat_service::OperationStatusResponse *response
    ) override;
    chat_service::ChatResponses send_to_chat(
        const chat_service::MessageSend &msg
    );

private:
    std::unordered_map<
        int,
        std::shared_ptr<grpc::ServerReaderWriter<
            chat_service::ChatResponses,
            chat_service::ChatRequests>>>
        users_online;
    std::unordered_map<
        int,
        std::shared_ptr<grpc::ServerReaderWriter<
            chat_service::InvitesToChatResponses,
            chat_service::InvitesToChatRequests>>>
        users_invites_online;
};

struct chat_data_service final : public chat_service::ChatDataService::Service {
    grpc::Status GetUserChats(
        grpc::ServerContext *context,
        const chat_service::GetChatsRequest *request,
        chat_service::UserChats *response
    ) override;
    grpc::Status GetLastMessages(
        grpc::ServerContext *context,
        const chat_service::GetChatMessagesRequest *request,
        chat_service::LastMessages *responst
    ) override;
    grpc::Status GetChatMembers(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        chat_service::GetChatMembersResponse *response
    ) override;
    grpc::Status CheckUserInChat(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        chat_service::BoolResponse *response
    ) override;
    grpc::Status GetChatPreview(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        chat_service::ChatPreview *response
    ) override;
    grpc::Status GetGroupChatPreview(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        common::GroupChatPreview *response
    ) override;
    grpc::Status GetChatEvents(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        chat_service::ChatEvents *response
    ) override;
    grpc::Status GetChatAdmins(
        grpc::ServerContext *context,
        const chat_service::UserChat *request,
        chat_service::ChatAdmins *response
    ) override;
};
