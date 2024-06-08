#ifndef MANAGER_H
#define MANAGER_H

#include "client.hpp"
#include <vector>

#include <QObject>

class movie{
public:
    int id, ratio;

    bool operator==(const movie& other) const {
        return id == other.id;
    }
};

class Manager : public QObject
{    
    Q_OBJECT

public:

    client new_client;
    std::string username;
    int ID = -1;
    std::vector <movie> watchlist;

    explicit Manager(const std::shared_ptr<grpc::Channel> &new_channel, QObject* parent = nullptr)
        : QObject(parent), new_client(new_channel) {
            new_client.onNewMessageReceived = [this](const chat_service::MessageReceived& message) {
                emit newMessageReceived(message);
            };
            new_client.onNewInviteReceived = [this](const invitation::SendInvitationResponse& response) {
                emit NewInviteReceived(response);
            };
            new_client.onInviteRespondReceived = [this](const invitation::RespondToInvitationResponse& response) {
                emit InviteRespondReceived(response);
            };
            new_client.onRemoveFromChatReceived = [this](const chat_service::RemoveResponse& response) {
                emit RemoveFromChatReceived(response);
            };
            new_client.onSubscribersChanged = [this](const users_interaction::SubscribeResponse& response) {
                qDebug() << "Subscribers changed, emitting signal";
                emit subscribersChanged();
            };
            new_client.onSubscriptionsChanged = [this](const users_interaction::SubscribeResponse& response) {
                qDebug() << "Subscribers changed, emitting signal";
                emit subscriptionsChanged();
            };
            new_client.onBlockedChanged = [this](const users_interaction::BlockResponse& response) {
                emit blockedChanged();
            };
            new_client.onNeedLogin = [this]() {
                emit needLogin();
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
    void newMessageReceived(const chat_service::MessageReceived& message);
    void NewInviteReceived(const invitation::SendInvitationResponse& response);
    void InviteRespondReceived(const invitation::RespondToInvitationResponse& response);
    void RemoveFromChatReceived(const chat_service::RemoveResponse& response);
    void needLogin();
    void subscribersChanged();
    void subscriptionsChanged();
    void blockedChanged();
};

#endif // MANAGER_H
