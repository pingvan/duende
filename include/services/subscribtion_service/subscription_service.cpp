#include "subscription_service.hpp"

Status SubscriptionServiceImpl::Subscribe(
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
        Notification notification;
        notification.set_subscriber_id(request->subscriber_id());
        notification.set_subscribie_id(request->subscribie_id());
        notification.set_message("You have a new subscriber!");
        ServerWriter<Notification> writer(context);
        writer.Write(notification);
    }
    return Status::OK;
}

Status SubscriptionServiceImpl::Unsubscribe(
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

Status SubscriptionServiceImpl::ShowAllSubscriptions(
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

Status SubscriptionServiceImpl::ShowAllSubscribers(
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
