// #include "invitation_server.hpp"
// #include "components/database/database-connector.hpp"

// Status InvitationServiceImpl::SendInvitation(ServerContext* context, const InvitationRequest* request, InvitationReply* reply) {
//     InvitationRequest current_invitation = *request;
//     database::connector db_connector;
//     if (db_connector.is_invitation_sent(current_invitation.sender_id(), current_invitation.receiver_id())) {
//         reply->set_message("Invitation already sent");
//         return Status::OK;
//     }
//     db_connector.send_invitation(current_invitation.sender_id(), current_invitation.receiver_id());
//     reply->set_message("Invitation sent");
//     return Status::OK;
// }

// Status InvitationServiceImpl::RespondToInvitation(ServerContext* context, const InvitationResponse* request, InvitationReply* reply) {
//     InvitationResponse current_invitation = *request;
//     database::connector db_connector;
//     if (db_connector.is_invitation_accepted(current_invitation.sender_id(), current_invitation.receiver_id())) {
//         reply->set_message("Invitation already accepted");
//         return Status::OK;
//     }
//     db_connector.respond_to_invitation(current_invitation.sender_id(), current_invitation.receiver_id(), current_invitation.accepted());
//     reply->set_message("Invitation responded");
//     return Status::OK;
// }

// Status InvitationServiceImpl::CheckNewInvitations(ServerContext* context, const CheckRequest* request, CheckReply* reply) {
//     CheckRequest current_invitation = *request;
//     database::connector db_connector;
//     if (db_connector.is_invitation_checked(current_invitation.receiver_id())) {
//         reply->set_message("Invitations already checked");
//         return Status::OK;
//     }
//     db_connector.check_new_invitations(current_invitation.receiver_id());
//     reply->set_message("Invitations checked");
//     return Status::OK;
// }