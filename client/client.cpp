#include "client.hpp"

client::client(const std::shared_ptr<grpc::Channel> &new_channel)
    : channel(new_channel),
      authorization_stub(authservice::AuthService::NewStub(new_channel)),
      authorization_data_stub(authservice::AuthServiceData::NewStub(new_channel)
      ),
      chat_stub(chat_service::ChatHandler::NewStub(new_channel)),
      chat_data_stub(chat_service::ChatDataService::NewStub(new_channel)),
      profile_stub(profile::ProfileService::NewStub(new_channel)),
      profile_data_stub(profile::ProfileServiceData::NewStub(new_channel)),
      search_stub(search_service::SerachService::NewStub(new_channel)),
      invite_stub(invitation::InvitationService::NewStub(new_channel)),
      invite_data_stub(invitation::InvitationDataService::NewStub(new_channel)),
      users_interaction_stub(
          users_interaction::UsersInteractionService::NewStub(new_channel)
      ),
      recommendation_stub(
          recommendation_service::RecommendationService::NewStub(new_channel)
      ) {
}

void client::init_invite_chat_stream(int user_id) {
    invite_chat_stream = chat_stub->InvitesToChat(&invite_chat_context);

    chat_service::InvitesToChatRequests request;
    auto *init = new chat_service::InitialRequest;
    init->set_user_id(user_id);
    request.set_allocated_init(init);
    invite_chat_stream->Write(request);

    start_reading_invite_chat_stream();
}

void client::start_reading_invite_chat_stream() {
    qDebug() << "start_reading_invite_chat_stream started";
    QFuture<void> future =
        QtConcurrent::run([this] {
            qDebug() << "QtConcurrent invite chat stream running";
            chat_service::InvitesToChatResponses response;
            while (invite_chat_stream->Read(&response)) {
                qDebug() << "Invite chat response received";
                if (response.has_remove()) {
                    if (onRemoveFromChatReceived) {
                        onRemoveFromChatReceived(
                            response.remove()
                        );
                    }
                } else if (response.has_response_with_status()) {
                    qDebug()
                        << (response.response_with_status().status() ==
                            chat_service::Status::SUCCESS);
                }
            }
            qDebug() << "Invite chat stream ended";
        });
}

void client::leave_invite_chat_stream(int user_id) {
    chat_service::InvitesToChatRequests request;
    auto *leave = new chat_service::DisconnectRequest;
    leave->set_user_id(user_id);
    request.set_allocated_leave(leave);
    invite_chat_stream->Write(request);
}

chat_service::ChatPreview *client::create_group_chat(int creator_id, std::string chat_name, std::string description) {
    grpc::ClientContext context;

    chat_service::CreateGroupChatRequest request;
    auto *response = new chat_service::ChatPreview;
    request.set_creator_id(creator_id);
    request.set_chat_name(chat_name);
    request.set_description(description);

    grpc::Status status = chat_stub->CreateGroupChat(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }

    return response;
}

bool client::check_user_privileges(int user_id, int chat_id) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    auto *response = new chat_service::UserPrivilegeResponse;
    request.set_user_id(user_id);
    request.set_chat_id(chat_id);

    grpc::Status status = chat_stub->CheckUserPrivileges(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }
    return (response->privilege() == chat_service::UserPrivilege::ADMIN);
}

bool client::change_chat_name(int chat_id, int admin_id, std::string new_chat_name) {
    if (!check_user_privileges(admin_id, chat_id)) {
        return false;
    }

    grpc::ClientContext context;

    chat_service::ChangeChatNameRequest request;
    auto *response = new chat_service::OperationStatusResponse;
    request.set_chat_id(chat_id);
    request.set_admin_id(admin_id);
    request.set_new_chat_name(new_chat_name);

    grpc::Status status = chat_stub->ChangeChatName(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }

    return true;
}

