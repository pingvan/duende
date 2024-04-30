#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/invite_service/invite_service.hpp"
#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"
#include "services/chat_service/chat_service.hpp"

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
using invitation::Notification;
using chat_service::ChatRequests;
using chat_service::ChatResponses;
using chat_service::ChatHandler;

class InvitationServiceImpl final : public InvitationService::Service {
    Status InvitationStream(
        ServerContext *context,
        ServerReaderWriter<StreamResponse, StreamRequest> *stream
    ) override;
};

class ChatHandlerImpl final : public ChatHandler::Service {
    Status HandleChat(
        ServerContext *context,
        ServerReaderWriter<ChatResponses, ChatRequests> *stream
    ) override;
};