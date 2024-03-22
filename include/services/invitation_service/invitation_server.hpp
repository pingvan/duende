#pragma once

#include <grpcpp/grpcpp.h>

#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;
using invitation::InvitationRequest;
using invitation::InvitationResponse;
using invitation::InvitationReply;
using invitation::CheckRequest;
using invitation::CheckReply;
using invitation::InvitationService;

class InvitationServiceImpl final : public InvitationService::Service {
    Status SendInvitation(ServerContext* context, const InvitationRequest* request, ServerWriter<InvitationReply>* writer) override;
    Status RespondToInvitation(ServerContext* context, const InvitationResponse* request, ServerWriter<InvitationReply>* writer) override;
    Status CheckNewInvitations(ServerContext* context, const CheckRequest* request, ServerWriter<CheckReply>* writer) override;
};