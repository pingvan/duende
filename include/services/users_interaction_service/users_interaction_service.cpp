#include "users_interaction_service.hpp"
#include <cstdint>

Status UsersInteractionServiceImpl::Block(
    ServerContext *context,
    const BlockRequest *request,
    BlockResponse *response
) {
    if (block_db_api.check_is_blocked(request->blocker_id(), request->blockie_id())) {
        return Status(grpc::ALREADY_EXISTS, "User is already blocked");
    } else {
        block_db_api.block_user(request->blocker_id(), request->blockie_id());
        response->set_message("User blocked");
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::Unblock(
    ServerContext *context,
    const UnblockRequest *request,
    UnblockResponse *response
) {
    if (!block_db_api.check_is_blocked(request->blocker_id(), request->blockie_id())) {
        return Status(grpc::NOT_FOUND, "User is not blocked");
    } 
    // else {
    //     db_connector.unblock_user(request->blocker_id(), request->blockie_id());
    //     response->set_message("User unblocked");
    // }
    return Status::OK;
}

Status UsersInteractionServiceImpl::ShowAllBlocked(
    ServerContext *context,
    const ShowAllBlockedRequest *request,
    ShowAllBlockedResponse *response
) {
    std::vector<uint32_t> blocked_users = block_db_api.get_blocked_users(request->blockie_id());
    for (auto blockie_id : blocked_users) {
        response->add_blockie_id(blockie_id);
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::Subscribe(
    ServerContext *context,
    const SubscribeRequest *request,
    SubscribeResponse *response
) {
    if (subscription_db_api.check_is_subscribed(request->subscriber_id(), request->subscribie_id())) {
        return Status(grpc::ALREADY_EXISTS, "Subscription already exists");
    } else {
        subscription_db_api.subscribe_to_user(request->subscriber_id(), request->subscribie_id());
        response->set_message("Subscription made");
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::Unsubscribe(
    ServerContext *context,
    const UnsubscribeRequest *request,
    UnsubscribeResponse *response
) {
    if (!subscription_db_api.check_is_subscribed(request->subscriber_id(), request->subscribie_id())) {
        return Status(grpc::NOT_FOUND, "Subscription does not exist");
    } else {
        subscription_db_api.unsubscribe_from_user(request->subscriber_id(), request->subscribie_id());
        response->set_message("Unsubscription made");
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::ShowAllSubscriptions(
    ServerContext *context,
    const ShowAllSubscriptionsRequest *request,
    ShowAllSubscriptionsResponse *response
) {
    std::vector<uint32_t> subscriptions = subscription_db_api.get_subscriptions(request->subscriber_id());
    for (int subscribie_id : subscriptions) {
        response->add_subscribie_id(subscribie_id);
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::ShowAllSubscribers(
    ServerContext *context,
    const ShowAllSubscribersRequest *request,
    ShowAllSubscribersResponse *response
) {
    std::vector<uint32_t> subscribers = subscription_db_api.get_subscribers(request->subscribie_id());
    for (int subscriber_id : subscribers) {
        response->add_subscriber_id(subscriber_id);
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::CheckIsMutual(
    ServerContext *context,
    const CheckIsMutualRequest *request,
    CheckIsMutualResponse *response
) {
    bool is_mutual = subscription_db_api.check_is_mutual(request->subscriber_id(), request->subscribie_id());
    response->set_is_mutual(is_mutual);
    return Status::OK;
}

Status UsersInteractionServiceImpl::GetMutualSubscriptions(
    ServerContext *context,
    const GetMutualSubscriptionsRequest *request,
    GetMutualSubscriptionsResponse *response
) {
    std::vector<uint32_t> subscriptions = subscription_db_api.get_subscriptions(request->subscriber_id());
    for (int subscription_id : subscriptions) {
        if (subscription_db_api.check_is_mutual(request->subscriber_id(), subscription_id)) {
            response->add_subscribie_id(subscription_id);
        }
    }
    return Status::OK;
}
