#include <unordered_map>
#include <mutex>
#include <grpcpp/grpcpp.h>
#include <memory>

#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.pb.h"

#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"


struct usersOnlineHandler {
    usersOnlineHandler() = default;
    usersOnlineHandler(const usersOnlineHandler&) = delete;
    usersOnlineHandler& operator=(const usersOnlineHandler&) = delete;

    static usersOnlineHandler* get_instance() {
        if (instance == nullptr) {
            instance = std::make_unique<usersOnlineHandler>();
        }
        return instance.get();
    }

    private:
        static std::unique_ptr<usersOnlineHandler> instance;
        std::mutex m;

        std::unordered_map<uint, std::shared_ptr<grpc::ServerReaderWriter<chat_service::ChatResponses, chat_service::ChatRequests>>> users_online_chat;
};