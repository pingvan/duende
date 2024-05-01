#include "invitation_server.hpp"
#include <unordered_map>

grpc::Status InvitationServiceImpl::InvitationStream(
    grpc::ServerContext *context,
    grpc::ServerReaderWriter<invitation::InvitationResponse, invitation::InvitationRequest> *stream
) {
    invitation::InvitationRequest request;
    database::invite_service_database_api invite_db_api;
    while (stream->Read(&request)) {
        if (request.has_send_invitation_request()) {
            if (!invite_db_api.is_invite_already_sended(
                    request.send_invitation_request().sender_id(), request.send_invitation_request().receiver_id()
                )) {
                int sender_id = request.send_invitation_request().sender_id();
                int receiver_id = request.send_invitation_request().receiver_id();
                int invite_id = invite_db_api.add_invite(
                    "Invitation", invite_status::sent,
                    sender_id, receiver_id
                );

                invitation::InvitationResponse response;

                const auto send_response = new invitation::SendInvitationResponse;
                send_response->set_sender_id(sender_id);
                send_response->set_receiver_id(receiver_id);
                
                const auto invite = new invitation::Invite;
                invite->set_invite_id(invite_id);
                invite->set_invite_text("User " + std::to_string(sender_id) + " invites you to chat");
                invite->set_status(invitation::InviteStatus::SENT);
                send_response->set_allocated_invite(invite);

                response.set_allocated_send_invitation_response(send_response);
                
                if (users_online.count(receiver_id)) {
                    users_online[receiver_id]->Write(response);
                } else {
                    invite_db_api.add_unhandled_invite(
                        receiver_id, invite_id
                    );
                }
            }
        } else if (request.has_respond_to_invitation_request()) {
            invitation::InvitationResponse response;
            const auto respond_response = new invitation::RespondToInvitationResponse;
            int invite_id = request.respond_to_invitation_request().invite_id();
            int receiver_id = request.respond_to_invitation_request().receiver_id();
            auto status = request.respond_to_invitation_request().status();

            if (status == invitation::InviteStatus::ACCEPTED) {
                invite_db_api.change_invite_status(
                    invite_id, invite_status::accepted
                );

                respond_response->set_invite_id(invite_id);
                respond_response->set_receiver_id(receiver_id);
                respond_response->set_status(status);
                response.set_allocated_respond_to_invitation_response(respond_response);

                if (users_online.count(receiver_id)) {
                    users_online[receiver_id]->Write(response);
                }

            } else {
                invite_db_api.change_invite_status(
                    invite_id, invite_status::declined
                );
            }
        } else if (request.has_init_request()) {
            invitation::InvitationResponse response;
            users_online.insert(
                {request.init_request().user_id(),
                 std::make_shared<grpc::ServerReaderWriter<
                     invitation::InvitationResponse, invitation::InvitationRequest>>(*stream)}
            );
            const auto init_response = new invitation::InitResponse;
            init_response->set_status(invitation::Status::SUCCESS);
            response.set_allocated_init_response(init_response);
            stream->Write(response);
        } else if (request.has_leave_request()) {
            invitation::InvitationResponse response;
            users_online.erase(request.leave_request().user_id());
            const auto disconnect_response = new invitation::LeaveResponse;
            disconnect_response->set_status(invitation::Status::SUCCESS);
            response.set_allocated_leave_response(disconnect_response);
            stream->Write(response);
        }
    }
    return grpc::Status::OK;
}

grpc::Status invitation_service_data::GetReceivedInvitations(
    grpc::ServerContext *context,
    const invitation::RequestWithId *request,
    invitation::InvitesReceived *response
) {
    database::invite_service_database_api invite_db_api;
    auto invites = invite_db_api.get_received_invites(request->id()).invites;
    for (auto &invite : invites) {
        auto *new_invite = response->add_invites();
        invite.convert_to_proto(new_invite);
    }
    return grpc::Status::OK;
}

grpc::Status invitation_service_data::GetSentInvitations(
    grpc::ServerContext *context,
    const invitation::RequestWithId *request,
    invitation::InvitesSent *response
) {
    database::invite_service_database_api invite_db_api;
    auto invites = invite_db_api.get_sended_invites(request->id()).invites;
    for (auto &invite : invites) {
        auto *new_invite = response->add_invites();
        invite.convert_to_proto(new_invite);
    }
    return grpc::Status::OK;
}