#include "chat_service.hpp"
#include <grpcpp/support/status.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <cassert>
#include "components/database/services/chat_service/chat_service.hpp"
#include <iostream>
#include <memory>
#include <vector>


grpc::Status chat_service_::HandleChat(grpc::ServerContext *context, grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests> *stream) {
    chat_service::ChatRequests request;
    while (stream->Read(&request)) {
        if (request.has_init()) {
            users_online.insert({request.init().user_id(), std::make_shared<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>(*stream)});
            // users_online_weak.insert({request.init().user_id(), users_online[request.init().user_id()]});

            // auto sh = std::make_shared<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>(*stream);
            // std::weak_ptr<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>> gw = sh;
            // users_online_weak_created_locally.insert({request.init().user_id(), gw});
            // auto shared = std::make_shared<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>(*stream);
            // auto weak = shared;
            // users_online_weak.insert({request.init().user_id(), weak});
            chat_service::ChatResponses response;
            const auto init_response = new chat_service::InitialResponse;
            init_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_init(init_response);
            stream->Write(response);
        } else if (request.has_send_message()) {
            send_to_chat(request.send_message().msg());
        } else if (request.has_leave()) {
            // users_online.erase(request.leave().user_id());
            chat_service::ChatResponses response;
            const auto disconnect_response= new chat_service::DisconnectResponse;
            disconnect_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_leave(disconnect_response);
            stream->Write(response);
        } else {
        }
    }
    return grpc::Status::OK;
}

std::chrono::system_clock::time_point make_chrono_type(const std::string& timestamp_str) {
        std::tm tm = {};
        std::istringstream ss(timestamp_str);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        return tp;
}

std::string chat_service_::make_timestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto now_c = std::chrono::system_clock::to_time_t(now);
    const std::tm now_tm = *std::gmtime(&now_c);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &now_tm);
    std::string timestamp_str(buffer);
    return timestamp_str;
}


void chat_service_::send_to_chat(const chat_service::Message& msg) {
    chat_service::ChatResponses response;
    auto *accept_message = new chat_service::AcceptMessage;
    auto *allocated_msg = new chat_service::Message(msg);
    accept_message->set_allocated_msg(allocated_msg);
    response.set_allocated_accept_message(accept_message);


    auto timestamp = make_timestamp();
    auto message_id = database::chat_service_database_api::add_message(msg.sender_id(), msg.chat_id(), timestamp, msg.message_text());
    auto members_ids = database::chat_service_database_api::get_members_ids(msg.chat_id());
    
    
    // std::ranges::sort(members_ids);
    // auto members_id = std::unique(members_ids.begin(), members_ids.end());
    // auto members_id = std::ranges::unique(members_ids);
    // members_ids = {64, 71};
    for (auto member_id : members_ids) {
        if (member_id != msg.sender_id()) {
            if (auto ptr = users_online.find(member_id); ptr == users_online.end()) {
                Unhandled:
                database::chat_service_database_api::add_unhandle_message(member_id, message_id);
            } else {
                uint cnt = 0;
                std::cout << "writing to: " << ptr->first << '\n';
                // assert(ptr != users_online.end());

                // assert(ptr->second != nullptr);
                // std::cout << "pointer: " <<  ptr->second << '\n';
                // ptr->second->SendInitialMetadata();

                // std::cout << "looking inside users_online_weak\n";
                // std::cout << "gw.use_count() == " << users_online_weak[member_id].use_count() << "; ";
                // // we have to make a copy of shared pointer before usage:
                // auto spt = users_online_weak[member_id].lock();
                // if (spt)
                //     std::cout << "*spt is okay " << '\n';
                // else
                //     std::cout << "gw is expired\n";

                // bool success1 = spt->Write(response);

                // std::cout << "looking inside users_online_weak_created_locally\n";
                // std::cout << "gw.use_count() == " << users_online_weak_created_locally[member_id].use_count() << "; ";
                // // we have to make a copy of shared pointer before usage:
                // auto spt1 = users_online_weak_created_locally[member_id].lock();
                // if (spt1)
                //     std::cout << "*spt is okay " << '\n';
                // else
                //     std::cout << "gw is expired\n";

                // bool success2 = spt->Write(response);


                // std::cerr << "shared use count: " << ptr->second.use_count() << '\n';

                // if (!ptr->second) {
                //     std::cerr << "wow it works this way!!!1\n";
                //     return;
                // } else {
                //     std::cerr << "nonono\n";
                // }

                bool success = ptr->second->Write(response);
                std::cout << "status: " << success << '\n';
                while (!success && cnt != 10) {
                    // success = spt->Write(response);
                    success = ptr->second->Write(response);
                    cnt++;
                }
                if (!success) { //if not successfull - considering that stream is closed;
                    std::cout << "erasing: " << ptr->first << '\n';
                    users_online.erase(ptr->first);
                    goto Unhandled;
                }
            }
        }
    }
}

grpc::Status chat_data_service::GetUserChats(grpc::ServerContext *context, const chat_service::GetChatsRequest *request, chat_service::UserChats *response) {
        auto chats = database::chat_service_database_api::get_user_chats(request->user_id());
        chats.convert_to_proto(response);
        return grpc::Status::OK;
}

grpc::Status chat_data_service::GetLastMessages(grpc::ServerContext *context, const ::chat_service::GetChatMessagesRequest *request, ::chat_service::LastMessages *response) {
        auto is_user_in_chat = database::chat_service_database_api::check_user_in_chat(request->user_id(), request->chat_id());
        if (!is_user_in_chat) {
            return grpc::Status(grpc::StatusCode::PERMISSION_DENIED, "User is not in chat");
        }
        auto messages = database::chat_service_database_api::get_chats_messages(request->chat_id());
        messages.convert_to_proto(response);
        return grpc::Status::OK;
}
