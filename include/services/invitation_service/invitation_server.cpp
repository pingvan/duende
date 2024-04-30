#include "invitation_server.hpp"
#include <unordered_map>

std::unordered_map<int, ServerReaderWriter<StreamResponse, StreamRequest> *>
    online_users;

Status InvitationServiceImpl::InvitationStream(
    ServerContext *context,
    ServerReaderWriter<StreamResponse, StreamRequest> *stream
) {
    StreamRequest request;
    database::invite_service_database_api db_api;
    int user_id = -1;
    while (stream->Read(&request)) {
        StreamResponse response;
        if (request.has_send_invitation_request()) {
            const SendInvitationRequest &send_request =
                request.send_invitation_request();
            user_id = send_request.sender_id();
            online_users[user_id] = stream;
            if (db_api.is_invite_already_sended(
                    send_request.sender_id(), send_request.receiver_id()
                )) {
                return grpc::Status(
                    grpc::ALREADY_EXISTS, "Invitation already sent"
                );
            } else {
                db_api.add_invite(
                    "Invitation", database::invite_status::sent,
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
                } else {
                    int invite_id = db_api.add_invite(
                        send_request.sender_id(), send_request.receiver_id(),
                        send_request.invite_text()
                    );
                    uint32 chat_id = db_api.create_chat(
                        send_request.sender_id(), send_request.receiver_id()
                    );
                    db_api.send_message(
                        chat_id, send_request.sender_id(),
                        send_request.invite_text()
                    );
                    db_api.add_unhandled_invite(
                        send_request.receiver_id(), invite_id
                    );
                    response.mutable_send_invitation_response()->set_message(
                        "Invitation sent and chat created"
                    );
                }
            }
        }
    }
    else if (request.has_respond_to_invitation_request()) {
        const RespondToInvitationRequest &respond_request =
            request.respond_to_invitation_request();
        int invite_id = respond_request.invite_id();
        if (respond_request.accept()) {
            db_api.change_invite_status(
                invite_id, database::invite_status::accepted
            );
            response.mutable_respond_to_invitation_response()->set_message(
                "Invitation accepted and chat created"
            );
            Notification *notification = response.mutable_notification();
            notification->set_sender_id(respond_request.sender_id());
            notification->set_receiver_id(respond_request.receiver_id());
            notification->set_message("Your invitation was accepted");
            if (online_users.count(respond_request.sender_id())) {
                online_users[respond_request.sender_id()]->Write(response);
            }
        } else {
            db_api.change_invite_status(
                invite_id, database::invite_status::declined
            );
            response.mutable_respond_to_invitation_response()->set_message(
                "Invitation declined"
            );
        }
        stream->Write(response);
    }
}

if (user_id != -1) {
    online_users.erase(user_id);
}
return Status::OK;
}