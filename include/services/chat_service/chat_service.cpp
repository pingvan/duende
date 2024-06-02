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
    uint32_t user_id = 0;
    while (stream->Read(&request)) {
        if (request.has_init()) {
            std::cerr << "initing\n";
            user_id = request.init().user_id();
            users_online.insert({request.init().user_id(), std::make_shared<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>(*stream)});
            chat_service::ChatResponses response;
            const auto init_response = new chat_service::OperationStatusResponse;
            init_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_response_with_status(init_response);
            stream->Write(response);
        } else if (request.has_send_message()) {
            std::cerr << "sending\n";
            // auto a = request.send_message().msg();
            // stream->Write(send_to_chat(request.send_message().msg()));
            send_to_chat(request.send_message().msg()); 
        } else { 
            users_online.erase(request.leave().user_id());
            chat_service::ChatResponses response;
            const auto disconnect_response = new chat_service::OperationStatusResponse;
            disconnect_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_response_with_status(disconnect_response);
            stream->Write(response);
        }
    }
    if (auto it = users_online.find(user_id); it != users_online.end()) {
        users_online.erase(it);
    }
    return grpc::Status::OK;
}

chat_service::AcceptMessageResponse* chat_service_::make_accept_message_response(const chat_service::MessageSend& msg, const std::string &timestamp) {
    auto *accept_message = new chat_service::AcceptMessageResponse;
    auto *message_received = new chat_service::MessageReceived;
    auto *user_preview_proto = new common::UserPreview;
    std::cerr << "before preview\n";
    database::authservice_database_api::get_user_preview(msg.sender_id()).convert_to_proto(user_preview_proto);
    std::cerr << "get preview\n";
    message_received->set_allocated_sender(user_preview_proto);
    message_received->set_chat_id(msg.chat_id());
    message_received->set_message_text(msg.message_text());
    message_received->set_time_stamp(timestamp);
    accept_message->set_allocated_msg(message_received);
    return accept_message;
}