bool client::change_description(int chat_id, int admin_id, std::string new_description) {
    if (!check_user_privileges(admin_id, chat_id)) {
        return false;
    }

    grpc::ClientContext context;

    chat_service::ChangeChatDescriptionRequest request;
    auto *response = new chat_service::OperationStatusResponse;
    request.set_chat_id(chat_id);
    request.set_admin_id(admin_id);
    request.set_new_description(new_description);

    grpc::Status status = chat_stub->ChangeChatDescription(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }

    return true;
}

void client::change_chat_cover(uint32_t chat_id, uint32_t admin_id, const std::string &photo_path) {
    if (!check_user_privileges(admin_id, chat_id)) {
        return;
    }
    chat_service::ChangeChatCoverRequest request;

    std::ifstream file(photo_path, std::ios::binary);
    if (!file) {
        std::cout << "Cannot open file: " << photo_path << std::endl;
        return;
    }
    std::basic_string photo_data(
        (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
    );
    request.set_chat_id(chat_id);
    request.set_admin_id(admin_id);
    request.set_new_cover(photo_data);

    chat_service::OperationStatusResponse response;
    grpc::ClientContext context;
    grpc::Status status =
        chat_stub->ChangeChatCover(&context, request, &response);
        
    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return;
    }
}

void client::add_privileges_to_user(int sender_id, int receiver_id, int chat_id) {
    if (!check_user_privileges(sender_id, chat_id)) {
        return; // TODO add errors
    }

    grpc::ClientContext context;

    chat_service::UserChat request;
    auto *response = new chat_service::OperationStatusResponse;
    request.set_user_id(receiver_id);
    request.set_chat_id(chat_id);

    grpc::Status status = chat_stub->AddPrivilegesToUser(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }
}

void client::remove_privileges_from_user(int sender_id, int receiver_id, int chat_id) {
    if (!check_user_privileges(sender_id, chat_id)) {
        return; // TODO add errors
    }

    grpc::ClientContext context;

    chat_service::UserChat request;
    auto *response = new chat_service::OperationStatusResponse;
    request.set_user_id(receiver_id);
    request.set_chat_id(chat_id);

    grpc::Status status = chat_stub->RemovePrivilegesFromUser(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }
}

void client::remove_user_from_chat(int sender_id, int receiver_id, int chat_id) {
    if (!check_user_privileges(sender_id, chat_id) && sender_id != receiver_id) {
        return; // TODO add errors
    }

    chat_service::RemoveRequest request;
    request.set_sender_id(sender_id);
    request.set_receiver_id(receiver_id);
    request.set_chat_id(chat_id);

    chat_service::InvitesToChatRequests inviteChatRequest;
    inviteChatRequest.mutable_remove()->CopyFrom(request);

    std::cout << inviteChatRequest.DebugString() << std::endl;
    bool status = invite_chat_stream->Write(inviteChatRequest);
    if (!status) {
        std::cerr << "Error" << std::endl;
    }
}

void client::init_invite_stream(int user_id) {
    invite_stream = invite_stub->InvitationStream(&invite_context);

    invitation::InvitationRequest request;
    auto *init = new invitation::InitRequest;
    init->set_user_id(user_id);
    request.set_allocated_init(init);
    invite_stream->Write(request);

    start_reading_invite_stream();
}

void client::start_reading_invite_stream() {
    qDebug() << "start_reading_invite_stream started";
    QFuture<void> future =
        QtConcurrent::run([this] {
            qDebug() << "QtConcurrent invite stream running";
            invitation::InvitationResponse response;
            while (invite_stream->Read(&response)) {
                qDebug() << "Invite response received";
                if (response.has_invite()) {
                    if (onNewInviteReceived) {
                        onNewInviteReceived(response.invite()
                        );
                    }
                }
                if (response.has_invite_respond()) {
                    if (onInviteRespondReceived) {
                        onInviteRespondReceived(
                            response.invite_respond()
                        );
                    }
                }
                if (response.has_init()) {
                    qDebug()
                        << (response.init().status() ==
                            invitation::Status::SUCCESS);
                }
                if (response.has_leave()) {
                    qDebug()
                        << (response.leave().status() ==
                            invitation::Status::SUCCESS);
                }
            }
            qDebug() << "Invite stream ended";
        });
}

