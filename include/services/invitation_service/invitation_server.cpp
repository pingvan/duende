#include "invitation_server.hpp"
#include <unordered_map>

grpc::Status invitation_service::InvitationStream(
    grpc::ServerContext *context,
    grpc::ServerReaderWriter<invitation::InvitationResponse, invitation::InvitationRequest> *stream
) {
    invitation::InvitationRequest request;
    database::invite_service_database_api invite_db_api;
    while (stream->Read(&request)) {
        if (request.has_invite()) {
            if (!invite_db_api.is_invite_already_sended(
                    request.invite().sender().user_id(), request.invite().invite().sender().user_id()
                )) {
                int sender_id = request.invite().sender().user_id();
                int receiver_id = request.invite().invite().sender().user_id();
                std::string sender_login = request.invite().sender().login();
                std::string receiver_login = request.invite().invite().sender().login();
                std::string invite_text = request.invite().invite().invite_text();
                auto invite_data = invite_db_api.add_invite(
                    invite_text, sender_id, receiver_id
                );
                int invite_id = invite_data.first;
                std::string invite_timestamp = invite_data.second;

                invitation::InvitationResponse sender_response;
                invitation::InvitationResponse receiver_response;

                const auto sender = new common::UserPreview;
                const auto receiver = new common::UserPreview;
                sender->set_login(sender_login);
                receiver->set_login(receiver_login);
                sender->set_user_id(sender_id);
                receiver->set_user_id(receiver_id);

                const auto invite_sender_response = new invitation::SendInvitationResponse;
                const auto invite_receiver_response = new invitation::SendInvitationResponse;
                const auto sender_invite = new invitation::Invite;
                const auto receiver_invite = new invitation::Invite;
                sender_invite->set_allocated_sender(receiver);
                sender_invite->set_invite_id(invite_id);
                sender_invite->set_invite_text(invite_text);
                sender_invite->set_time_stamp(invite_timestamp);
                sender_invite->set_status(invitation::InviteStatus::SENT);

                receiver_invite->set_allocated_sender(sender);
                receiver_invite->set_invite_id(invite_id);
                receiver_invite->set_invite_text(invite_text);
                receiver_invite->set_time_stamp(invite_timestamp);
                receiver_invite->set_status(invitation::InviteStatus::SENT);

                invite_sender_response->set_sender_id(sender_id);
                invite_sender_response->set_allocated_invite(sender_invite);
                invite_receiver_response->set_sender_id(sender_id);
                invite_receiver_response->set_allocated_invite(receiver_invite);

                sender_response.set_allocated_invite(invite_sender_response);
                receiver_response.set_allocated_invite(invite_receiver_response);
                
                if (users_online.count(receiver_id)) {
                    users_online[receiver_id]->Write(receiver_response);
                } else {
                    invite_db_api.add_unhandled_invite(
                        receiver_id, invite_id
                    );
                }

                stream->Write(sender_response);
            }
        } else if (request.has_invite_respond()) {
            invitation::InvitationResponse response;
            const auto respond_response = new invitation::RespondToInvitationResponse;
            int invite_id = request.invite_respond().invite_id();
            int sender_id = request.invite_respond().sender_id();
            int receiver_id = request.invite_respond().receiver_id();
            auto status = request.invite_respond().status();

            if (status == invitation::InviteStatus::ACCEPTED) {
                invite_db_api.change_invite_status(invite_id);

                respond_response->set_allocated_chat(create_private_chat(sender_id, receiver_id));
            } else {
                invite_db_api.change_invite_status(invite_id);
            }

            respond_response->set_invite_id(invite_id);
            respond_response->set_receiver_id(receiver_id);
            respond_response->set_status(status);
            response.set_allocated_invite_respond(respond_response);

            if (users_online.count(receiver_id)) {
                users_online[receiver_id]->Write(response);
            }

            stream->Write(response);
        } else if (request.has_init()) {
            invitation::InvitationResponse response;
            users_online.insert(
                {request.init().user_id(),
                 std::make_shared<grpc::ServerReaderWriter<
                     invitation::InvitationResponse, invitation::InvitationRequest>>(*stream)}
            );
            const auto init_response = new invitation::InitResponse;
            init_response->set_status(invitation::Status::SUCCESS);
            response.set_allocated_init(init_response);
            stream->Write(response);
        } else if (request.has_leave()) {
            invitation::InvitationResponse response;
            users_online.erase(request.leave().user_id());
            const auto disconnect_response = new invitation::LeaveResponse;
            disconnect_response->set_status(invitation::Status::SUCCESS);
            response.set_allocated_leave(disconnect_response);
            stream->Write(response);
        }
    }
    return grpc::Status::OK;
}

chat_service::ChatPreview *invitation_service::create_private_chat(int first_id, int second_id) {
    grpc::ClientContext context;

    chat_service::CreatePrivateChatRequest request;
    request.set_firest_user_id(first_id);
    request.set_second_user_id(second_id);

    auto *response = new chat_service::ChatPreview;
    grpc::Status status = chat_stub->CreatePrivateChat(&context, request, response);

    return response;
}

grpc::Status invitation_data_service::GetReceivedInvitations(
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

grpc::Status invitation_data_service::GetSentInvitations(
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
