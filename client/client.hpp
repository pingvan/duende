#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <iostream>
#include <fstream>

#include "common/common.grpc.pb.h"
#include "common/common.pb.h"
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
#include "services/users_interaction_service/users_interaction.grpc.pb.h"
#include "services/users_interaction_service/users_interaction.pb.h"
#include "services/recommendation_service/recommendation_service.grpc.pb.h"
#include "services/recommendation_service/recommendation_service.pb.h"

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
    std::unique_ptr<invitation::InvitationDataService::Stub> invite_data_stub;
    std::unique_ptr<users_interaction::UsersInteractionService::Stub> users_interaction_stub;
    std::unique_ptr<recommendation_service::RecommendationService::Stub> recommendation_stub;
    std::unique_ptr<profile::ProfileServiceData::Stub> profile_data_stub;

    std::unique_ptr<grpc::ClientReaderWriter<chat_service::ChatRequests, chat_service::ChatResponses>> chat_stream;
    std::unique_ptr<grpc::ClientReaderWriter<chat_service::InvitesToChatRequests, chat_service::InvitesToChatResponses>> invite_chat_stream;
    std::unique_ptr<grpc::ClientReaderWriter<invitation::InvitationRequest, invitation::InvitationResponse>> invite_stream;

    grpc::ClientContext chat_context;
    grpc::ClientContext invite_chat_context;
    grpc::ClientContext invite_context;

public:
    std::function<void(const chat_service::MessageReceived&)> onNewMessageReceived;
    std::function<void(const invitation::SendInvitationResponse&)> onNewInviteReceived;
    std::function<void(const invitation::RespondToInvitationResponse&)> onInviteRespondReceived;
    std::function<void(const chat_service::RemoveResponse&)> onRemoveFromChatReceived;
    std::function<void(const users_interaction::SubscribeResponse&)> onSubscribersChanged;
    std::function<void(const users_interaction::SubscribeResponse&)> onSubscriptionsChanged;
    std::function<void(const users_interaction::BlockResponse&)> onBlockedChanged;

    explicit client(const std::shared_ptr<grpc::Channel> &new_channel); 

    void init_invite_chat_stream(int user_id);  
    void start_reading_invite_chat_stream();
    void leave_invite_chat_stream(int user_id);
    chat_service::ChatPreview *create_group_chat(int creator_id, std::string chat_name, std::string description);
    bool check_user_privileges(int user_id, int chat_id);
    void change_chat_name(int chat_id, std::string new_chat_name);
    void change_description(int chat_id, std::string new_description);
    // void change_cover(int chat_id, std::string new_cover);
    void add_privileges_to_user(int sender_id, int receiver_id, int chat_id);
    void remove_privileges_from_user(int sender_id, int receiver_id, int chat_id);
    void remove_user_from_chat(int sender_id, int receiver_id, int chat_id);

    void init_chat_stream(int user_id);  
    void start_reading_chat_stream();
    void leave_chat_stream(int user_id);
    void send_message(int user_id, int chatId, const std::string& messageText);

    void init_invite_stream(int user_id);
    void start_reading_invite_stream();
    void leave_invite_stream(int user_id);
    void send_invite(int sender_id, int receiver_id, std::string sender_login, std::string receiver_login, int chat_id = -1);
    void respond_invite(int invite_id, int sender_id, int receiver_id, int chat_id, bool accept);

    std::vector<invitation::Invite> get_sent_invites(int user_id);
    std::vector<invitation::Invite> get_received_invites(int user_id);

    search_service::UserSearchResponse *search_user_by_credentials(const std::string &user_credentials);
    search_service::ActorSearchResponse *search_actor_by_name(const std::string &actor_name);
    search_service::FilmSearchResponse *search_film_by_title(const std::string &film_title);

    common::UserFullPage *get_user_full_page(int user_id);
    //TODO get_user_preview

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
    std::vector<chat_service::MessageReceived> get_last_messages(int user_id, int chat_id);
    std::vector<common::UserPreview> get_chat_members(int chat_id);
    bool check_user_in_chat(int user_id, int chat_id);

    authservice::LoginResponse *login(const std::string &client_login, const std::string &password);
    authservice::SignupResponse *registration(
        const std::string &client_login,
        const std::string &email,
        const std::string &password,
        const std::string &password_confirmation
    );

    void subscribe(int subscriber_id, int subscribie_id);
    void unsubscribe(int subscriber_id, int subscribie_id);
    std::vector<uint32_t> get_subscriptions(uint32_t subscriber_id);
    std::vector<uint32_t> get_subscribers(uint32_t subscribie_id);
    void block(int blocker_id, int blockie_id);
    void unblock(int blocker_id, int blockie_id);
    std::vector<uint32_t> get_blocked(uint32_t blockie_id);
    bool is_subscribed(uint32_t user_id, uint32_t subscribie_id);
    bool is_blocked(uint32_t user_id, uint32_t blockie_id);
    bool invite_already_sent(uint32_t sender_id, uint32_t receiver_id);
    std::vector <uint32_t> get_mutual_subscriptions(uint32_t user_id);
    void change_photo(uint32_t user_id, const std::string &photo_path);
    std::vector<common::UserPreview> get_user_recommendations(uint32_t user_id);
    std::vector<common::FilmPreview> get_film_recommendations(uint32_t user_id);
    common::FilmFullPage *get_film_full_page(int film_id);
};
