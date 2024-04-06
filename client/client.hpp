#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>

#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"
#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.pb.h"
#include "services/profile_service/profile.grpc.pb.h"
#include "services/profile_service/profile.pb.h"

struct client {
private:
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<authservice::AuthService::Stub> authorization_stub;
    std::unique_ptr<authservice::AuthServiceData::Stub> authorization_data_stub;
    std::unique_ptr<chat_service::ChatDataService::Stub> chat_data_stub;
    std::unique_ptr<chat_service::ChatHandler::Stub> chat_stub;
    std::unique_ptr<profile::ProfileService::Stub> profile_stub;

public:
    explicit client(const std::shared_ptr<grpc::Channel> &new_channel);

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
    grpc::Status registration(
        const std::string &client_login,
        const std::string &email,
        const std::string &password,
        const std::string &password_confirmation
    );

};
