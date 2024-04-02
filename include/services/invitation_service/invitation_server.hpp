#pragma once

#include <grpcpp/grpcpp.h>
#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using invitation::CheckReply;
using invitation::CheckRequest;
using invitation::InvitationReply;
using invitation::InvitationRequest;
using invitation::InvitationResponse;
using invitation::InvitationService;

class InvitationServiceImpl final : public InvitationService::Service {
    Status SendInvitation(
        ServerContext *context,
        ServerReader<InvitationRequest> *reader,
        ServerWriter<InvitationReply> *writer
    ) override;
    Status RespondToInvitation(
        ServerContext *context,
        ServerReader<InvitationResponse> *reader,
        ServerWriter<InvitationReply> *writer
    ) override;
    Status CheckNewInvitations(
        ServerContext *context,
        ServerReader<CheckRequest> *reader,
        ServerWriter<CheckReply> *writer
    ) override;
};