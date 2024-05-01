#ifndef CHATS_H
#define CHATS_H

#include "manager.h"
#include "otherpage/otherpage.h"

#include <grpcpp/grpcpp.h>
#include <QDebug>
#include <QWidget>
#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtConcurrent/QtConcurrent>
#include <QLabel>
#include <QThread>
#include <QScrollArea>
#include <QScrollBar> 

namespace Ui {
class Chats;
}

class Chats : public QWidget
{
    Q_OBJECT

public:
    explicit Chats(Manager* managerPtr, QWidget *parent = nullptr);
    ~Chats();

signals:
    void messageReceived(const QString& message);

private slots:
    void sendMessage();
    void onChatClicked(QListWidgetItem* item);
    void onNewMessageReceived(const chat_service::Message& message);
    void onNewInviteSent(const invitation::Invite& invite);
    void onNewInviteReceived(const invitation::SendInvitationResponse&);
    void onInviteRespondReceived(const invitation::RespondToInvitationResponse&);
    void build();
    void connect_slots();
    void make_chat_item(const chat_service::ChatPreview& chat, int chat_page_index, int item_index);
    void make_message_item(const chat_service::MessageData& message);
    void make_invite_item(const invitation::Invite& invite, int invite_page_index);

private:
    Ui::Chats *ui;
    Manager *manager;
    QHBoxLayout *mainLayout;

    int currentChatId = -1;
    int currentInviteId = -1;
    QStackedWidget *chatPages;
    QStackedWidget *invitePages;
    QListWidget *chatList;
    QListWidget *inviteList;
    QTextEdit *chatMessages;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QLabel *chatTitle;
    QLabel *inviteTitle;
    QPushButton* toggleButton;

    QVBoxLayout *chatPageLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *scrollWidgetLayout;
    QWidget *messageWidget;
    QVBoxLayout *messageLayout;
};

#endif // CHATS_H
