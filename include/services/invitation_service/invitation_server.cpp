#include "invitation_server.hpp"
#include <unordered_map>

std::unordered_map<
    int,
    grpc::ServerReaderWriter<
        InvitationEventResponse,
        InvitationEventRequest>*>
    online_users;

std::unordered_map<
    int,
    grpc::ServerReaderWriter<
        InvitationEventResponse,
        InvitationEventRequest>*>
    online_users_chat;

Status InvitationServiceImpl::InvitationStream(
    ServerContext *context,
    ServerReaderWriter<InvitationEventResponse, InvitationEventRequest> *stream
) {
    InvitationEventRequest request;
    database::invite_service_database_api invite_db_api;
    int user_id = -1;
    while (stream->Read(&request)) {
        InvitationEventResponse response;
        if (request.has_send_invitation_request()) {
            const SendInvitationRequest &send_request =
                request.send_invitation_request();
            user_id = send_request.sender_id();
            online_users[user_id] = stream;
            if (invite_db_api.is_invite_already_sended(
                    send_request.sender_id(), send_request.receiver_id()
                )) {
                return grpc::Status(
                    grpc::ALREADY_EXISTS, "Invitation already sent"
                );
            } else {
                invite_db_api.add_invite(
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
                    int invite_id = invite_db_api.add_invite(
                        send_request.sender_id(), send_request.receiver_id(),
                        send_request.invite_text()
                    );
                    invite_db_api.add_unhandled_invite(
                        send_request.receiver_id(), invite_id
                    );
                }
            }
        } else if (request.has_respond_to_invitation_request()) {
            const RespondToInvitationRequest &respond_request =
                request.respond_to_invitation_request();
            int invite_id = respond_request.invite_id();
            if (respond_request.accept()) {
                invite_db_api.change_invite_status(
                    invite_id, database::invite_status::accepted
                );
                response.mutable_respond_to_invitation_response()->set_message(
                    "Invitation accepted"
                );
                Notification *notification = response.mutable_notification();
                notification->set_sender_id(respond_request.sender_id());
                notification->set_receiver_id(respond_request.receiver_id());
                notification->set_message("Your invitation was accepted");
                if (online_users.count(respond_request.sender_id())) {
                    online_users[respond_request.sender_id()]->Write(response);
                }
            } else {
                invite_db_api.change_invite_status(
                    invite_id, database::invite_status::declined
                );
                response.mutable_respond_to_invitation_response()->set_message(
                    "Invitation declined"
                );
            }
            stream->Write(response);
        } else if (request.has_init()) {
            user_id = request.init().user_id();
            online_users.insert({user_id, stream});
            online_users_chat.insert({user_id, stream});
            response.mutable_init_response()->set_status(Status::SUCCESS);
            stream->Write(response);
        } else if (request.has_leave()) {
            user_id = request.leave().user_id();
            online_users.erase(user_id);
            online_users_chat.erase(user_id);
            response.mutable_leave_response()->set_status(Status::SUCCESS);
            stream->Write(response);
        }
    }
    if (user_id != -1) {
        online_users.erase(user_id);
    }
}