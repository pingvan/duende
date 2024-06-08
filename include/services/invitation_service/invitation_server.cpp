#include "invitation_server.hpp"
#include <unordered_map>

grpc::Status invitation_service::InvitationStream(
    grpc::ServerContext *context,
    grpc::ServerReaderWriter<
        invitation::InvitationResponse,
        invitation::InvitationRequest> *stream
) {
    invitation::InvitationRequest request;
    database::invite_service_database_api invite_db_api;
    database::chat_service_database_api chat_db_api;
    database::block_service_database_api block_db_api;
    while (stream->Read(&request)) {
        if (request.has_invite()) {
            bool is_group_invite = request.invite().has_chat_id();

            if (is_group_invite) {
                int sender_id = request.invite().sender_id();
                int receiver_id = request.invite().receiver_id();
                int chat_id = request.invite().chat_id();
                common::GroupChatPreview *chat =
                    get_group_chat_preview(chat_id);

                if (invite_db_api.is_invite_already_sent_into_the_group_chat(
                        chat_id, receiver_id
                    )) {
                    invitation::InvitationResponse sender_response;
                    const auto invite_sender_response =
                        new invitation::SendInvitationResponse;

                    invite_sender_response->set_error_message(
                        "This user is already invited to this chat"
                    );

                    sender_response.set_allocated_invite(invite_sender_response
                    );
                    stream->Write(sender_response);
                    continue;
                }

                if (block_db_api.check_is_blocked(receiver_id, sender_id)) {
                    invitation::InvitationResponse sender_response;
                    const auto invite_sender_response =
                        new invitation::SendInvitationResponse;

                    invite_sender_response->set_error_message(
                        "This user has blocked you so you can't send an "
                        "invitation to him"
                    );

                    sender_response.set_allocated_invite(invite_sender_response
                    );
                    stream->Write(sender_response);
                    continue;
                }

                if (block_db_api.check_is_blocked(sender_id, receiver_id)) {
                    invitation::InvitationResponse sender_response;
                    const auto invite_sender_response =
                        new invitation::SendInvitationResponse;

                    invite_sender_response->set_error_message(
                        "You should unblock this user to send an invitation to "
                        "him"
                    );

                    sender_response.set_allocated_invite(invite_sender_response
                    );
                    stream->Write(sender_response);
                    continue;
                }

                std::string invite_text = "Invitation";
                auto invite_data = invite_db_api.add_invite(
                    invite_text, sender_id, receiver_id, chat_id
                );
                int invite_id = invite_data.first;
                std::string invite_timestamp = invite_data.second;

                invitation::InvitationResponse sender_response;
                invitation::InvitationResponse receiver_response;

                const auto sender = new common::UserPreview;
                auto sender_preview =
                    database::authservice_database_api::get_user_preview(
                        sender_id
                    );
                auto sender_preview_proto = new common::UserPreview;
                sender_preview.convert_to_proto(sender_preview_proto);
                sender->CopyFrom(*sender_preview_proto);

                const auto receiver = new common::UserPreview;
                auto receiver_preview =
                    database::authservice_database_api::get_user_preview(
                        receiver_id
                    );
                auto receiver_preview_proto = new common::UserPreview;
                receiver_preview.convert_to_proto(receiver_preview_proto);
                receiver->CopyFrom(*receiver_preview_proto);

                const auto invite_sender_response =
                    new invitation::SendInvitationResponse;
                const auto invite_receiver_response =
                    new invitation::SendInvitationResponse;
                const auto sender_invite = new invitation::Invite;
                const auto receiver_invite = new invitation::Invite;
                sender_invite->set_allocated_sender(receiver);
                sender_invite->set_invite_id(invite_id);
                sender_invite->set_invite_text(invite_text);
                sender_invite->set_time_stamp(invite_timestamp);
                sender_invite->set_status(invitation::InviteStatus::SENT);
                sender_invite->mutable_chat_preview()->CopyFrom(*chat);

                receiver_invite->set_allocated_sender(sender);
                receiver_invite->set_invite_id(invite_id);
                receiver_invite->set_invite_text(invite_text);
                receiver_invite->set_time_stamp(invite_timestamp);
                receiver_invite->set_status(invitation::InviteStatus::SENT);
                receiver_invite->mutable_chat_preview()->CopyFrom(*chat);

                invite_sender_response->set_sender_id(sender_id);
                invite_sender_response->set_allocated_invite(sender_invite);
                invite_receiver_response->set_sender_id(sender_id);
                invite_receiver_response->set_allocated_invite(receiver_invite);

                sender_response.set_allocated_invite(invite_sender_response);
                receiver_response.set_allocated_invite(invite_receiver_response
                );

                if (users_online.count(receiver_id)) {
                    users_online[receiver_id]->Write(receiver_response);
                } else {
                    invite_db_api.add_unhandled_invite(receiver_id, invite_id);
                }

                stream->Write(sender_response);
            } else {
                int receiver_id = request.invite().receiver_id();
                int sender_id = request.invite().sender_id();

                if (invite_db_api.is_invite_already_sent(
                        sender_id, receiver_id
                    )) {
                    invitation::InvitationResponse sender_response;
                    const auto invite_sender_response =
                        new invitation::SendInvitationResponse;

                    invite_sender_response->set_error_message(
                        "You have already sent an invitation to this user"
                    );

                    sender_response.set_allocated_invite(invite_sender_response
                    );
                    stream->Write(sender_response);
                    continue;
                }

                if (invite_db_api.is_invite_already_sent(
                        receiver_id, sender_id
                    )) {
                    invitation::InvitationResponse sender_response;
                    const auto invite_sender_response =
                        new invitation::SendInvitationResponse;

                    invite_sender_response->set_error_message(
                        "This user has already sent you an invitation"
                    );

                    sender_response.set_allocated_invite(invite_sender_response
                    );
                    stream->Write(sender_response);
                    continue;
                }

                if (chat_db_api.check_is_private_chat_already_exists(
                        sender_id, receiver_id
                    )) {
                    invitation::InvitationResponse sender_response;
                    const auto invite_sender_response =
                        new invitation::SendInvitationResponse;

                    invite_sender_response->set_error_message(
                        "You already have a private chat with this user"
                    );

                    sender_response.set_allocated_invite(invite_sender_response
                    );
                    stream->Write(sender_response);
                    continue;
                }

                if (block_db_api.check_is_blocked(receiver_id, sender_id)) {
                    invitation::InvitationResponse sender_response;
                    const auto invite_sender_response =
                        new invitation::SendInvitationResponse;

                    invite_sender_response->set_error_message(
                        "This user has blocked you so you can't send an "
                        "invitation to him"
                    );

                    sender_response.set_allocated_invite(invite_sender_response
                    );
                    stream->Write(sender_response);
                    continue;
                }

                if (block_db_api.check_is_blocked(sender_id, receiver_id)) {
                    invitation::InvitationResponse sender_response;
                    const auto invite_sender_response =
                        new invitation::SendInvitationResponse;

                    invite_sender_response->set_error_message(
                        "You should unblock this user to send an invitation to "
                        "him"
                    );

                    sender_response.set_allocated_invite(invite_sender_response
                    );
                    stream->Write(sender_response);
                    continue;
                }

                std::string invite_text = "Invitation";
                auto invite_data = invite_db_api.add_invite(
                    invite_text, sender_id, receiver_id
                );
                int invite_id = invite_data.first;
                std::string invite_timestamp = invite_data.second;

                invitation::InvitationResponse sender_response;
                invitation::InvitationResponse receiver_response;

                const auto sender = new common::UserPreview;
                auto sender_preview =
                    database::authservice_database_api::get_user_preview(
                        sender_id
                    );
                auto sender_preview_proto = new common::UserPreview;
                sender_preview.convert_to_proto(sender_preview_proto);
                sender->CopyFrom(*sender_preview_proto);

                const auto receiver = new common::UserPreview;
                auto receiver_preview =
                    database::authservice_database_api::get_user_preview(
                        receiver_id
                    );
                auto receiver_preview_proto = new common::UserPreview;
                receiver_preview.convert_to_proto(receiver_preview_proto);
                receiver->CopyFrom(*receiver_preview_proto);

                const auto invite_sender_response =
                    new invitation::SendInvitationResponse;
                const auto invite_receiver_response =
                    new invitation::SendInvitationResponse;
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
                receiver_response.set_allocated_invite(invite_receiver_response
                );

                if (users_online.count(receiver_id)) {
                    users_online[receiver_id]->Write(receiver_response);
                } else {
                    invite_db_api.add_unhandled_invite(receiver_id, invite_id);
                }

                stream->Write(sender_response);
            }
        } else if (request.has_invite_respond()) {
            invitation::InvitationResponse sender_response;
            invitation::InvitationResponse receiver_response;
            const auto respond_sender_response =
                new invitation::RespondToInvitationResponse;
            const auto respond_receiver_response =
                new invitation::RespondToInvitationResponse;
            int invite_id = request.invite_respond().invite_id();
            int sender_id = request.invite_respond().sender_id();
            int receiver_id = request.invite_respond().receiver_id();
            bool is_group_chat = request.invite_respond().has_chat_id();
            auto status = request.invite_respond().status();

            if (status == invitation::InviteStatus::ACCEPTED) {
                invite_db_api.change_invite_status(invite_id);

                if (!is_group_chat) {
                    chat_service::ChatPreview *private_chat =
                        create_private_chat(sender_id, receiver_id);
                    respond_sender_response->mutable_chat()->CopyFrom(
                        *private_chat
                    );
                    respond_receiver_response->mutable_chat()->CopyFrom(
                        *private_chat
                    );
                    delete private_chat;
                } else {
                    chat_service::ChatPreview *group_chat =
                        get_chat_preview(request.invite_respond().chat_id());
                    respond_sender_response->mutable_chat()->CopyFrom(
                        *group_chat
                    );
                    respond_receiver_response->mutable_chat()->CopyFrom(
                        *group_chat
                    );

                    add_user_to_chat(sender_id, group_chat->chat_id());
                }

            } else {
                invite_db_api.change_invite_status(invite_id);
            }

            respond_sender_response->set_sender_id(sender_id);
            respond_sender_response->set_receiver_id(receiver_id);
            respond_sender_response->set_status(status);
            respond_sender_response->set_invite_id(invite_id);
            sender_response.set_allocated_invite_respond(respond_sender_response
            );

            respond_receiver_response->set_sender_id(sender_id);
            respond_receiver_response->set_receiver_id(receiver_id);
            respond_receiver_response->set_status(status);
            respond_receiver_response->set_invite_id(invite_id);
            receiver_response.set_allocated_invite_respond(
                respond_receiver_response
            );

            if (users_online.count(receiver_id)) {
                users_online[receiver_id]->Write(receiver_response);
            }

            stream->Write(sender_response);

        } else if (request.has_init()) {
            invitation::InvitationResponse response;
            users_online.insert(
                {request.init().user_id(),
                 std::make_shared<grpc::ServerReaderWriter<
                     invitation::InvitationResponse,
                     invitation::InvitationRequest>>(*stream)}
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

chat_service::ChatPreview *
invitation_service::create_private_chat(int first_id, int second_id) {
    grpc::ClientContext context;

    chat_service::CreatePrivateChatRequest request;
    request.set_firest_user_id(first_id);
    request.set_second_user_id(second_id);

    auto *response = new chat_service::ChatPreview;
    grpc::Status status =
        chat_stub->CreatePrivateChat(&context, request, response);

    return response;
}

chat_service::ChatPreview *invitation_service::get_chat_preview(int chat_id) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    request.set_chat_id(chat_id);

    auto *response = new chat_service::ChatPreview;
    grpc::Status status =
        chat_data_stub->GetChatPreview(&context, request, response);

    return response;
}

common::GroupChatPreview *invitation_service::get_group_chat_preview(int chat_id
) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    request.set_chat_id(chat_id);

    auto *response = new common::GroupChatPreview;
    grpc::Status status =
        chat_data_stub->GetGroupChatPreview(&context, request, response);

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
