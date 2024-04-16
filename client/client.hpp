#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <QtConcurrent/QtConcurrent>

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

    std::unique_ptr<grpc::ClientReaderWriter<chat_service::ChatRequests, chat_service::ChatResponses>> stream;

    grpc::ClientContext context;

public:
    explicit client(const std::shared_ptr<grpc::Channel> &new_channel); 

    void init_chat_stream(int user_id) {
        std::cout << "1\n";
        stream = chat_stub->HandleChat(&context);

        chat_service::ChatRequests request;
        auto *init = new chat_service::InitialRequest;
        init->set_user_id(user_id);
        request.set_allocated_init(init);
        stream->Write(request);

        start_reading_stream();
    }

    void start_reading_stream() {
        std::cout << "2\n";
        QFuture<void> future = QtConcurrent::run([this] {
            chat_service::ChatResponses response;
            while (stream->Read(&response)) {
                if (response.has_accept_message()) {
                    auto message = response.accept_message().msg();
                    std::cout << "Message: " << message.message_text() << std::endl;
                }
            }
        });
    }

    void send_message(int user_id, int chatId, const std::string& messageText) {
        std::cout << "3\n";
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
            std::cerr << "Error!!" << std::endl;
        }
    }

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
