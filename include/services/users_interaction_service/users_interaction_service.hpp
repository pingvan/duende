#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/block_service/block_service.hpp"
#include "components/database/services/subscription_service/subscription_service.hpp"
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
using users_interaction::CheckIsMutualRequest;
using users_interaction::CheckIsMutualResponse;
using users_interaction::GetMutualSubscriptionsRequest;
using users_interaction::GetMutualSubscriptionsResponse;
using users_interaction::BoolResponse;

class UsersInteractionServiceImpl final : public users_interaction::UsersInteractionService::Service {
    database::subscription_service_database_api subscription_db_api;
    database::block_service_database_api block_db_api;
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

    Status CheckIsMutual(
        ServerContext *context,
        const CheckIsMutualRequest *request,
        CheckIsMutualResponse *response
    ) override;

    Status GetMutualSubscriptions(
        ServerContext *context,
        const GetMutualSubscriptionsRequest *request,
        GetMutualSubscriptionsResponse *response
    ) override;

    Status CheckIsBlocked(
        ServerContext *context,
        const BlockRequest *request,
        BoolResponse *response) override;

    Status CheckIsSubscribed(
        ServerContext *context,
        const SubscribeRequest *request,
        BoolResponse *response) override;
};