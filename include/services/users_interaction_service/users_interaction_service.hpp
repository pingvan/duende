#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/users_interaction_service/users_interaction_service.hpp"
#include "services/users_interaction_service/users_interaction.grpc.pb.h"
#include "services/users_interaction_service/users_interaction.pb.h"
#include <vector>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using users_interaction::BlockRequest;
using users_interaction::BlockResponse;
using users_interaction::UnblockRequest;
using users_interaction::UnblockResponse;
using users_interaction::ShowAllBlockedRequest;
using users_interaction::ShowAllBlockedResponse;
using users_interaction::SubscribeRequest;
using users_interaction::SubscribeResponse;
using users_interaction::UnsubscribeRequest;
using users_interaction::UnsubscribeResponse;
using users_interaction::ShowAllSubscriptionsRequest;
using users_interaction::ShowAllSubscriptionsResponse;
using users_interaction::ShowAllSubscribersRequest;
using users_interaction::ShowAllSubscribersResponse;


class UsersInteractionServiceImpl final : public users_interaction::UsersInteractionService::Service {
    Status Block(
        ServerContext *context,
        const BlockRequest *request,
        BlockResponse *response
    ) override;

    Status Unblock(
        ServerContext *context,
        const UnblockRequest *request,
        UnblockResponse *response
    ) override;

    Status ShowAllBlocked(
        ServerContext *context,
        const ShowAllBlockedRequest *request,
        ShowAllBlockedResponse *response
    ) override;

    Status Subscribe(
        ServerContext *context,
        const SubscribeRequest *request,
        SubscribeResponse *response
    ) override;

    Status Unsubscribe(
        ServerContext *context,
        const UnsubscribeRequest *request,
        UnsubscribeResponse *response
    ) override;

    Status ShowAllSubscriptions(
        ServerContext *context,
        const ShowAllSubscriptionsRequest *request,
        ShowAllSubscriptionsResponse *response
    ) override;

    Status ShowAllSubscribers(
        ServerContext *context,
        const ShowAllSubscribersRequest *request,
        ShowAllSubscribersResponse *response
    ) override;
};