void client::leave_invite_stream(int user_id) {
    invitation::InvitationRequest request;
    auto *leave = new invitation::LeaveRequest;
    leave->set_user_id(user_id);
    request.set_allocated_leave(leave);
    invite_stream->Write(request);
}

void client::send_invite(
    int sender_id,
    int receiver_id,
    int chat_id
) {
    invitation::SendInvitationRequest request;

    request.set_sender_id(sender_id);
    request.set_receiver_id(receiver_id);
    
    if (chat_id != -1) {
        request.set_chat_id(chat_id);
    }

    std::cout << request.DebugString() << std::endl;

    invitation::InvitationRequest stream_request;
    stream_request.mutable_invite()->CopyFrom(request);

    assert(invite_stream != nullptr);

    bool status = invite_stream->Write(stream_request);
    if (!status) {
        std::cerr << "Error" << std::endl;
    }
}

void client::respond_invite(
    int invite_id,
    int sender_id,
    int receiver_id,
    bool accept,
    int chat_id
) {
    invitation::RespondToInvitationRequest request;
    request.set_sender_id(sender_id);
    request.set_receiver_id(receiver_id);
    request.set_invite_id(invite_id);
    if (accept) {
        request.set_status(invitation::InviteStatus::ACCEPTED);
    }
    else {
        request.set_status(invitation::InviteStatus::DECLINED);
    }
    if (chat_id != -1) {
        request.set_chat_id(chat_id);
    }
    
    std::cout << request.DebugString() << std::endl;

    invitation::InvitationRequest stream_request;
    stream_request.mutable_invite_respond()->CopyFrom(request);

    assert(invite_stream != nullptr);

    bool status = invite_stream->Write(stream_request);
    if (!status) {
        std::cerr << "Error" << std::endl;
    }
}

std::vector<invitation::Invite> client::get_sent_invites(int user_id) {
    grpc::ClientContext context;

    invitation::RequestWithId request;
    auto *response = new invitation::InvitesSent;
    request.set_id(user_id);
    grpc::Status status =
        invite_data_stub->GetSentInvitations(&context, request, response);
    if (!status.ok()) {
        return {};
    }

    std::vector<invitation::Invite> invites;
    for (int i = 0; i < response->invites_size(); i++) {
        invites.push_back(response->invites(i));
    }
    return invites;
}

std::vector<invitation::Invite> client::get_received_invites(int user_id) {
    grpc::ClientContext context;

    invitation::RequestWithId request;
    auto *response = new invitation::InvitesReceived;
    request.set_id(user_id);
    grpc::Status status =
        invite_data_stub->GetReceivedInvitations(&context, request, response);
    if (!status.ok()) {
        return {};
    }

    std::vector<invitation::Invite> invites;
    for (int i = 0; i < response->invites_size(); i++) {
        invites.push_back(response->invites(i));
    }
    return invites;
}

void client::init_chat_stream(int user_id) {
    chat_stream = chat_stub->HandleChat(&chat_context);

    chat_service::ChatRequests request;
    auto *init = new chat_service::InitialRequest;
    init->set_user_id(user_id);
    request.set_allocated_init(init);
    chat_stream->Write(request);

    start_reading_chat_stream();
}

