#include "users_interaction_service.hpp"

Status UsersInteractionServiceImpl::Block(
    ServerContext *context,
    const BlockRequest *request,
    BlockResponse *response
) {
    database::connector db_connector;
    if (db_connector.is_blocked(request->blocker_id(), request->blockie_id())) {
        return Status(grpc::ALREADY_EXISTS, "User is already blocked");
    } else {
        db_connector.block_user(request->blocker_id(), request->blockie_id());
        response->set_message("User blocked");
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::Unblock(
    ServerContext *context,
    const UnblockRequest *request,
    UnblockResponse *response
) {
    database::connector db_connector;
    if (!db_connector.is_blocked(request->blocker_id(), request->blockie_id())) {
        return Status(grpc::NOT_FOUND, "User is not blocked");
    } else {
        db_connector.unblock_user(request->blocker_id(), request->blockie_id());
        response->set_message("User unblocked");
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::ShowAllBlocked(
    ServerContext *context,
    const ShowAllBlockedRequest *request,
    ShowAllBlockedResponse *response
) {
    database::connector db_connector;
    std::vector<int> blocked_users = db_connector.get_all_blocked(request->blockie_id());
    for (int blockie_id : blocked_users) {
        response->add_blockie_id(blockie_id);
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::Subscribe(
    ServerContext *context,
    const SubscribeRequest *request,
    SubscribeResponse *response
) {
    database::connector db_connector;
    if (db_connector.is_subscribed(request->subscriber_id(), request->subscribie_id())) {
        return Status(grpc::ALREADY_EXISTS, "Subscription already exists");
    } else {
        db_connector.subscribe(request->subscriber_id(), request->subscribie_id());
        response->set_message("Subscription made");
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::Unsubscribe(
    ServerContext *context,
    const UnsubscribeRequest *request,
    UnsubscribeResponse *response
) {
    database::connector db_connector;
    if (db_connector.is_unsubscribed(request->subscriber_id(), request->subscribie_id())) {
        return Status(grpc::NOT_FOUND, "Subscription does not exist");
    } else {
        db_connector.unsubscribe(request->subscriber_id(), request->subscribie_id());
        response->set_message("Unsubscription made");
    }
    return Status::OK;
}

Status UsersInteractionServiceImpl::ShowAllSubscriptions(
    ServerContext *context,
    const ShowAllSubscriptionsRequest *request,
    ShowAllSubscriptionsResponse *response
) {
    database::connector db_connector;
    std::vector<int> subscriptions = db_connector.get_all_subscriptions(request->subscriber_id());
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
    database::connector db_connector;
    std::vector<int> subscribers = db_connector.get_all_subscribers(request->subscribie_id());
    for (int subscriber_id : subscribers) {
        response->add_subscriber_id(subscriber_id);
    }
    return Status::OK;
}
