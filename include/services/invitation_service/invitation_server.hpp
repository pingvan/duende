#pragma once

#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using invitation::StreamRequest;
using invitation::StreamResponse;
using invitation::InvitationService;
using invitation::SendInvitationRequest;
using invitation::RespondToInvitationRequest;
using invitation::CheckNewInvitationsRequest;
using invitation::Notification;

class InvitationServiceImpl final : public InvitationService::Service {
    Status InvitationStream(
        ServerContext *context,
        ServerReaderWriter<StreamResponse, StreamRequest> *stream
    ) override;
};

struct InvitationDataService final : public invitation::InvitionDataService::Service {
    grpc::Status GetReceivedInvitations(grpc::ServerContext *context, const invitation::RequestWithId *request, invitation::InvitesReceived *response) override;
    grpc::Status GetSentInvitations(grpc::ServerContext *context, const invitation::RequestWithId *request, invitation::InvitesSent *response) override;
};