void client::start_reading_chat_stream() {
    qDebug() << "start_reading_chat_stream started";
    QFuture<void> future =
        QtConcurrent::run([this] {
            qDebug() << "QtConcurrent chat stream running";
            chat_service::ChatResponses response;
            while (chat_stream->Read(&response)) {
                qDebug() << "Chat response received";
                if (response.has_accept_message()) {
                    auto message = response.accept_message().msg();
                    if (onNewMessageReceived) {
                        onNewMessageReceived(message);
                    }
                } else if (response.has_response_with_status()) {
                    qDebug()
                        << (response.response_with_status().status() ==
                            chat_service::Status::SUCCESS);
                }
            }
            qDebug() << "Chat stream ended";
        });
}

void client::leave_chat_stream(int user_id) {
    chat_service::ChatRequests request;
    auto *leave = new chat_service::DisconnectRequest;
    leave->set_user_id(user_id);
    request.set_allocated_leave(leave);
    chat_stream->Write(request);
}

void client::send_message(
    int user_id,
    int chatId,
    const std::string &messageText
) {
    chat_service::SendMessageRequest request;
    auto *message = new chat_service::MessageSend();
    message->set_sender_id(user_id);
    message->set_chat_id(chatId);
    message->set_message_text(messageText);
    request.mutable_msg()->CopyFrom(*message);

    chat_service::ChatRequests chatRequest;
    chatRequest.mutable_send_message()->CopyFrom(request);

    std::cout << chatRequest.DebugString() << std::endl;
    bool status = chat_stream->Write(chatRequest);
    if (!status) {
        std::cerr << "Error" << std::endl;
    }
}

search_service::UserSearchResponse *client::search_user_by_credentials(
    const std::string &user_credentials
) {
    grpc::ClientContext context;

    search_service::UserSearchRequest request;
    auto *response = new search_service::UserSearchResponse;
    request.set_user_credentials(user_credentials);
    grpc::Status status =
        search_stub->SearchUserByCredentials(&context, request, response);
    return response;
}

search_service::ActorSearchResponse *client::search_actor_by_name(
    const std::string &actor_name
) {
    grpc::ClientContext context;

    search_service::ActorSearchRequest request;
    auto *response = new search_service::ActorSearchResponse;
    request.set_actor_name(actor_name);
    grpc::Status status =
        search_stub->SearchActorByName(&context, request, response);
    return response;
}

search_service::FilmSearchResponse *client::search_film_by_title(
    const std::string &film_title
) {
    grpc::ClientContext context;

    search_service::FilmSearchRequest request;
    auto *response = new search_service::FilmSearchResponse;
    request.set_film_title(film_title);
    grpc::Status status =
        search_stub->SearchFilmByTitle(&context, request, response);
    return response;
}

common::UserFullPage *client::get_user_full_page(int user_id) {
    grpc::ClientContext context;

    authservice::RequestWithUserId request;
    auto *response = new common::UserFullPage;

    request.set_user_id(user_id);
    grpc::Status status =
        authorization_data_stub->GetUserFullPage(&context, request, response);
    if (!status.ok()) {
        return {};
    }
    return response;
}

common::UserPreview *client::get_user_preview(int user_id) {
    grpc::ClientContext context;

    authservice::RequestWithUserId request;
    auto *response = new common::UserPreview;

    request.set_user_id(user_id);
    grpc::Status status =
        authorization_data_stub->GetUserPrewiew(&context, request, response);
    if (!status.ok()) {
        return {};
    }
    return response;
}

void client::add_genre(int user_id, const std::string &genre_name) {
    grpc::ClientContext context;

    profile::GenreRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Genre *genre = request.add_genres();
    genre->set_name(genre_name);
    grpc::Status status =
        profile_stub->AddToListOfGenres(&context, request, response);
}

void client::remove_genre(int user_id, const std::string &genre_name) {
    grpc::ClientContext context;

    profile::GenreRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Genre *genre = request.add_genres();
    genre->set_name(genre_name);
    qDebug() << "Sending remove genre request to server: "
             << genre_name.c_str();
    grpc::Status status =
        profile_stub->RemoveFromListOfGenres(&context, request, response);
    if (!status.ok()) {
        qDebug() << status.error_message().c_str();
    }
}

