#pragma once

#include <grpcpp/grpcpp.h>
#include <random>
#include <string>

#include "services/profile_service/chat.grpc.pb.h"
#include "services/profile_service/chat.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chat::ChatService;
using chat::JoinSessionResponse;
using chat::Login;
using chat::Logout;
using chat::Message;
using chat::StartSessionResponse;
using chat::StreamRequest;
using chat::StreamResponse;



class ChatServiceImpl final : public ChatService::Service {
    grpc::Status ChatStream(grpc::ServerContext* context, grpc::ServerReaderWriter<StreamResponse, StreamRequest>* stream) override {
        StreamRequest request;
        while (stream->Read(&request)) {
            StreamResponse response;
            if (request.has_new_session_request()) {
                // handle??
                StartSessionResponse new_session_response;
                std::string new_session_id = generate_session_id();
                new_session_response.set_session_id(new_session_id);
                response.mutable_new_session_response()->CopyFrom(new_session_response);
            } else if (request.has_message_request()) {
                // handle??
                Message message;
                message.set_message("Message received");
                response.mutable_message_response()->CopyFrom(message);
            }
            // handle??
            stream->Write(response);
        }
        return grpc::Status::OK;
    }

    std::string generate_session_id() {
		std::string generated_id = "";
		int i = 0;
		while (++i <= 8) {
			generated_id.append(std::to_string(rand() % 10));
		}
		return generated_id;
	}
};