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
using invitation::InvitationEventRequest;
using invitation::InvitationEventResponse;
using invitation::InvitationService;
using invitation::SendInvitationRequest;
using invitation::SendInvitationResponse;
using invitation::RespondToInvitationRequest;
using invitation::RespondToInvitationResponse;
using invitation::InitRequest;
using invitation::InitResponse;
using invitation::LeaveRequest;
using invitation::LeaveResponse;
using invitation::Notification;

class InvitationServiceImpl final : public InvitationService::Service {
    Status InvitationStream(
        ServerContext *context,
        ServerReaderWriter<InvitationEventResponse, InvitationEventRequest> *stream
    ) override;
};