#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/invite_service/invite_service.hpp"
#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"
#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.pb.h"

struct invitation_service final : public invitation::InvitationService::Service {
    invitation_service(const std::shared_ptr<grpc::Channel> &channel) : chat_stub(chat_service::ChatHandler::NewStub(channel)) {}

    grpc::Status InvitationStream(
        grpc::ServerContext *context,
        grpc::ServerReaderWriter<invitation::InvitationResponse, invitation::InvitationRequest>
            *stream
    ) override;

    chat_service::ChatPreview *create_private_chat(int first_id, int second_id);

private:
    std::unique_ptr<chat_service::ChatHandler::Stub> chat_stub;
    std::unordered_map<
        int,
        std::shared_ptr<grpc::ServerReaderWriter<
            invitation::InvitationResponse,
            invitation::InvitationRequest>>>
        users_online;
};

struct invitation_data_service final : public invitation::InvitationDataService::Service {
    grpc::Status GetReceivedInvitations(
        grpc::ServerContext *context,
        const invitation::RequestWithId *request,
        invitation::InvitesReceived *response
    ) override;
    grpc::Status GetSentInvitations(
        grpc::ServerContext *context,
        const invitation::RequestWithId *request,
        invitation::InvitesSent *response
    ) override;
};