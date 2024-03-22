#include "chat_service.hpp"

#include <chrono>
#include <iostream>

grpc::Status chat_service_::handle_chat(grpc::ServerContext *context, grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests> *stream) {
    chat_service::ChatRequests request;
    while (stream->Read(&request)) {
        if (request.has_init()) {
            users_online.insert({request.init().user_id(), std::make_shared<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>(*stream)});
            chat_service::ChatResponses response;
            const auto init_response= new chat_service::InitialResponse;
            init_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_init(init_response);
            stream->Write(response);
        } else if (request.has_send_message()) {
            send_to_chat(request.send_message().msg());
        } else if (request.has_leave()) {
            users_online.erase(request.leave().user_id());
            chat_service::ChatResponses response;
            const auto disconnect_response= new chat_service::DisconnectResponse;
            disconnect_response->set_status(chat_service::Status::SUCCESS);
            response.set_allocated_leave(disconnect_response);
            stream->Write(response);
        } else {
            //some unsupported message
        }
    }
    return grpc::Status::OK;
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
    std::cout << "inside sending\n";
    chat_service::ChatResponses response;
    auto *accept_message = new chat_service::AcceptMessage;
    auto *allocated_msg = new chat_service::Message(msg);
    accept_message->set_allocated_msg(allocated_msg);
    response.set_allocated_accept_message(accept_message);


    auto timestamp = make_timestamp();
    auto message_id = database::chat_service::add_message(msg.sender_id(), msg.chat_id(), msg.timestamp_(), msg.message_text());
    auto members_ids = database::chat_service::get_members_ids(msg.chat_id());
    for (auto member_id : members_ids) {
        if (auto ptr = users_online.find(member_id); ptr == users_online.end()) {
            Unhandled:
            database::chat_service::add_unhandle_message(member_id, message_id);
        } else {
            uint cnt = 0;
            std::cout << "writing\n";
            bool success = ptr->second->Write(response);
            while (!success || cnt != 10) {
                success = ptr->second->Write(response);
                cnt++;
            }
            if (!success) { //if not successfull - considering that stream is closed
                users_online.erase(ptr->first);
                goto Unhandled;
            }
        }
    }

}
