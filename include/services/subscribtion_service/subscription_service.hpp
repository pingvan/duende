#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/subscription_service/subscription_service.hpp"
#include "services/subscription_service/subscription.grpc.pb.h"
#include "services/subscription_service/subscription.pb.h"
#include <vector>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using subscription::SubscribeRequest;
using subscription::SubscribeResponse;
using subscription::UnsubscribeRequest;
using subscription::UnsubscribeResponse;
using subscription::ShowAllSubscriptionsRequest;
using subscription::ShowAllSubscriptionsResponse;
using subscription::ShowAllSubscribersRequest;
using subscription::ShowAllSubscribersResponse;
using subscription::Notification;

class SubscriptionServiceImpl final : public subscription::SubscriptionService::Service {
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