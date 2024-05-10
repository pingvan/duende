#include "chat_service.hpp"
#include <grpcpp/support/status.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cassert>
#include "components/database/placeholders/placeholders.hpp"
#include "components/database/services/chat_service/chat_service.hpp"
#include "components/database/services/authservice/authservice.hpp"
#include "services/chat_service/chat_service.pb.h"
#include <iostream>
#include <memory>
#include <vector>
#include <thread>


grpc::Status chat_service_::HandleChat(grpc::ServerContext *context, grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests> *stream) {
    chat_service::ChatRequests request;
    uint user_id = 0;
    while (stream->Read(&request)) {
        if (request.has_init()) {
            user_id = request.init().user_id();
            users_online.insert({request.init().user_id(), std::make_shared<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>(*stream)});
            chat_service::ChatResponses response;
            const auto init_response = new chat_service::OperationStatusResponse;
            init_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_response_with_status(init_response);

            std::this_thread::sleep_for(std::chrono::seconds(5));

            std::cerr << "context status: " << context->IsCancelled() << '\n';

            bool res = stream->Write(response);
            std::cerr << "result of stream->Write(response): " << res << "\n";
        } else if (request.has_send_message()) {
            auto a = request.send_message().msg();
            stream->Write(send_to_chat(request.send_message().msg()));
        } else { 
            users_online.erase(request.leave().user_id());
            chat_service::ChatResponses response;
            const auto disconnect_response = new chat_service::OperationStatusResponse;
            disconnect_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_response_with_status(disconnect_response);
            stream->Write(response);
        }
    }
    std::cerr << "cycle is broken by: " << user_id << '\n';
    if (auto it = users_online.find(user_id); it != users_online.end()) {
        users_online.erase(it);
    }
    return grpc::Status::OK;
}

chat_service::AcceptMessageResponse* chat_service_::make_accept_message_response(const chat_service::MessageSend& msg, const std::string &timestamp) {
    auto *accept_message = new chat_service::AcceptMessageResponse;
    auto *message_received = new chat_service::MessageReceived;
    auto *user_preview_proto = new common::UserPreview;
    database::authservice_database_api::get_user_preview(msg.sender_id()).convert_to_proto(user_preview_proto);
    message_received->set_allocated_sender(user_preview_proto);
    message_received->set_chat_id(msg.chat_id());
    message_received->set_message_text(msg.message_text());
    message_received->set_time_stamp(timestamp);
    accept_message->set_allocated_msg(message_received);
    return accept_message;
}


chat_service::ChatResponses chat_service_::send_to_chat(const chat_service::MessageSend& msg) {
    auto id_timestamp = database::chat_service_database_api::add_message(msg.sender_id(), msg.chat_id(), msg.message_text());
    auto members_ids = database::chat_service_database_api::get_members_ids(msg.chat_id());

    chat_service::ChatResponses response;
    response.set_allocated_accept_message(make_accept_message_response(msg, id_timestamp.second));

    for (auto member_id : members_ids) {
            if (auto ptr = users_online.find(member_id); ptr == users_online.end()) {
                Unhandled:
                database::chat_service_database_api::add_unhandle_message(member_id, id_timestamp.first);
            } else {
                uint cnt = 0;
                bool success = ptr->second->Write(response);
                while (!success && cnt != 10) {
                    success = ptr->second->Write(response);
                    cnt++;
                }
                if (!success) { //if not successfull - considering that stream is closed;
                    users_online.erase(ptr->first);
                    goto Unhandled;
                }
            }
    }

    return response;
}

//make interseptors which check that this fucntion can be called only form other invite service
grpc::Status chat_service_::CreatePrivateChat(grpc::ServerContext *context, const chat_service::CreatePrivateChatRequest *request, chat_service::ChatPreview *response) {
        auto id_timestamp = database::chat_service_database_api::create_private_chat({request->firest_user_id(), request->second_user_id()});
        response->set_chat_id(id_timestamp.first);
        response->set_is_group_chat(false);
        auto first_user_preview = database::authservice_database_api::get_user_preview(request->firest_user_id()); 
        auto second_user_preview = database::authservice_database_api::get_user_preview(request->second_user_id());
        auto first_user_preview_proto = response->add_chat_members();
        auto second_user_preview_proto = response->add_chat_members();
        first_user_preview.convert_to_proto(first_user_preview_proto);
        second_user_preview.convert_to_proto(second_user_preview_proto);
        return grpc::Status::OK;
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
