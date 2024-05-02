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
#include <QGraphicsDropShadowEffect>


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
    void build();
    void connect_slots();
    void onChatClicked(QListWidgetItem* item);
    void onInviteClicked(QListWidgetItem* item);
    void sendMessage();
    void onNewMessageReceived(const chat_service::MessageReceived& message);
    void onNewInviteReceived(const invitation::SendInvitationResponse&);
    void onInviteRespond(bool accept);
    void onInviteRespondReceived(const invitation::RespondToInvitationResponse&);
    void make_chat_item(const chat_service::ChatPreview& chat, int chat_page_index, int item_index);
    void make_message_item(const chat_service::MessageReceived& message);
    void make_invite_item(const invitation::Invite& invite, bool am_i_sender, int invite_page_index);
    void delete_invite_item(int invite_id);
    void updateChatList(const QString &input);
    void updateInviteList(const QString &input);

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
    QVBoxLayout *invitePageLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *scrollWidgetLayout;
    QWidget *messageWidget;
    QVBoxLayout *messageLayout;

    QPushButton* acceptButton;
    QPushButton* declineButton;

    QMap<int, std::vector<chat_service::MessageReceived>> chatMessagesData;

    QLineEdit *search_line_chats;
    QLineEdit *search_line_invites;
    QStringListModel *model_chats;
    QStringListModel *model_invites;
    QCompleter *completer_chats;
    QCompleter *completer_invites;
};

#endif // CHATS_H
