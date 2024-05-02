#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/invite_service/invite_service.hpp"
#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"

struct InvitationServiceImpl final : public invitation::InvitationService::Service {
    grpc::Status InvitationStream(
        grpc::ServerContext *context,
        grpc::ServerReaderWriter<invitation::InvitationResponse, invitation::InvitationRequest>
            *stream
    ) override;

private:
    std::unordered_map<
        int,
        std::shared_ptr<grpc::ServerReaderWriter<
            invitation::InvitationResponse,
            invitation::InvitationRequest>>>
        users_online;
};

struct invitation_service_data final : public invitation::InvitationDataService::Service {
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