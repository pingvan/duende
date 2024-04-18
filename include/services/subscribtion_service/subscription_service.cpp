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
            const SubscribeRequest& subscribe_request = request.subscribe_request();
            if (db_connector.is_subscribed(
                    subscribe_request.subscriber_id(), subscribe_request.subscribie_id()
                )) {
                response.mutable_subscribe_response()->set_message("Subscription already exists");
            } else {
                db_connector.subscribe(
                    subscribe_request.subscriber_id(), subscribe_request.subscribie_id()
                );
                response.mutable_subscribe_response()->set_message("Subscription made");
                Notification* notification = response.mutable_notification();
                notification->set_subscriber_id(subscribe_request.sender_id());
                notification->set_subscribie_id(subscribe_request.subscribie_id());
                notification->set_message("followed you");
            }
        } else if (request.has_unsubscribe_request()) {
            const UnsubscribeRequest& unsubscribe_request = request.unsubscribe_request();
            if (db_connector.is_unsubscribed(
                    unsubscribe_request.subscriber_id(), unsubscribe_request.subscribie_id()
                )) {
                response.mutable_unsubscribe_response()->set_message("Subscription does not exist");
            } else {
                db_connector.unsubscribe(
                    unsubscribe_request.subscriber_id(), unsubscribe_request.subscribie_id()
                );
                response.mutable_unsubscribe_response()->set_message("Unsubscription made");
            }
        }
        stream->Write(response);
    }
    return Status::OK;
}