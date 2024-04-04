#include "invitation_server.hpp"
#include <grpcpp/support/status.h>
#include "components/database/services/invite_service/invite_service.hpp"

/*Status InvitationServiceImpl::InvitationStream(
    ServerContext *context,
    ServerReaderWriter<StreamResponse, StreamRequest> *stream
) {
    StreamRequest request;
    while (stream->Read(&request)) {
        StreamResponse response;
        if (request.has_send_invitation_request()) {
            const SendInvitationRequest& send_request = request.send_invitation_request();
            if (database::invite_service_database_api::is_invite_already_sended(
                    send_request.sender_id(), send_request.receiver_id()
                )) {
                response.mutable_send_invitation_response()->set_message("Invitation already sent");
            } else {
                database::invite_service_database_api::add(
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
            if (database::invite_service_database_api::is_invitation_accepted(
                    respond_request.receiver_id()
                )) {
                response.mutable_respond_to_invitation_response()->set_message("Invitation already accepted");
            } else {
                database::invite_service_database_api::respond_to_invitation(
                    respond_request.receiver_id(), respond_request.accept()
                );
                if (respond_request.accept()) {
                    database::invite_service_database_api::create_chat(
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
            if (database::invite_service_database_api::is_invitation_checked(check_request.receiver_id())) {
                response.mutable_check_new_invitations_response()->set_message("Invitations already checked");
            } else {
                database::invite_service_database_api::check_new_invitations(check_request.receiver_id());
                response.mutable_check_new_invitations_response()->set_message("Invitations checked");
            }
        }
        stream->Write(response);
    }
    return Status::OK;
}*/


grpc::Status InvitationDataService::GetReceivedInvitations(
    grpc::ServerContext *context, 
    const invitation::RequestWithId *request, 
    invitation::InvitesReceived *response
) {
    auto received_invitions = database::invite_service_database_api::get_received_invites(request->id());
    received_invitions.convert_to_proto(response);
    return grpc::Status::OK;
}

grpc::Status InvitationDataService::GetSentInvitations(
    grpc::ServerContext *context, 
    const invitation::RequestWithId *request, 
    invitation::InvitesSent *response
) {
    auto sended_invitions = database::invite_service_database_api::get_sended_invites(request->id());
    sended_invitions.convert_to_proto(response);
    return grpc::Status::OK;
}