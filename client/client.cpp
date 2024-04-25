#include "client.hpp"


client::client(const std::shared_ptr<grpc::Channel> &new_channel)
    : channel(new_channel),
      authorization_stub(authservice::AuthService::NewStub(new_channel)),
      authorization_data_stub(authservice::AuthServiceData::NewStub(new_channel)),
      chat_data_stub(chat_service::ChatDataService::NewStub(new_channel)),
      chat_stub(chat_service::ChatHandler::NewStub(new_channel)),
      profile_stub(profile::ProfileService::NewStub(new_channel)),
      search_stub(search_service::SerachService::NewStub(new_channel)) {
}

void client::init_chat_stream(int user_id) {
    stream = chat_stub->HandleChat(&context);

    chat_service::ChatRequests request;
    auto *init = new chat_service::InitialRequest;
    init->set_user_id(user_id);
    request.set_allocated_init(init);
    stream->Write(request);

    start_reading_stream();
}   

void client::start_reading_stream() {
    qDebug() << "start_reading_stream started";
    QFuture<void> future = QtConcurrent::run([this] {
        qDebug() << "QtConcurrent running";
        chat_service::ChatResponses response;
        while (stream->Read(&response)) {
            qDebug() << "Response received";
            if (response.has_accept_message()) {
                auto message = response.accept_message().msg();
                qDebug() << "Message accepted:" << QString::fromStdString(message.message_text());
                if (onNewMessageReceived) {
                    onNewMessageReceived(message);
                }
            }
        }
        qDebug() << "Stream ended";
    });
}

void client::send_message(int user_id, int chatId, const std::string& messageText) {
    chat_service::SendMessageRequest request;
    auto* message = new chat_service::Message();
    message->set_sender_id(user_id);
    message->set_chat_id(chatId);
    message->set_message_text(messageText);
    request.mutable_msg()->CopyFrom(*message);

    chat_service::ChatRequests chatRequest;
    chatRequest.mutable_send_message()->CopyFrom(request);

    std::cout << chatRequest.DebugString() << std::endl;
    bool status = stream->Write(chatRequest);
    if (!status) {
        std::cerr << "Error" << std::endl;
    }
}

search_service::UserSearchResponse *client::search_user_by_credentials(const std::string &user_credentials) {
    grpc::ClientContext context;
    search_service::UserSearchRequest request;
    auto *response = new search_service::UserSearchResponse;
    request.set_user_credentials(user_credentials);
    grpc::Status status = search_stub->SearchUserByCredentials(&context, request, response);
    return response;
}

search_service::ActorSearchResponse *client::search_actor_by_name(const std::string &actor_name) {
    grpc::ClientContext context;
    search_service::ActorSearchRequest request;
    auto *response = new search_service::ActorSearchResponse;
    request.set_actor_name(actor_name);
    grpc::Status status = search_stub->SearchActorByName(&context, request, response);
    return response;
}

search_service::FilmSearchResponse *client::search_film_by_title(const std::string &film_title) {
    grpc::ClientContext context;
    search_service::FilmSearchRequest request;
    auto *response = new search_service::FilmSearchResponse;
    request.set_film_title(film_title);
    grpc::Status status = search_stub->SearchFilmByTitle(&context, request, response);
    return response;
}

authservice::UserFullPage *client::get_user_full_page(int user_id) {
    grpc::ClientContext context;
    authservice::RequestWithUserId request;
    auto *response = new authservice::UserFullPage;
    
    std::cout << "ID: " << user_id << std::endl;

    request.set_user_id(user_id);
    grpc::Status status = authorization_data_stub->GetUserFullPage(&context, request, response);
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
    profile::Genre* genre = request.add_genres();
    genre->set_name(genre_name);
    grpc::Status status = profile_stub->AddToListOfGenres(&context, request, response);
}

void client::remove_genre(int user_id, const std::string &genre_name) {
    grpc::ClientContext context;
    profile::GenreRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Genre* genre = request.add_genres();
    genre->set_name(genre_name);
    grpc::Status status = profile_stub->RemoveFromListOfGenres(&context, request, response);
}

void client::add_movie(int user_id, int movie_id, int rating) {
    grpc::ClientContext context;
    profile::MovieRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Movie* movie = request.add_movies();
    movie->set_id(movie_id);
    movie->set_rating(rating);
    grpc::Status status = profile_stub->AddToWatchlist(&context, request, response);
}

void client::remove_movie(int user_id, int movie_id, int rating) {
    grpc::ClientContext context;
    profile::MovieRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Movie* movie = request.add_movies();
    movie->set_id(movie_id);
    movie->set_rating(rating);
    grpc::Status status = profile_stub->RemoveFromWatchlist(&context, request, response);
}

void client::add_actor(int user_id, int actor_id) {
    grpc::ClientContext context;
    profile::ActorRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Actor* actor = request.add_actors();
    actor->set_id(actor_id);
    grpc::Status status = profile_stub->AddToListOfActors(&context, request, response);
}

void client::remove_actor(int user_id, int actor_id) {
    grpc::ClientContext context;
    profile::ActorRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    profile::Actor* actor = request.add_actors();
    actor->set_id(actor_id);
    grpc::Status status = profile_stub->RemoveFromListOfActors(&context, request, response);
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
    grpc::Status status = profile_stub->ChangeQuote(&context, request, response);
}

void client::change_username(int user_id, const std::string &new_username) {
    grpc::ClientContext context;
    profile::UsernameRequest request;
    auto *response = new profile::ProfileReply;
    request.set_id(user_id);
    request.set_username(new_username);
    grpc::Status status = profile_stub->ChangeUsername(&context, request, response);
}

std::vector<chat_service::ChatPreview> client::get_user_chats(int user_id) {
    grpc::ClientContext context;
    chat_service::GetChatsRequest request;
    auto *response = new chat_service::UserChats;
    request.set_user_id(user_id);
    grpc::Status status = chat_data_stub->GetUserChats(&context, request, response);
    if (!status.ok()) {
        return {};
    }

    std::vector<chat_service::ChatPreview> chats;
    for (int i = 0; i < response->user_chats_size(); i++) {
        chats.push_back(response->user_chats(i));
    }
    return chats;
}

std::vector<chat_service::MessageData> client::get_last_messages(int user_id, int chat_id) {
    grpc::ClientContext context;
    chat_service::GetChatMessagesRequest request;
    auto *response = new chat_service::LastMessages;
    request.set_user_id(user_id);
    request.set_chat_id(chat_id);
    grpc::Status status = chat_data_stub->GetLastMessages(&context, request, response);
    if (!status.ok()) {
        return {};
    }

    std::vector<chat_service::MessageData> messages;
    for (int i = 0; i < response->messages_size(); i++) {
        messages.push_back(response->messages(i));
    }
    return messages;
}

authservice::LoginResponse *client::login(const std::string &client_login, const std::string &password) {
    grpc::ClientContext context;
    authservice::LoginRequest request;
    auto *response = new authservice::LoginResponse;
    if (client_login.find('@') != -1) {
        request.set_email(client_login);
    }
    else {
        request.set_username(client_login);
    }
    request.set_password(password);
    grpc::Status status = authorization_stub->Login(&context, request, response);
    if (!status.ok()) {
        response->set_message(status.error_message());
    }
    else {
        response->set_message("OK");
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

    grpc::Status status = authorization_stub->Signup(&context, request, response);
    if (!status.ok()) {
        response->set_message(status.error_message());
    }
    else {
        response->set_message("OK");
    }
    return response;
}