void client::add_movie(int user_id, int movie_id, int rating) {
    grpc::ClientContext context;

    profile::MovieRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Movie *movie = request.add_movies();
    movie->set_id(movie_id);
    movie->set_rating(rating);
    grpc::Status status =
        profile_stub->AddToWatchlist(&context, request, response);
}

void client::remove_movie(int user_id, int movie_id, int rating) {
    grpc::ClientContext context;

    profile::MovieRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Movie *movie = request.add_movies();
    movie->set_id(movie_id);
    movie->set_rating(rating);
    grpc::Status status =
        profile_stub->RemoveFromWatchlist(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << std::endl;
    }
}

void client::add_actor(int user_id, int actor_id) {
    grpc::ClientContext context;

    profile::ActorRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Actor *actor = request.add_actors();
    actor->set_id(actor_id);
    grpc::Status status =
        profile_stub->AddToListOfActors(&context, request, response);
}

void client::remove_actor(int user_id, int actor_id) {
    grpc::ClientContext context;

    profile::ActorRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Actor *actor = request.add_actors();
    actor->set_id(actor_id);
    grpc::Status status =
        profile_stub->RemoveFromListOfActors(&context, request, response);
    if (!status.ok()){
        std::cout << status.error_message() << '\n';
    }
}

void client::change_bio(int user_id, const std::string &new_bio) {
    grpc::ClientContext context;

    profile::BioRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    request.set_bio(new_bio);
    grpc::Status status = profile_stub->ChangeBio(&context, request, response);
}

void client::change_quote(int user_id, const std::string &new_quote) {
    grpc::ClientContext context;

    profile::QuoteRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    request.set_quote(new_quote);
    grpc::Status status =
        profile_stub->ChangeQuote(&context, request, response);
}

void client::change_username(int user_id, const std::string &new_username) {
    grpc::ClientContext context;

    profile::UsernameRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    request.set_username(new_username);
    grpc::Status status =
        profile_stub->ChangeUsername(&context, request, response);
}

std::vector<chat_service::ChatPreview> client::get_user_chats(int user_id) {
    grpc::ClientContext context;

    chat_service::GetChatsRequest request;
    auto *response = new chat_service::UserChats;
    request.set_user_id(user_id);

    std::cout << "GetUserChats called from client\n";
    grpc::Status status =
        chat_data_stub->GetUserChats(&context, request, response);
    if (!status.ok()) {
        return {};
    }

    std::vector<chat_service::ChatPreview> chats;
    for (int i = 0; i < response->user_chats_size(); i++) {
        chats.push_back(response->user_chats(i));
    }
    return chats;
}

std::vector<chat_service::MessageReceived>
client::get_last_messages(int user_id, int chat_id) {
    grpc::ClientContext context;

    chat_service::GetChatMessagesRequest request;
    auto *response = new chat_service::LastMessages;
    request.set_user_id(user_id);
    request.set_chat_id(chat_id);
    grpc::Status status =
        chat_data_stub->GetLastMessages(&context, request, response);
    if (!status.ok()) {
        return {};
    }

    std::vector<chat_service::MessageReceived> messages;
    for (int i = 0; i < response->messages_size(); i++) {
        messages.push_back(response->messages(i));
    }
    return messages;
}

std::vector<common::UserPreview> client::get_chat_members(int chat_id) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    auto *response = new chat_service::GetChatMembersResponse;
    request.set_chat_id(chat_id);
    grpc::Status status =
        chat_data_stub->GetChatMembers(&context, request, response);
    if (!status.ok()) {
        return {};
    }

    std::vector<common::UserPreview> members;
    for (int i = 0; i < response->members_size(); i++) {
        members.push_back(response->members(i));
    }
    return members;
}

