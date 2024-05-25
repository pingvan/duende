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
                    request.invite().sender_id(), request.invite().invite().sender().user_id()
                )) {
                int sender_id = request.invite().sender_id();
                int receiver_id = request.invite().invite().sender().user_id();
                std::string invite_text = request.invite().invite().invite_text();
                auto invite_data = invite_db_api.add_invite(
                    invite_text, sender_id, receiver_id
                );
                int invite_id = invite_data.first;
                std::string invite_timestamp = invite_data.second;
                int chat_id = request.invite().invite().chat_id();

                invitation::InvitationResponse sender_response;
                invitation::InvitationResponse receiver_response;

                const auto sender = new common::UserPreview;
                const auto receiver = new common::UserPreview;
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
                sender_invite->set_chat_id(chat_id);

                receiver_invite->set_allocated_sender(sender);
                receiver_invite->set_invite_id(invite_id);
                receiver_invite->set_invite_text(invite_text);
                receiver_invite->set_time_stamp(invite_timestamp);
                receiver_invite->set_status(invitation::InviteStatus::SENT);
                receiver_invite->set_chat_id(chat_id);

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
            invitation::InvitationResponse sender_response;
            invitation::InvitationResponse receiver_response;
            const auto respond_sender_response = new invitation::RespondToInvitationResponse;
            const auto respond_receiver_response = new invitation::RespondToInvitationResponse;
            int invite_id = request.invite_respond().invite().invite_id();
            int sender_id = request.invite_respond().sender_id();
            int receiver_id = request.invite_respond().invite().sender().user_id();
            int chat_id = request.invite_respond().invite().chat_id();
            auto status = request.invite_respond().invite().status();

            if (status == invitation::InviteStatus::ACCEPTED) {
                invite_db_api.change_invite_status(invite_id);

                if (chat_id == -1) { 
                    chat_service::ChatPreview *private_chat = create_private_chat(sender_id, receiver_id);
                    respond_sender_response->mutable_chat()->CopyFrom(*private_chat);
                    respond_receiver_response->mutable_chat()->CopyFrom(*private_chat);
                    delete private_chat;
                }
                else {
                    // chat = get_chat_preview(chat_id); // TODO: Implement this function
                    const auto group_chat = new chat_service::ChatPreview;
                    respond_sender_response->mutable_chat()->CopyFrom(*group_chat);
                    respond_receiver_response->mutable_chat()->CopyFrom(*group_chat);
                    delete group_chat;

                    add_user_to_chat(sender_id, chat_id);
                }
                            
            } else {
                invite_db_api.change_invite_status(invite_id);
            }

            const auto sender = new common::UserPreview;
            const auto receiver = new common::UserPreview;
            sender->set_user_id(sender_id);
            receiver->set_user_id(receiver_id);

            const auto sender_invite = new invitation::Invite;
            const auto receiver_invite = new invitation::Invite;

            sender_invite->set_invite_id(invite_id);
            sender_invite->set_status(status);
            sender_invite->set_chat_id(chat_id);
            sender_invite->set_allocated_sender(receiver);

            receiver_invite->set_invite_id(invite_id);
            receiver_invite->set_status(status);
            receiver_invite->set_chat_id(chat_id);
            receiver_invite->set_allocated_sender(sender);

            respond_sender_response->set_sender_id(sender_id);
            respond_sender_response->set_allocated_invite(sender_invite);
            sender_response.set_allocated_invite_respond(respond_sender_response);

            respond_receiver_response->set_sender_id(sender_id);
            respond_receiver_response->set_allocated_invite(receiver_invite);
            receiver_response.set_allocated_invite_respond(respond_receiver_response);

            if (users_online.count(receiver_id)) {
                users_online[receiver_id]->Write(receiver_response);
            }

            stream->Write(sender_response);

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

grpc::Status invitation_service::add_user_to_chat(int user_id, int chat_id) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    request.set_user_id(user_id);
    request.set_chat_id(chat_id);

    auto *response = new chat_service::OperationStatusResponse;
    grpc::Status status = chat_stub->AddUserToChat(&context, request, response);

    return status;
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
