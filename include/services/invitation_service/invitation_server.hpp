#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/invite_service/invite_service.hpp"
#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using invitation::InitRequest;
using invitation::InitResponse;
using invitation::InvitationEventRequest;
using invitation::InvitationEventResponse;
using invitation::InvitationService;
using invitation::LeaveRequest;
using invitation::LeaveResponse;
using invitation::Notification;
using invitation::RespondToInvitationRequest;
using invitation::RespondToInvitationResponse;
using invitation::SendInvitationRequest;
using invitation::SendInvitationResponse;

class InvitationServiceImpl final : public InvitationService::Service {
    Status InvitationStream(
        ServerContext *context,
        ServerReaderWriter<InvitationEventResponse, InvitationEventRequest>
            *stream
    ) override;

private:
    std::unordered_map<
        int,
        std::shared_ptr<grpc::ServerReaderWriter<
            InvitationEventResponse,
            InvitationEventRequest>>>
        users_online;

    std::unordered_map<
        int,
        std::shared_ptr<grpc::ServerReaderWriter<
            InvitationEventResponse,
            InvitationEventRequest>>>
        users_online_chat;
};