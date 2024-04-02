#include "invitation_server.hpp"
#include "components/database/database-connector.hpp"

Status InvitationServiceImpl::SendInvitation(
    ServerContext *context,
    ServerReader<InvitationRequest> *reader,
    ServerWriter<InvitationReply> *writer
) {
    InvitationRequest request;
    database::connector db_connector;
    while (reader->Read(&request)) {
        InvitationReply reply;
        if (db_connector.is_invitation_sent(
                request.sender_id(), request.receiver_id()
            )) {
            reply.set_message("Invitation already sent");
        } else {
            db_connector.send_invitation(
                request.sender_id(), request.receiver_id()
            );
            reply.set_message("Invitation sent");
        }
        writer->Write(reply);
    }
    return Status::OK;
}

Status InvitationServiceImpl::RespondToInvitation(
    ServerContext *context,
    ServerReader<InvitationResponse> *reader,
    ServerWriter<InvitationReply> *writer
) {
    InvitationResponse response;
    database::connector db_connector;
    while (reader->Read(&response)) {
        InvitationReply reply;
        if (db_connector.is_invitation_accepted(
                response.sender_id(), response.receiver_id()
            )) {
            reply.set_message("Invitation already accepted");
        } else {
            db_connector.respond_to_invitation(
                response.sender_id(), response.receiver_id(),
                response.accepted()
            );
            if (response.accepted()) {
                db_connector.create_chat(
                    response.sender_id(), response.receiver_id()
                );
                reply.set_message("Invitation accepted and chat created");
            } else {
                reply.set_message("Invitation declined");
            }
        }
        writer->Write(reply);
    }
    return Status::OK;
}

Status InvitationServiceImpl::CheckNewInvitations(
    ServerContext *context,
    ServerReader<CheckRequest> *reader,
    ServerWriter<CheckReply> *writer
) {
    CheckRequest request;
    database::connector db_connector;
    while (reader->Read(&request)) {
        CheckReply reply;
        if (db_connector.is_invitation_checked(request.receiver_id())) {
            reply.set_message("Invitations already checked");
        } else {
            db_connector.check_new_invitations(request.receiver_id());
            reply.set_message("Invitations checked");
        }
        writer->Write(reply);
    }
    return Status::OK;
}