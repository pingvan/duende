#include "subscription_service.hpp"

Status SubscriptionServiceImpl::SubscriptionStream(
    ServerContext *context,
    ServerReaderWriter<StreamResponse, StreamRequest> *stream
) {
    StreamRequest request;
    database::connector db_connector;
    while (stream->Read(&request)) {
        StreamResponse response;
        if (request.has_subscribe_request()) {
            const SubscribeRequest &subscribe_request =
                request.subscribe_request();
            if (db_connector.is_subscribed(
                    subscribe_request.subscriber_id(),
                    subscribe_request.subscribie_id()
                )) {
                response.mutable_subscribe_response()->set_message(
                    "Subscription already exists"
                );
            } else {
                db_connector.subscribe(
                    subscribe_request.subscriber_id(),
                    subscribe_request.subscribie_id()
                );
                response.mutable_subscribe_response()->set_message(
                    "Subscription made"
                );
                Notification *notification = response.mutable_notification();
                notification->set_subscriber_id(subscribe_request.sender_id());
                notification->set_subscribie_id(subscribe_request.subscribie_id(
                ));
                notification->set_message("followed you");
            }
        } else if (request.has_unsubscribe_request()) {
            const UnsubscribeRequest &unsubscribe_request =
                request.unsubscribe_request();
            if (db_connector.is_unsubscribed(
                    unsubscribe_request.subscriber_id(),
                    unsubscribe_request.subscribie_id()
                )) {
                response.mutable_unsubscribe_response()->set_message(
                    "Subscription does not exist"
                );
            } else {
                db_connector.unsubscribe(
                    unsubscribe_request.subscriber_id(),
                    unsubscribe_request.subscribie_id()
                );
                response.mutable_unsubscribe_response()->set_message(
                    "Unsubscription made"
                );
            }
        }
        else if (request.has_show_all_subscriptions_request()) {
            const ShowAllSubscriptionsRequest &show_all_subscriptions_request =
                request.show_all_subscriptions_request();
            std::vector<int> subscriptions = db_connector.get_all_subscriptions(
                show_all_subscriptions_request.subscriber_id()
            );
            ShowAllSubscriptionsResponse *show_all_subscriptions_response =
                response.mutable_show_all_subscriptions_response();
            for (int subscribie_id : subscriptions) {
                show_all_subscriptions_response->add_subscribie_id(subscribie_id
                );
            }
        } else if (request.has_show_all_subscribers_request()) {
            const ShowAllSubscribersRequest &show_all_subscribers_request =
                request.show_all_subscribers_request();
            std::vector<int> subscribers = db_connector.get_all_subscribers(
                show_all_subscribers_request.subscribie_id()
            );
            ShowAllSubscribersResponse *show_all_subscribers_response =
                response.mutable_show_all_subscribers_response();
            for (int subscriber_id : subscribers) {
                show_all_subscribers_response->add_subscriber_id(subscriber_id);
            }
        }
        stream->Write(response);
    }
    return Status::OK;
}