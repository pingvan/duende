#include "invitation_server.hpp"
#include "components/database/database-connector.hpp"

Status InvitationServiceImpl::SendInvitation(ServerContext* context, const InvitationRequest* request, ServerWriter<InvitationReply>* writer) {
    InvitationRequest current_invitation = *request;
    database::connector db_connector;
    InvitationReply reply;
    if (db_connector.is_invitation_sent(current_invitation.sender_id(), current_invitation.receiver_id())) {
        reply.set_message("Invitation already sent");
        writer->Write(reply);
        return Status::OK;
    }
    db_connector.send_invitation(current_invitation.sender_id(), current_invitation.receiver_id());
    reply.set_message("Invitation sent");
    writer->Write(reply);
    return Status::OK;
}

Status InvitationServiceImpl::RespondToInvitation(ServerContext* context, const InvitationResponse* request, ServerWriter<InvitationReply>* writer) {
    InvitationResponse current_invitation = *request;
    database::connector db_connector;
    InvitationReply reply;
    if (db_connector.is_invitation_accepted(current_invitation.sender_id(), current_invitation.receiver_id())) {
        reply.set_message("Invitation already accepted");
        writer->Write(reply);
        return Status::OK;
    }
    db_connector.respond_to_invitation(current_invitation.sender_id(), current_invitation.receiver_id(), current_invitation.accepted());
    if (current_invitation.accepted()) {
        db_connector.create_chat(current_invitation.sender_id(), current_invitation.receiver_id());
        reply.set_message("Invitation accepted and chat created");
    } else {
        reply.set_message("Invitation declined");
    }
    writer->Write(reply);
    return Status::OK;
}

Status InvitationServiceImpl::CheckNewInvitations(ServerContext* context, const CheckRequest* request, ServerWriter<CheckReply>* writer) {
    CheckRequest current_invitation = *request;
    database::connector db_connector;
    CheckReply reply;
    if (db_connector.is_invitation_checked(current_invitation.receiver_id())) {
        reply.set_message("Invitations already checked");
        writer->Write(reply);
        return Status::OK;
    }
    db_connector.check_new_invitations(current_invitation.receiver_id());
    reply.set_message("Invitations checked");
    writer->Write(reply);
    return Status::OK;
}