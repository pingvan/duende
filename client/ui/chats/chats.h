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
#include <QDialog>
#include <QGraphicsDropShadowEffect>
#include <QMenu>
#include <QFileDialog>


namespace Ui {
class Chats;
}

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
        : QLabel(parent) {}

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override {
        emit clicked();
    }
};

class Chats : public QWidget
{
    Q_OBJECT

public:
    explicit Chats(Manager* managerPtr, QWidget *parent = nullptr);
    void showChatInfo(bool editMode);
    QString parseTimeStamp(const std::string& time_stamp);
    ~Chats();
    QPixmap make_rounded_pixmap(const QPixmap& pixmap, int s);

signals:
    void messageReceived(const QString& message);
    void userClicked(int user_id);
    void profileButtonClicked(int id);

private slots:
    void build();
    void connect_slots();
    void onChatClicked(QListWidgetItem* item);
    void onInviteClicked(QListWidgetItem* item);
    void sendMessage();
    void onNewMessageReceived(const chat_service::MessageReceived& message);
    void onNewInviteReceived(const invitation::SendInvitationResponse& invite_response);
    void onInviteRespond(bool accept);
    void onInviteRespondReceived(const invitation::RespondToInvitationResponse&);
    void onRemoveFromChatReceived(const chat_service::RemoveResponse&);
    void make_chat_item(const chat_service::ChatPreview& chat, int chat_page_index, int item_index);
    void make_message_item(const chat_service::MessageReceived& message);
    void make_invite_item(const invitation::Invite& invite, bool am_i_sender);
    void delete_invite_item(int invite_id);
    void delete_chat_item(int chat_id);
    void updateChatList(const QString &input);
    void updateInviteList(const QString &input);
    void onAddChatButtonClicked();
    void onLeaveChatButtonClicked();
    void onAddMemberToChatButtonClicked();
    void onGenerateFilmButtonClicked();
    void onUserInPrivateChatClicked();
    void onUserInInviteClicked();
    void onProfileButtonClicked();

private:
    Ui::Chats *ui;
    Manager *manager;
    QHBoxLayout *mainLayout;
    QWidget *chatWidget;
    QVBoxLayout *chatWidgetLayout;
    QWidget *inviteWidget;
    QVBoxLayout *inviteWidgetLayout;

    int currentChatId = -1;
    int currentInviteId = -1;
    QStackedWidget *chatPages;
    QStackedWidget *invitePages;
    QListWidget *chatList;
    QListWidget *inviteList;
    QTextEdit *chatMessages;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    ClickableLabel *chatTitle;
    ClickableLabel *inviteTitle;
    QPushButton* toggleButton;
    QPushButton* addChatButton;
    QStackedWidget *rightLayout;
    QHBoxLayout *toolBarLayout;
    QPushButton* generateFilmButton;
    QPushButton* chatInfoButton;
    QPushButton* addMemberToChatButton;
    QPushButton* leaveChatButton;
    QHBoxLayout *inputLayout;

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
    QMap<int, invitation::Invite> invitesData;
    QMap<int, chat_service::ChatPreview> chatsData;

    QLineEdit *search_line_chats;
    QLineEdit *search_line_invites;

    ClickableLabel* chatPhoto;
};

#endif // CHATS_H
