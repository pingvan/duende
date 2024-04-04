#include "invitation_server.hpp"
#include "components/database/database-connector.hpp"

Status InvitationServiceImpl::InvitationStream(
    ServerContext *context,
    ServerReaderWriter<StreamResponse, StreamRequest> *stream
) {
    StreamRequest request;
    database::connector db_connector;
    while (stream->Read(&request)) {
        StreamResponse response;
        if (request.has_send_invitation_request()) {
            const SendInvitationRequest& send_request = request.send_invitation_request();
            if (db_connector.is_invitation_sent(
                    send_request.sender_id(), send_request.receiver_id()
                )) {
                response.mutable_send_invitation_response()->set_message("Invitation already sent");
            } else {
                db_connector.send_invitation(
                    send_request.sender_id(), send_request.receiver_id()
                );
                response.mutable_send_invitation_response()->set_message("Invitation sent");
                Notification* notification = response.mutable_notification();
                notification->set_sender_id(send_request.sender_id());
                notification->set_receiver_id(send_request.receiver_id());
                notification->set_message("wants to chat with you");
            }
        } else if (request.has_respond_to_invitation_request()) {
            const RespondToInvitationRequest& respond_request = request.respond_to_invitation_request();
            if (db_connector.is_invitation_accepted(
                    respond_request.receiver_id()
                )) {
                response.mutable_respond_to_invitation_response()->set_message("Invitation already accepted");
            } else {
                db_connector.respond_to_invitation(
                    respond_request.receiver_id(), respond_request.accept()
                );
                if (respond_request.accept()) {
                    db_connector.create_chat(
                        respond_request.receiver_id()
                    );
                    response.mutable_respond_to_invitation_response()->set_message("Invitation accepted and chat created");
                    Notification* notification = response.mutable_notification();
                    notification->set_sender_id(respond_request.sender_id());
                    notification->set_receiver_id(respond_request.receiver_id());
                    notification->set_message("accepted your invitation, chat created");
                } else {
                    response.mutable_respond_to_invitation_response()->set_message("Invitation declined");
                    //мб в случае отказа не уведомлять
                    Notification* notification = response.mutable_notification();
                    notification->set_sender_id(respond_request.sender_id());
                    notification->set_receiver_id(respond_request.receiver_id());
                    notification->set_message("declined your invitation");
                }
            }
        } else if (request.has_check_new_invitations_request()) {
            const CheckNewInvitationsRequest& check_request = request.check_new_invitations_request();
            if (db_connector.is_invitation_checked(check_request.receiver_id())) {
                response.mutable_check_new_invitations_response()->set_message("Invitations already checked");
            } else {
                db_connector.check_new_invitations(check_request.receiver_id());
                response.mutable_check_new_invitations_response()->set_message("Invitations checked");
            }
        }
        stream->Write(response);
    }
    return Status::OK;
}