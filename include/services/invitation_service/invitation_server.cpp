#include "invitation_server.hpp"
#include <unordered_map>

std::unordered_map<int, ServerReaderWriter<StreamResponse, StreamRequest> *>
    online_users;

Status InvitationServiceImpl::InvitationStream(
    ServerContext *context,
    ServerReaderWriter<StreamResponse, StreamRequest> *stream
) {
    StreamRequest request;
    database::connector db_connector;
    int user_id = -1;
    while (stream->Read(&request)) {
        StreamResponse response;
        if (request.has_send_invitation_request()) {
            const SendInvitationRequest &send_request =
                request.send_invitation_request();
            user_id = send_request.sender_id();
            online_users[user_id] = stream;
            if (db_connector.is_invitation_sent(
                    send_request.sender_id(), send_request.receiver_id()
                )) {
                return grpc::Status(
                    grpc::ALREADY_EXISTS, "Invitation already sent"
                );
            } else {
                db_connector.send_invitation(
                    send_request.sender_id(), send_request.receiver_id()
                );
                response.mutable_send_invitation_response()->set_message(
                    "Invitation sent"
                );
                Notification *notification = response.mutable_notification();
                notification->set_sender_id(send_request.sender_id());
                notification->set_receiver_id(send_request.receiver_id());
                notification->set_message("You have a new invitation");
                if (online_users.count(send_request.receiver_id())) {
                    online_users[send_request.receiver_id()]->Write(response);
                }
            }
        } else if (request.has_respond_to_invitation_request()) {
            const RespondToInvitationRequest &respond_request =
                request.respond_to_invitation_request();
            if (db_connector.is_invitation_accepted(respond_request.receiver_id(
                ))) {
                return grpc::Status(
                    grpc::ALREADY_EXISTS, "Invitation already accepted"
                );
            } else {
                db_connector.respond_to_invitation(
                    respond_request.receiver_id(), respond_request.accept()
                );
                if (respond_request.accept()) {
                    db_connector.create_chat(respond_request.receiver_id());
                    response.mutable_respond_to_invitation_response()
                        ->set_message("Invitation accepted and chat created");
                    Notification *notification =
                        response.mutable_notification();
                    notification->set_sender_id(respond_request.sender_id());
                    notification->set_receiver_id(respond_request.receiver_id()
                    );
                    notification->set_message("Your invitation was accepted");
                    if (online_users.count(respond_request.sender_id())) {
                        online_users[respond_request.sender_id()]->Write(
                            response
                        );
                    }
                } else {
                    response.mutable_respond_to_invitation_response()
                        ->set_message("Invitation declined");
                }
                stream->Write(response);
            }
        }
    }
    if (user_id != -1) {
        online_users.erase(user_id);
    }
    return Status::OK;
}