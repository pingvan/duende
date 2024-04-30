#ifndef MANAGER_H
#define MANAGER_H

#include "client.hpp"

#include <QObject>

class Manager : public QObject
{    
    Q_OBJECT

public:

    client new_client;
    std::string username;
    int ID = -1;

    explicit Manager(const std::shared_ptr<grpc::Channel> &new_channel, QObject* parent = nullptr)
        : QObject(parent), new_client(new_channel) {
            new_client.onNewMessageReceived = [this](const chat_service::Message& message) {
                emit newMessageReceived(message);
            };
            new_client.onNewInviteSended = [this](const invitation::SendInvitationResponse& response) {
                emit NewInviteSended(response);
            };
            new_client.onInviteRespondReceived = [this](const invitation::RespondToInvitationResponse& response) {
                emit InviteRespondReceived(response);
            };
    }

    void logout() {
        ID = -1;
        username = "";
    }

    Manager() = default;
    Manager(const Manager& other) = delete;
    Manager(Manager&& other) noexcept = default;
    Manager &operator=(const Manager&) = delete;
    Manager &operator=(Manager&&) noexcept = default;

signals:
    void newMessageReceived(const chat_service::Message& message);
    void NewInviteSended(const invitation::SendInvitationResponse& response);
    void InviteRespondReceived(const invitation::RespondToInvitationResponse& response);
    
};

#endif // MANAGER_H
