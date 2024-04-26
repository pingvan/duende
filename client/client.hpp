#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"
#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.pb.h"
#include "services/profile_service/profile.grpc.pb.h"
#include "services/profile_service/profile.pb.h"
#include "services/search_service/search_service.grpc.pb.h"
#include "services/search_service/search_service.pb.h"
#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"

struct client {
private:
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<authservice::AuthService::Stub> authorization_stub;
    std::unique_ptr<authservice::AuthServiceData::Stub> authorization_data_stub;
    std::unique_ptr<chat_service::ChatDataService::Stub> chat_data_stub;
    std::unique_ptr<chat_service::ChatHandler::Stub> chat_stub;
    std::unique_ptr<profile::ProfileService::Stub> profile_stub;
    std::unique_ptr<search_service::SerachService::Stub> search_stub;
    std::unique_ptr<invitation::InvitationService::Stub> invite_stub;

    std::unique_ptr<grpc::ClientReaderWriter<chat_service::ChatRequests, chat_service::ChatResponses>> chat_stream;
    std::unique_ptr<grpc::ClientReaderWriter<invitation::StreamRequest, invitation::StreamResponse>> invite_stream;

    grpc::ClientContext chat_context;
    grpc::ClientContext invite_context;

public:
    std::function<void(const chat_service::Message&)> onNewMessageReceived;
    std::function<void(const invitation::SendInvitationResponse&)> onNewInviteSended;
    std::function<void(const invitation::RespondToInvitationResponse&)> onInviteRespondReceived;

    explicit client(const std::shared_ptr<grpc::Channel> &new_channel); 

    void init_chat_stream(int user_id);  
    void start_reading_chat_stream();
    void send_message(int user_id, int chatId, const std::string& messageText);

    void start_reading_invite_stream();
    void send_invite(int sender_id, int receiver_id, std::string invite_text, invitation::InviteStatus invite_status);

    search_service::UserSearchResponse *search_user_by_credentials(const std::string &user_credentials);
    search_service::ActorSearchResponse *search_actor_by_name(const std::string &actor_name);
    search_service::FilmSearchResponse *search_film_by_title(const std::string &film_title);

    authservice::UserFullPage *get_user_full_page(int user_id);

    void change_username(int user_id, const std::string &new_username); 
    void change_quote(int user_id, const std::string &new_quote); 
    void change_bio(int user_id, const std::string &new_bio); 
    void add_actor(int user_id, int actor_id);
    void remove_actor(int user_id, int actor_id);
    void add_movie(int user_id, int movie_id, int rating);
    void remove_movie(int user_id, int movie_id, int rating);
    void add_genre(int user_id, const std::string &genre_name);
    void remove_genre(int user_id, const std::string &genre_name);

    std::vector<chat_service::ChatPreview> get_user_chats(int user_id);
    std::vector<chat_service::MessageData> get_last_messages(int user_id, int chat_id);

    authservice::LoginResponse *login(const std::string &client_login, const std::string &password);
    authservice::SignupResponse *registration(
        const std::string &client_login,
        const std::string &email,
        const std::string &password,
        const std::string &password_confirmation
    );

};