bool client::check_user_in_chat(int user_id, int chat_id) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    auto *response = new chat_service::BoolResponse;
    request.set_user_id(user_id);
    request.set_chat_id(chat_id);

    grpc::Status status =
        chat_data_stub->CheckUserInChat(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }

    return response->response();
}

chat_service::ChatPreview *client::get_chat_preview(int chat_id) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    auto *response = new chat_service::ChatPreview;
    request.set_chat_id(chat_id);

    grpc::Status status = chat_data_stub->GetChatPreview(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }

    return response;
}

common::GroupChatPreview *client::get_group_chat_preview(int chat_id) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    auto *response = new common::GroupChatPreview;
    request.set_chat_id(chat_id);

    grpc::Status status = chat_data_stub->GetGroupChatPreview(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }

    return response;
}

std::vector<chat_service::ChatEvent> client::get_chat_events(int chat_id) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    auto *response = new chat_service::ChatEvents;
    request.set_chat_id(chat_id);

    grpc::Status status = chat_data_stub->GetChatEvents(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }

    std::vector<chat_service::ChatEvent> events;
    for (int i = 0; i < response->events_size(); i++) {
        events.push_back(response->events(i));
    }
    return events;
}

std::vector<int> client::get_chat_admins(int chat_id) {
    grpc::ClientContext context;

    chat_service::UserChat request;
    auto *response = new chat_service::ChatAdmins;
    request.set_chat_id(chat_id);

    grpc::Status status = chat_data_stub->GetChatAdmins(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << '\n';
    }

    std::vector<int> admins;
    for (int i = 0; i < response->admin_id_size(); i++) {
        admins.push_back(response->admin_id(i));
    }
    return admins;
}

authservice::LoginResponse *
client::login(const std::string &client_login, const std::string &password) {
    grpc::ClientContext context;
    authservice::LoginRequest request;
    auto *response = new authservice::LoginResponse;
    if (client_login.find('@') != -1) {
        request.set_email(client_login);
    } else {
        request.set_username(client_login);
    }
    request.set_password(password);
    grpc::Status status =
        authorization_stub->Login(&context, request, response);
    if (!status.ok()) {
        std::cout << status.error_message() << std::endl;
    }

    return response;
}

authservice::SignupResponse *client::registration(
    const std::string &client_login,
    const std::string &email,
    const std::string &password,
    const std::string &password_confirmation
) {
    grpc::ClientContext context;

    authservice::SignupRequest request;
    auto *response = new authservice::SignupResponse;
    request.set_username(client_login);
    request.set_email(email);
    request.set_password(password);
    request.set_password_confirmation(password_confirmation);

    grpc::Status status =
        authorization_stub->Signup(&context, request, response);

    if (!status.ok()) {
        std::cout << status.error_message() << std::endl;
    }

    return response;
}

void client::subscribe(int subscriber_id, int subscribie_id) {
    grpc::ClientContext context;

    users_interaction::SubscribeRequest request;
    request.set_subscriber_id(subscriber_id);
    request.set_subscribie_id(subscribie_id);

    users_interaction::SubscribeResponse response;

    grpc::Status status =
        users_interaction_stub->Subscribe(&context, request, &response);

    if (!status.ok()) {
        std::cout << status.error_message() << std::endl;
    }
}

void client::unsubscribe(int subscriber_id, int subscribie_id) {
    grpc::ClientContext context;

    users_interaction::UnsubscribeRequest request;
    request.set_subscriber_id(subscriber_id);
    request.set_subscribie_id(subscribie_id);

    users_interaction::UnsubscribeResponse response;

    grpc::Status status =
        users_interaction_stub->Unsubscribe(&context, request, &response);

    if (!status.ok()) {
        std::cout << status.error_message() << std::endl;
    }
}

