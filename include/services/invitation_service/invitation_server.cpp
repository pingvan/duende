#include "invitation_server.hpp"
#include <unordered_map>

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
                response.mutable_send_invitation_response()->set_status(
                    Status::ERROR
                );
            } else {
                invite_db_api.add_invite(
                    "Invitation", database::invite_status::sent,
                    send_request.sender_id(), send_request.receiver_id()
                );
                response.mutable_send_invitation_response()->set_status(
                    InviteStatus::SENT
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
                response.mutable_respond_to_invitation_response()->set_status(
                    InviteStatus::ACCEPTED
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
                response.mutable_respond_to_invitation_response()->set_status(
                    InviteStatus::DECLINED
                );
            }
            stream->Write(response);
        } else if (request.has_init()) {
            users_online_chat.insert(
                {request.init().user_id(),
                 std::make_shared<grpc::ServerReaderWriter<
                     InvitationEventResponse, InvitationEventRequest>>(*stream)}
            );
            InvitationEventResponse response;
            const auto init_response = new InitialResponse;
            init_response->set_status(InitialResponse::SUCCESS);
            response.set_allocated_init(init_response);
            stream->Write(response);
        } else if (request.has_leave()) {
            users_online_chat.erase(request.leave().user_id());
            InvitationEventResponse response;
            const auto disconnect_response = new LeaveResponse;
            disconnect_response->set_status(LeaveResponse::SUCCESS);
            response.set_allocated_leave(disconnect_response);
            stream->Write(response);
        }
    }
    if (user_id != -1) {
        online_users.erase(user_id);
    }
}