chat_service::ChatResponses chat_service_::send_to_chat(const chat_service::MessageSend& msg) {
    std::cerr << "first line\n";
    aes::encryptor encryptor;
    auto id_timestamp = database::chat_service_database_api::add_message(msg.sender_id(), msg.chat_id(), encryptor.encrypt(msg.message_text()));
    std::cerr << "get members\n";
    auto members_ids = database::chat_service_database_api::get_members_ids(msg.chat_id());
    std::cerr << "before making response\n";
    chat_service::ChatResponses response;
    response.set_allocated_accept_message(make_accept_message_response(msg, id_timestamp.second));
    std::cerr << "msg created\n";

    for (auto member_id : members_ids) {
            if (auto ptr = users_online.find(member_id); ptr != users_online.end()) {
                uint32_t cnt = 0;
                std::cerr << "writing to: " << ptr->first << std::endl;
                bool success = ptr->second->Write(response);
                while (!success && cnt != 10) {
                    success = ptr->second->Write(response);
                    cnt++;
                }
                if (!success) {
                    users_online.erase(ptr->first);
                    goto Unhandled;
                }
            } else {
                Unhandled:
                database::chat_service_database_api::add_unhandle_message(member_id, id_timestamp.first);
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

grpc::Status chat_service_::CreateGroupChat(grpc::ServerContext* context, const chat_service::CreateGroupChatRequest* request, chat_service::ChatPreview* response) {
    std::vector<uint32_t> chat_members = {static_cast<unsigned int>(request->creator_id())};
    auto id_timestamp = database::chat_service_database_api::create_group_chat(request->creator_id(), request->chat_name(), request->description(), chat_members);
    response->set_chat_id(id_timestamp.first);
    response->set_is_group_chat(true);
    response->set_name(request->chat_name());
    response->set_description(request->description());
    response->set_time_stamp(id_timestamp.second);
    for (auto member_id : chat_members) {
        auto member_preview = database::authservice_database_api::get_user_preview(member_id); 
        auto member_preview_proto = response->add_chat_members();
        member_preview.convert_to_proto(member_preview_proto);
    }
    return grpc::Status::OK;
}

grpc::Status chat_service_::CheckUserPrivileges(grpc::ServerContext* context, const chat_service::UserChat* request, chat_service::UserPrivilegeResponse* response) {
    bool is_user_admin = database::chat_service_database_api::check_user_privileges(request->user_id(), request->chat_id());
    chat_service::UserPrivilege privilege = is_user_admin ? chat_service::UserPrivilege::ADMIN : chat_service::UserPrivilege::DEFAULT;
    response->set_privilege(privilege);
    return grpc::Status::OK;
}

// static void add_user_to_chat(const int &chat_id, const int &user_id);
// static void remove_user_from_chat(const int &chat_id, const int &user_id);

grpc::Status chat_service_::ChangeChatName(grpc::ServerContext* context, const chat_service::ChangeChatNameRequest* request, chat_service::OperationStatusResponse* response) {
    database::chat_service_database_api::change_chat_name(request->chat_id(), request->admin_id(), request->new_chat_name());
    response->set_status(chat_service::Status::SUCCESS);
    return grpc::Status::OK;
}

grpc::Status chat_service_::ChangeChatDescription(grpc::ServerContext* context, const chat_service::ChangeChatDescriptionRequest* request, chat_service::OperationStatusResponse* response) {
    database::chat_service_database_api::change_chat_description(request->chat_id(), request->admin_id(), request->new_description());
    response->set_status(chat_service::Status::SUCCESS);
    return grpc::Status::OK;
}

grpc::Status chat_service_::ChangeChatCover(grpc::ServerContext* context, const chat_service::ChangeChatCoverRequest* request, chat_service::OperationStatusResponse* response) {
    // TODO
    // database::chat_service_database_api::change_chat_cover(request->chat_id(), request->new_cover());
    response->set_status(chat_service::Status::SUCCESS);
    return grpc::Status::OK;
}

grpc::Status chat_service_::AddPrivilegesToUser(grpc::ServerContext* context, const chat_service::UserChat* request, chat_service::OperationStatusResponse* response) {
    database::chat_service_database_api::add_privileges_to_user(request->chat_id(), request->user_id());
    response->set_status(chat_service::Status::SUCCESS);
    return grpc::Status::OK;
}

grpc::Status chat_service_::RemovePrivilegesFromUser(grpc::ServerContext* context, const chat_service::UserChat* request, chat_service::OperationStatusResponse* response) {
    database::chat_service_database_api::remove_privileges_from_user(request->chat_id(), request->user_id());
    response->set_status(chat_service::Status::SUCCESS);
    return grpc::Status::OK;
}

grpc::Status chat_service_::InvitesToChat(grpc::ServerContext* context, grpc::ServerReaderWriter<chat_service::InvitesToChatResponses, chat_service::InvitesToChatRequests>* stream) {
    chat_service::InvitesToChatRequests request;
    uint32_t user_id = 0;   
    while (stream->Read(&request)) {
        if (request.has_init()) {
            user_id = request.init().user_id();
            chat_service::InvitesToChatResponses response;
            users_invites_online.insert(
                {request.init().user_id(),
                 std::make_shared<grpc::ServerReaderWriter<
                     chat_service::InvitesToChatResponses, chat_service::InvitesToChatRequests>>(*stream)}
            );
            const auto init_response = new chat_service::OperationStatusResponse;
            init_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_response_with_status(init_response);
            stream->Write(response);

        } else if (request.has_remove()) {
            int sender_id = request.remove().sender_id();
            int receiver_id = request.remove().receiver_id();
            int chat_id = request.remove().chat_id();

            auto sender_preview = database::authservice_database_api::get_user_preview(sender_id);
            auto receiver_preview = database::authservice_database_api::get_user_preview(receiver_id);

            auto sender_preview_proto = new common::UserPreview();
            auto receiver_preview_proto = new common::UserPreview();

            sender_preview.convert_to_proto(sender_preview_proto);
            receiver_preview.convert_to_proto(receiver_preview_proto);

            std::string sender_login = sender_preview_proto->login();
            std::string receiver_login = receiver_preview_proto->login();

            database::chat_service_database_api::remove_user_from_chat(chat_id, receiver_id);

            chat_service::InvitesToChatResponses response;
            const auto remove_response = new chat_service::RemoveResponse;

            const auto sender = new common::UserPreview;
            sender->set_login(sender_login);
            sender->set_user_id(sender_id);

            const auto receiver = new common::UserPreview;
            receiver->set_login(receiver_login);
            receiver->set_user_id(receiver_id);

            remove_response->set_allocated_sender(sender);
            remove_response->set_allocated_receiver(receiver);
            remove_response->set_chat_id(chat_id);

            response.set_allocated_remove(remove_response);

            std::vector<int> chat_members = database::chat_service_database_api::get_members_ids(chat_id);

            for (int member_id : chat_members) {
                if (users_invites_online.count(member_id)) {
                    users_invites_online[member_id]->Write(response);
                }
            }

            stream->Write(response);
            
        } else if (request.has_leave()) {
            chat_service::InvitesToChatResponses response;
            users_invites_online.erase(request.leave().user_id());
            const auto leave_response = new chat_service::OperationStatusResponse;
            leave_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_response_with_status(leave_response);
            stream->Write(response);
        }
    } 
    if (auto it = users_invites_online.find(user_id); it != users_invites_online.end()) {
        users_invites_online.erase(it);
    }
    return grpc::Status::OK;
}

grpc::Status chat_service_::AddUserToChat(grpc::ServerContext* context, const chat_service::UserChat* request, chat_service::OperationStatusResponse* response) {
    database::chat_service_database_api::add_user_to_chat(request->chat_id(), request->user_id());
    response->set_status(chat_service::Status::SUCCESS);
    return grpc::Status::OK;
}

grpc::Status chat_data_service::GetUserChats(grpc::ServerContext *context, const chat_service::GetChatsRequest *request, chat_service::UserChats *response) {
        std::cout << "GetUserChats called from server\n";
        auto chats = database::chat_service_database_api::get_user_chats(request->user_id());
        chats.convert_to_proto(response);
        std::cout << "response->user_chats_size(): " << response->user_chats_size() << '\n';
        return grpc::Status::OK;
}

grpc::Status chat_data_service::GetLastMessages(grpc::ServerContext *context, const ::chat_service::GetChatMessagesRequest *request, ::chat_service::LastMessages *response) {
        std::cout << "Server got chat_id: " << request->chat_id() << std::endl;
        auto is_user_in_chat = database::chat_service_database_api::check_user_in_chat(request->user_id(), request->chat_id());
        if (!is_user_in_chat) {
            return grpc::Status(grpc::StatusCode::PERMISSION_DENIED, "User is not in chat");
        }
        auto messages = database::chat_service_database_api::get_chats_messages(request->chat_id());
        messages.convert_to_proto(response);
        std::cout << "response->messages_size(): " << response->messages_size() << '\n';
        return grpc::Status::OK;
}

grpc::Status chat_data_service::GetChatMembers(grpc::ServerContext *context, const chat_service::UserChat *request, chat_service::GetChatMembersResponse *response) {
    auto members = database::chat_service_database_api::get_members_ids(request->chat_id());
    for (auto member_id : members) {
        auto member_preview = database::authservice_database_api::get_user_preview(member_id);
        auto member_preview_proto = response->add_members();
        member_preview.convert_to_proto(member_preview_proto);
    }
    return grpc::Status::OK;
}

grpc::Status chat_data_service::CheckUserInChat(grpc::ServerContext *context, const chat_service::UserChat *request, chat_service::BoolResponse *response) {
    bool is_user_in_chat = database::chat_service_database_api::check_user_in_chat(request->user_id(), request->chat_id());
    response->set_response(is_user_in_chat);
    return grpc::Status::OK;
}