std::vector<uint32_t> client::get_subscriptions(uint32_t subscriber_id) {
    users_interaction::ShowAllSubscriptionsRequest request;
    request.set_subscriber_id(subscriber_id);

    users_interaction::ShowAllSubscriptionsResponse response;

    grpc::ClientContext context;

    grpc::Status status = users_interaction_stub->ShowAllSubscriptions(
        &context, request, &response
    );

    std::vector<uint32_t> result;
    if (status.ok()) {
        for (int i = 0; i < response.subscribie_id_size(); i++) {
            result.push_back(response.subscribie_id(i));
        }
    } else {
        std::cout << status.error_message() << std::endl;
    }
    return result;
}

std::vector<uint32_t> client::get_subscribers(uint32_t subscribie_id) {
    users_interaction::ShowAllSubscribersRequest request;
    request.set_subscribie_id(subscribie_id);

    users_interaction::ShowAllSubscribersResponse response;

    grpc::ClientContext context;

    grpc::Status status = users_interaction_stub->ShowAllSubscribers(
        &context, request, &response
    );

    std::vector<uint32_t> result;
    if (status.ok()) {
        for (int i = 0; i < response.subscriber_id_size(); i++) {
            result.push_back(response.subscriber_id(i));
        }
    } else {
        std::cout << status.error_message() << std::endl;
    }
    return result;
}

void client::block(int blocker_id, int blockie_id) {
    grpc::ClientContext context;

    users_interaction::BlockRequest request;
    request.set_blocker_id(blocker_id);
    request.set_blockie_id(blockie_id);

    users_interaction::BlockResponse response;

    grpc::Status status =
        users_interaction_stub->Block(&context, request, &response);

    if (!status.ok()) {
        std::cout << status.error_message() << std::endl;
    }
}

void client::unblock(int blocker_id, int blockie_id) {
    grpc::ClientContext context;

    users_interaction::UnblockRequest request;
    request.set_blocker_id(blocker_id);
    request.set_blockie_id(blockie_id);

    users_interaction::UnblockResponse response;

    grpc::Status status =
        users_interaction_stub->Unblock(&context, request, &response);

    if (!status.ok()) {
        std::cout << status.error_message() << std::endl;
    }
}

std::vector<uint32_t> client::get_blocked(uint32_t blockie_id) {
    users_interaction::ShowAllBlockedRequest request;
    request.set_blockie_id(blockie_id);

    users_interaction::ShowAllBlockedResponse response;

    grpc::ClientContext context;

    grpc::Status status =
        users_interaction_stub->ShowAllBlocked(&context, request, &response);

    std::vector<uint32_t> result;
    if (status.ok()) {
        for (int i = 0; i < response.blockie_id_size(); i++) {
            result.push_back(response.blockie_id(i));
        }
    } else {
        std::cout << status.error_message() << std::endl;
    }
    return result;
}

bool client::is_subscribed(uint32_t user_id, uint32_t subscribie_id) {
    std::vector<uint32_t> subscribers = get_subscribers(subscribie_id);
    return std::find(subscribers.begin(), subscribers.end(), user_id) !=
           subscribers.end();
}

bool client::is_blocked(uint32_t user_id, uint32_t blockie_id) {
    std::vector<uint32_t> blocked = get_blocked(blockie_id);
    return std::find(blocked.begin(), blocked.end(), user_id) != blocked.end();
}

std::vector<uint32_t> client::get_mutual_subscriptions(uint32_t user_id) {
    users_interaction::GetMutualSubscriptionsRequest request;
    request.set_subscriber_id(user_id);
    users_interaction::GetMutualSubscriptionsResponse response;
    grpc::ClientContext context;
    grpc::Status status = users_interaction_stub->GetMutualSubscriptions(
        &context, request, &response
    );
    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return {};
    }
    std::vector<uint32_t> mutual_subscriptions(
        response.subscribie_id().begin(), response.subscribie_id().end()
    );
    return mutual_subscriptions;
}

void client::change_photo(uint32_t user_id, const std::string &photo_path) {
    profile::ProfilePhotoRequest request;

    std::ifstream file(photo_path, std::ios::binary);
    if (!file) {
        std::cout << "Cannot open file: " << photo_path << std::endl;
        return;
    }
    std::basic_string photo_data(
        (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
    );
    std::cout << photo_data;
    request.set_user_id(user_id);
    request.set_photo(photo_data);

    profile::ProfileReply response;
    grpc::ClientContext context;
    grpc::Status status =
        profile_stub->ChangeProfilePhoto(&context, request, &response);

    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return;
    }
}

std::vector<common::UserPreview> client::get_user_recommendations(
    uint32_t user_id
) {
    recommendation_service::UserId request;
    request.set_id(user_id);

    recommendation_service::GetUsersRecommendationsResponse response;
    grpc::ClientContext context;
    grpc::Status status = recommendation_stub->GetUsersRecommendations(
        &context, request, &response
    );

    std::vector<common::UserPreview> user_recommendations;
    if (status.ok()) {
        for (const auto &user : response.user()) {
            user_recommendations.push_back(user);
        }
    } else {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
    }

    return user_recommendations;
}

std::vector<common::FilmPreview> client::get_film_recommendations(
    uint32_t user_id
) {
    recommendation_service::UserId request;
    request.set_id(user_id);

    recommendation_service::GetFilmsRecommendationsResponse response;
    grpc::ClientContext context;
    grpc::Status status = recommendation_stub->GetFilmsRecommendations(
        &context, request, &response
    );

    std::vector<common::FilmPreview> film_recommendations;
    if (status.ok()) {
        for (const auto &film : response.film()) {
            film_recommendations.push_back(film);
        }
    } else {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
    }

    return film_recommendations;
}

common::FilmFullPage *client::get_film_full_page(int film_id) {
    grpc::ClientContext context;

    profile::FilmId request;
    auto *response = new common::FilmFullPage;

    request.set_id(film_id);
    grpc::Status status =
        profile_data_stub->GetFilmFullPage(&context, request, response);
    if (!status.ok()) {
        qDebug() << status.error_message().c_str();
        return {};
    }
    return response;
}

common::FilmPreview *client::get_film_preview(int film_id) {
    grpc::ClientContext context;

    profile::FilmId request;
    auto *response = new common::FilmPreview;

    request.set_id(film_id);
    grpc::Status status =
        profile_data_stub->GetFilmPreview(&context, request, response);
    if (!status.ok()) {
        qDebug() << status.error_message().c_str();
        return {};
    }
    return response;
}

bool client::is_genre_favourite(uint32_t user_id, const std::string &genre_name) {
    grpc::ClientContext context;

    profile::GenreRequest request;
    auto *response = new profile::IsChosenReply;
    request.set_id(user_id);
    profile::Genre *genre = request.add_genres();
    genre->set_name(genre_name);
    grpc::Status status =
        profile_stub->CheckIsGenreFavourite(&context, request, response);
    if (!status.ok()) {
        return false;
    }
    return response->is_chosen();
}

bool client::is_actor_favourite(uint32_t user_id, uint32_t actor_id) {
    grpc::ClientContext context;

    profile::ActorRequest request;
    auto *response = new profile::IsChosenReply;
    request.set_id(user_id);
    profile::Actor *actor = request.add_actors();
    actor->set_id(actor_id);
    grpc::Status status =
        profile_stub->CheckIsActorFavourite(&context, request, response);
    if (!status.ok()) {
        return false;
    }
    return response->is_chosen();
}

bool client::is_movie_watched(uint32_t user_id, uint32_t movie_id) {
    grpc::ClientContext context;

    profile::MovieRequest request;
    auto *response = new profile::IsChosenReply;
    request.set_id(user_id);
    profile::Movie *movie = request.add_movies();
    movie->set_id(movie_id);
    grpc::Status status =
        profile_stub->CheckIsMovieChosen(&context, request, response);
    if (!status.ok()) {
        return false;
    }
    return response->is_chosen();
}