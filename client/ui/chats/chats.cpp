#include "chats.h"
#include "ui_chats.h"
#include <iostream>

Chats::Chats(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chats)
    , manager(managerPtr)
{   
    ui->setupUi(this);

    qDebug() << "Chats page start\n";

    qDebug() << "ID: " << manager->ID;

    // build layout
    build();

    // user chats and invites
    std::vector<chat_service::ChatPreview> chats = manager->new_client.get_user_chats(manager->ID);
    qDebug() << "Chats size: " << chats.size();
    std::vector<invitation::Invite> received_invites = manager->new_client.get_received_invites(manager->ID);
    qDebug() << "Received invites size: " << received_invites.size();
    std::vector<invitation::Invite> sent_invites = manager->new_client.get_sent_invites(manager->ID);
    qDebug() << "Sent invites size: " << sent_invites.size();
    std::sort(chats.begin(), chats.end(), [](const chat_service::ChatPreview& a, const chat_service::ChatPreview& b) {
        return a.time_stamp() < b.time_stamp();
    });

    // deal with invites
    for (const auto &invite : received_invites) {
        make_invite_item(invite, false);
    }

    for (const auto &invite : sent_invites) {
        make_invite_item(invite, true);
    }

    // deal with chats
    for (const auto &chat : chats) {
        auto *chatPage = new QWidget();
        int index = chatPages->addWidget(chatPage);
        make_chat_item(chat, index, -1);
    }

    // connect slots
    connect_slots();

    qDebug() << "Chats page end\n";
}

void Chats::onGenerateFilmButtonClicked() {
    int chat_id = currentChatId;

    QDialog filmDialog;
    filmDialog.setWindowTitle("Recommended Film");

    QVBoxLayout layout(&filmDialog);
    filmDialog.setLayout(&layout);

    QLabel *textLabel = new QLabel("We recommend that you watch this film:");
    layout.addWidget(textLabel);

    QLabel *imageLabel = new QLabel;
    QPixmap pixmap(":/img/titanik.jpeg");
    imageLabel->setPixmap(pixmap);
    layout.addWidget(imageLabel);

    filmDialog.exec();
}

void Chats::onLeaveChatButtonClicked()
{
    manager->new_client.remove_user_from_chat(manager->ID, manager->ID, currentChatId);
}

void Chats::onAddMemberToChatButtonClicked()
{
    int chat_id = currentChatId;

    QDialog dialog;
    dialog.setWindowTitle("Add members");

    QLabel membersLabel("Add members to your chat:");
    QListWidget membersList;
    QMap<QString, uint32_t> userMap;
    std::vector<uint32_t> members = manager->new_client.get_mutual_subscriptions(manager->ID);
    std::vector<uint32_t> chose_members;

    for (uint32_t member : members) {
        QString memberName = QString::fromStdString(manager->new_client.get_user_full_page(member)->preview().login());
        QListWidgetItem *item = new QListWidgetItem(memberName);
        membersList.addItem(item);
        userMap.insert(memberName, member);
    }

    QPushButton createButton("Save");

    QVBoxLayout layout(&dialog);
    layout.addWidget(&membersLabel);
    layout.addWidget(&membersList);
    layout.addWidget(&createButton);

    QObject::connect(&membersList, &QListWidget::itemClicked, [&](QListWidgetItem *item) {
        QString selectedItemText = item->text();
        uint32_t userId = userMap.value(selectedItemText);

        if (manager->new_client.check_user_in_chat(userId, chat_id)) {
            QMessageBox::critical(this, "Error", "User is already a member of the chat");
        }
        else {
            chose_members.push_back(userId);
        }

    });

    QObject::connect(&createButton, &QPushButton::clicked, [&]() {
        
        for (auto member : chose_members) {
            manager->new_client.send_invite(manager->ID, member, chat_id);
        }

        dialog.close();
    });

    dialog.exec();
}

void Chats::showChatInfo(bool editMode)
{
    int chat_id     = currentChatId;
    bool am_i_admin = manager->new_client.check_user_privileges(manager->ID, chat_id);

    QDialog dialog;
    dialog.setWindowTitle(chatTitle->text());

    QVBoxLayout layout(&dialog);
    dialog.setLayout(&layout);

    QLabel nameLabel("Chat name:");
    layout.addWidget(&nameLabel);

    QLineEdit nameLineEdit;
    QLabel nameText;
    if (editMode) {
        nameLineEdit.setText(chatTitle->text());
        layout.addWidget(&nameLineEdit);
    } else {
        if (nameLineEdit.text().isEmpty()) {
            nameText.setText(chatTitle->text());
        }
        else {
            nameText.setText(nameLineEdit.text());
        }
        layout.addWidget(&nameText);
    }

    QLabel descLabel("Description:");
    layout.addWidget(&descLabel);

    QLineEdit descLineEdit;
    QLabel descText;
    if (editMode) {
        descLineEdit.setText(chatList->currentItem()->data(Qt::UserRole + 4).toString());
        layout.addWidget(&descLineEdit);
    } else {
        if (descLineEdit.text().isEmpty()) {
            descText.setText(chatList->currentItem()->data(Qt::UserRole + 4).toString());
        }
        else {
            descText.setText(descLineEdit.text());
        }
        layout.addWidget(&descText);
    }

    QListWidget membersList;
    QMap<QString, uint32_t> userMap;
    std::vector<common::UserPreview> members = manager->new_client.get_chat_members(chat_id);

    if (editMode) {
        membersList.setContextMenuPolicy(Qt::CustomContextMenu);

        QObject::connect(&membersList, &QListWidget::customContextMenuRequested, [&](const QPoint &pos) {
            if (!am_i_admin) {
                return;
            }

            QListWidgetItem *item = membersList.itemAt(pos);
            if (item) {
                QString selectedItemText = item->text();
                uint32_t userId = userMap.value(selectedItemText);
                bool is_member_admin = manager->new_client.check_user_privileges(userId, chat_id);

                QMenu contextMenu;
                QAction *toggleAdminAction;
                QAction *removeMemberAction;

                if (is_member_admin) {
                    toggleAdminAction = contextMenu.addAction("Remove admin role");
                } else {
                    toggleAdminAction = contextMenu.addAction("Add admin role");
                }

                removeMemberAction = contextMenu.addAction("Remove member");

                QAction *selectedAction = contextMenu.exec(membersList.mapToGlobal(pos));

                if (selectedAction == removeMemberAction) {
                    qDebug() << "Remove user ID: " << userId;
                    manager->new_client.remove_user_from_chat(manager->ID, userId, chat_id);
                    delete item;
                    dialog.accept();
                }
                
                if (selectedAction == toggleAdminAction) {
                    if (is_member_admin) {
                        qDebug() << "Remove admin role for user ID: " << userId;
                        manager->new_client.remove_privileges_from_user(manager->ID, userId, chat_id);
                        item->setText(item->text().remove(" (admin)"));
                        if (userId == manager->ID) {
                            am_i_admin = false;
                        }
                        is_member_admin = false;
                    } else {
                        qDebug() << "Add admin role for user ID: " << userId;
                        manager->new_client.add_privileges_to_user(manager->ID, userId, chat_id);
                        item->setText(item->text() + " (admin)");
                        is_member_admin = true;
                    }
                    dialog.accept();
                }
            }
        });
    }

    for (auto member : members) {
        int member_id = member.user_id();
        bool is_member_admin = manager->new_client.check_user_privileges(member_id, chat_id);
        QString memberName = QString::fromStdString(member.login());
        if (is_member_admin) {
            memberName += " (admin)";
        }
        QListWidgetItem *item = new QListWidgetItem(memberName);
        membersList.addItem(item);
        userMap.insert(memberName, member_id);
    }

    layout.addWidget(&membersList);

    QObject::connect(&membersList, &QListWidget::itemClicked, [&](QListWidgetItem *item) {
        QString selectedItemText = item->text();
        uint32_t userId = userMap.value(selectedItemText);
        emit userClicked(userId);
        qDebug() << "Selected user ID: " << userId;
    });

    QPushButton *saveButton = nullptr;
    QPushButton *editButton = nullptr;

    if (editMode) {
        saveButton = new QPushButton("Save", &dialog);
        layout.addWidget(saveButton);
    }
    else {
        editButton = new QPushButton("Edit", &dialog);
        layout.addWidget(editButton);
    }

    if (saveButton) {
        QObject::connect(saveButton, &QPushButton::clicked, [&]() {
            QString new_chat_name = nameLineEdit.text();
            QString new_chat_desc = descLineEdit.text();
            if (new_chat_name != chatTitle->text()) {
                if (!manager->new_client.change_chat_name(chat_id, manager->ID, new_chat_name.toStdString())) {
                    QMessageBox::critical(
                        this, "Permission denied", "You are not admin of this chat!"
                    );
                }
                else {
                    // std::string message_text = manager->username + " changed chat name.";
                    // manager->new_client.send_message(-1, chat_id, message_text);

                    chatTitle->setText(new_chat_name);

                    chatList->currentItem()->setData(Qt::UserRole + 2, new_chat_name);
                }
            }
            if (new_chat_desc != descText.text()) {
                if (!manager->new_client.change_description(chat_id, manager->ID, new_chat_desc.toStdString())) {
                    QMessageBox::critical(
                        this, "Permission denied", "You are not admin of this chat!"
                    );
                }
                else {
                    // std::string message_text = manager->username + " changed chat description.";
                    // manager->new_client.send_message(-1, chat_id, message_text);

                    chatList->currentItem()->setData(Qt::UserRole + 4, new_chat_desc);
                }
            }
            dialog.accept();
        });
    }

    if (editButton) {
        QObject::connect(editButton, &QPushButton::clicked, [&]() {
            showChatInfo(true);
            dialog.accept();
        });
    }


    dialog.exec();
}

void Chats::onAddChatButtonClicked()
{
    QDialog dialog;
    dialog.setWindowTitle("Create new chat");

    QLabel nameLabel("Chat name:");
    QLineEdit nameLineEdit;
    QLabel descLabel("Description:");
    QLineEdit descLineEdit;

    QLabel membersLabel("Add members to your chat:");
    QListWidget membersList;
    QMap<QString, uint32_t> userMap;
    std::vector<uint32_t> members = manager->new_client.get_mutual_subscriptions(manager->ID);
    std::vector<uint32_t> chose_members;

    for (uint32_t member : members) {
        QString memberName = QString::fromStdString(manager->new_client.get_user_full_page(member)->preview().login());
        QListWidgetItem *item = new QListWidgetItem(memberName);
        membersList.addItem(item);
        userMap.insert(memberName, member);
    }

    QPushButton createButton("Save");

    QVBoxLayout layout(&dialog);
    layout.addWidget(&nameLabel);
    layout.addWidget(&nameLineEdit);
    layout.addWidget(&descLabel);
    layout.addWidget(&descLineEdit);
    layout.addWidget(&membersLabel);
    layout.addWidget(&membersList);
    layout.addWidget(&createButton);

    QObject::connect(&membersList, &QListWidget::itemClicked, [&](QListWidgetItem *item) {
        QString selectedItemText = item->text();
        uint32_t userId = userMap.value(selectedItemText);
        chose_members.push_back(userId);
        qDebug() << "Selected user ID: " << userId;
    });

    QObject::connect(&createButton, &QPushButton::clicked, [&]() {
        QString name = nameLineEdit.text();
        QString description = descLineEdit.text();
        
        chat_service::ChatPreview *chat = manager->new_client.create_group_chat(manager->ID, name.toStdString(), description.toStdString());

        for (auto member : chose_members) {
            manager->new_client.send_invite(manager->ID, member, chat->chat_id());
        }

        auto *chatPage = new QWidget();
        int index = chatPages->addWidget(chatPage);
        make_chat_item(*chat, index, -1);

        dialog.close();
    });

    dialog.exec();
}

void Chats::onRemoveFromChatReceived(const chat_service::RemoveResponse& response) {
    // get remove data
    int chat_id = response.chat_id();
    int sender_id = response.sender().user_id();
    std::string sender_login = response.sender().login();
    int receiver_id = response.receiver().user_id();
    std::string receiver_login = response.receiver().login();

    qDebug() << "Remove from chat received: " << chat_id << " " << sender_id << " " << receiver_id;

    // std::string message_text;
    // if (sender_id == receiver_id) {
    //     message_text = sender_login + " left chat.";
    // }
    // else {
    //     message_text = sender_login + " removed " + receiver_login + " from chat.";
    // }
    // manager->new_client.send_message(-1, chat_id, message_text);

    if (receiver_id == manager->ID) {
        delete_chat_item(chat_id);
    }
}

void Chats::onInviteRespond(bool accept)
{
    // get current invite
    QListWidgetItem* current_item = inviteList->currentItem();
    int receiver_id               = current_item->data(Qt::UserRole + 3).toInt();
    int invite_id                 = current_item->data(Qt::UserRole).toInt();

    if (invitesData[invite_id].has_chat_preview()) {
        int chat_id                   = invitesData[invite_id].chat_preview().chat_id();
        manager->new_client.respond_invite(invite_id, manager->ID, receiver_id, accept, chat_id);
    }
    else {
        manager->new_client.respond_invite(invite_id, manager->ID, receiver_id, accept);
    }

}

void Chats::onInviteRespondReceived(const invitation::RespondToInvitationResponse& response) {
    // get response data
    int sender_id   = response.sender_id();
    int receiver_id = response.receiver_id();
    int invite_id   = response.invite_id();
    bool accept     = (response.status() == invitation::InviteStatus::ACCEPTED);
    qDebug() << "Invite response received: " << invite_id << " " << accept;

    delete_invite_item(invite_id);

    if (accept) {
        if (response.chat().is_group_chat()) {
            if (sender_id == manager->ID) {
                // std::string message_text = manager->new_client.get_user_full_page(receiver_id)->preview().login() + " added " + manager->username + " to chat.";
                // manager->new_client.send_message(-1, response.chat().chat_id(), message_text);

                auto *chatPage = new QWidget();
                int index = chatPages->addWidget(chatPage);
                
                make_chat_item(response.chat(), index, -1);
            }
            else {
                // std::string message_text = manager->username + " added " + manager->new_client.get_user_full_page(sender_id)->preview().login()+  " to chat.";
                // manager->new_client.send_message(-1, response.chat().chat_id(), message_text);
            }
        }
        else {
            auto *chatPage = new QWidget();
            int index = chatPages->addWidget(chatPage);
            
            make_chat_item(response.chat(), index, -1);
        }
    }
    
}

void Chats::onNewInviteReceived(const invitation::SendInvitationResponse& invite_response) {
    // get invite data
    if (invite_response.has_error_message()) {
        QMessageBox::critical(this, "Error", QString::fromStdString(invite_response.error_message()));
        return;
    }

    qDebug() << "Chat id: " << invite_response.invite().chat_preview().chat_id();

    bool am_i_sender = (invite_response.sender_id() == manager->ID);
    make_invite_item(invite_response.invite(), am_i_sender);
}

void Chats::onNewMessageReceived(const chat_service::MessageReceived& message) {
    // get message data
    QLabel* senderLabel    = new QLabel(QString::fromStdString(message.sender().login()));
    QLabel* textLabel      = new QLabel(QString::fromStdString(message.message_text()));
    QLabel* timestampLabel = new QLabel(QString::fromStdString(message.time_stamp()));
    int chat_id            = message.chat_id();

    chatMessagesData[chat_id].push_back(message);

    // check if current chat
    if (currentChatId == chat_id) {
        make_message_item(message);
    }

    // search item that corresponds to the chat
    QListWidgetItem *current_item;
    for(int i = 0; i < chatList->count(); ++i) {
        QListWidgetItem* item = chatList->item(i);
        if(item->data(Qt::UserRole).toInt() == chat_id) {
            current_item = item;
            break;
        }
    }

    if (current_item == nullptr) return;

    // current chat
    chatsData[chat_id].set_time_stamp(timestampLabel->text().toStdString());
    chat_service::MessageReceived* message_copy = new chat_service::MessageReceived(message);
    chatsData[chat_id].set_allocated_last_message(message_copy);

    make_chat_item(chatsData[chat_id], chatPages->currentIndex(), chatList->row(current_item));
}

void Chats::sendMessage() {
    // get message data
    QString message_text     = messageInput->text().trimmed();

    // check something
    if (message_text.isEmpty()) {
        return;
    }

    if (currentChatId == -1) {
        return;
    }

    messageInput->clear();

    // send message
    manager->new_client.send_message(manager->ID, currentChatId, message_text.toStdString());
}

void Chats::onChatClicked(QListWidgetItem* item)
{
    // get chat data
    currentChatId     = item->data(Qt::UserRole).toInt();
    chatPages->setCurrentIndex(item->data(Qt::UserRole + 1).toInt());
    chatTitle->setText(item->data(Qt::UserRole + 2).toString());
    bool isGroupChat  = item->data(Qt::UserRole + 3).toBool();

    chatInfoButton->setVisible(isGroupChat);
    addMemberToChatButton->setVisible(isGroupChat);
    leaveChatButton->setVisible(isGroupChat);
    generateFilmButton->setVisible(true);
    messageInput->setVisible(true);
    sendButton->setVisible(true);

    // get messages
    if (chatMessagesData[currentChatId].empty()) {
        auto messages = manager->new_client.get_last_messages(manager->ID, currentChatId);
        std::sort(messages.begin(), messages.end(), [](const chat_service::MessageReceived& a, const chat_service::MessageReceived& b) {
            return a.time_stamp() < b.time_stamp();
        });
        chatMessagesData[currentChatId] = messages;
    }

    qDebug() << "Messages count: " << chatMessagesData[currentChatId].size();

    // clear chat page
    if (scrollWidgetLayout != nullptr) {
        while (scrollWidgetLayout->count() != 0) {
            auto item = scrollWidgetLayout->takeAt(0);
            delete item->widget();
            delete item;
        }
    }

    chatPages->currentWidget()->setLayout(chatPageLayout);

    // make messages
    for (const auto& message : chatMessagesData[currentChatId]) {
        make_message_item(message);
    }

    scrollWidget->setLayout(scrollWidgetLayout);
    scrollArea->setWidget(scrollWidget);

    // TODO
    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->update();
}

void Chats::onInviteClicked(QListWidgetItem* item)
{
    // get invite data
    currentInviteId = item->data(Qt::UserRole).toInt();
    invitePages->setCurrentIndex(item->data(Qt::UserRole + 1).toInt());
    inviteTitle->setText(item->data(Qt::UserRole + 2).toString());

    invitation::Invite invite = invitesData[currentInviteId];

    qDebug() << "Chat id: " << invite.chat_preview().chat_id();

    if (invitePageLayout != nullptr) {
        while (invitePageLayout->count() != 0) {
            auto item = invitePageLayout->takeAt(0);
            delete item->widget();
            delete item;
        }
    }

    QFrame *invite_frame = new QFrame();
    invite_frame->setFixedSize(600, 700);
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(10);
    shadowEffect->setYOffset(10);
    shadowEffect->setColor(QColor(136, 136, 136));

    invite_frame->setGraphicsEffect(shadowEffect);
    invite_frame->setStyleSheet("background-color: #FFFFFF; border-radius: 10px;");

    QVBoxLayout *inviteLayout = new QVBoxLayout(invite_frame);

    QLabel* photoLabel = new QLabel(this);
    QLabel* inviteTextLabel = new QLabel(this);
    QLabel* descriptionTitleLabel = new QLabel();
    descriptionTitleLabel->setStyleSheet("font-weight: bold;");
    QLabel* descriptionLabel = new QLabel(this);

    acceptButton = new QPushButton("Accept", this);
    acceptButton->setFixedHeight(50);
    acceptButton->setStyleSheet("background-color: #A18594;");
    acceptButton->setCursor(Qt::PointingHandCursor);

    declineButton = new QPushButton("Decline", this);
    declineButton->setFixedHeight(50);
    declineButton->setStyleSheet("background-color: #E4717A;");
    declineButton->setCursor(Qt::PointingHandCursor);

    connect(acceptButton, &QPushButton::clicked, this, [this]() { onInviteRespond(true); });
    connect(declineButton, &QPushButton::clicked, this, [this]() { onInviteRespond(false); });

    QPixmap photoPixmap(":/img/robertdeniro.jpg");
    QPixmap circlePixmap(photoPixmap.size());
    circlePixmap.fill(Qt::transparent);
    QPainter painter(&circlePixmap);
    QPainterPath path;
    path.addEllipse(circlePixmap.rect());
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, photoPixmap);

    photoLabel->setPixmap(circlePixmap.scaled(300, 300, Qt::KeepAspectRatio));
    photoLabel->setAlignment(Qt::AlignCenter);

    inviteTextLabel->setText(invite.invite_text().c_str());
    if (invite.has_chat_preview()) {
        descriptionTitleLabel->setText("Some information about our chat:");
        descriptionLabel->setText(manager->new_client.get_chat_preview(invite.chat_preview().chat_id())->description().c_str());
    } else {
        descriptionTitleLabel->setText("Some information about me:");
        descriptionLabel->setText(manager->new_client.get_user_full_page(invite.sender().user_id())->about().c_str());
    }

    if (invite.has_chat_preview()) {
        // TODO add members photos
    }

    inviteLayout->addWidget(photoLabel);
    inviteLayout->addWidget(inviteTextLabel);
    inviteLayout->addWidget(descriptionTitleLabel);
    inviteLayout->addWidget(descriptionLabel);

    if (item->data(Qt::UserRole + 3).toInt() == manager->ID) {
        acceptButton->hide();
        declineButton->hide();
        QLabel* inviteStatusLabel = new QLabel("Your invite is under review");
        inviteStatusLabel->setFixedHeight(50);
        inviteStatusLabel->setAlignment(Qt::AlignCenter);
        inviteLayout->addWidget(inviteStatusLabel);
    } else {
        acceptButton->show();
        declineButton->show();
        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(acceptButton);
        buttonLayout->addWidget(declineButton);
        inviteLayout->addLayout(buttonLayout);
    }

    invitePageLayout->addWidget(invite_frame, 0, Qt::AlignCenter);
    invitePages->currentWidget()->setLayout(invitePageLayout);
}

void Chats::make_message_item(const chat_service::MessageReceived& message)
{
    // get message data
    int sender_id           = message.sender().user_id();
    QString sender_username = QString::fromStdString(message.sender().login());
    QString message_text    = QString::fromStdString(message.message_text());
    QString time_stamp      = QString::fromStdString(message.time_stamp());

    bool am_i_sender = (sender_id == manager->ID);

    // make message widget
    messageWidget = new QWidget();
    messageLayout = new QVBoxLayout();

    QLabel *senderLabel = am_i_sender
        ? new QLabel(QString::fromStdString(manager->username) + " (You)")
        : new QLabel(sender_username);

    QLabel *textLabel      = new QLabel(message_text);
    textLabel->setWordWrap(true);
    textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLabel *timestampLabel = new QLabel(time_stamp);

    if (sender_id != -1) {
        messageLayout->addWidget(senderLabel);
    }
    messageLayout->addWidget(textLabel);
    messageLayout->addWidget(timestampLabel);
    messageWidget->setLayout(messageLayout);
    messageWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    if (sender_id == -1) {
        scrollWidgetLayout->addWidget(messageWidget, 1, Qt::AlignCenter);
    }
    else {
        am_i_sender
            ? scrollWidgetLayout->addWidget(messageWidget, 1, Qt::AlignRight)
            : scrollWidgetLayout->addWidget(messageWidget, 1, Qt::AlignLeft);
    }

    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->update();
}

void Chats::make_chat_item(const chat_service::ChatPreview& chat, int chat_page_index, int item_index)
{
    // get chat data
    QString chat_name    = QString::fromStdString(chat.name());
    int chat_id          = chat.chat_id();
    QString time_stamp   = QString::fromStdString(chat.time_stamp());
    QString last_message = QString::fromStdString(chat.last_message().message_text());
    int sender_id        = chat.last_message().sender().user_id();
    bool is_group_chat   = chat.is_group_chat();
    QString description  = QString::fromStdString(chat.description());
    // TODO chat_cover

    if (sender_id == manager->ID) {
        last_message = "(You) " + last_message;
    }

    if (!is_group_chat) {
        for (const auto& member : chat.chat_members()) {
            if (member.user_id() != manager->ID) {
                chat_name = QString::fromStdString(member.login());
            }
        }
    }

    chatsData[chat_id] = chat;

    // make chat widget
    QWidget *chatWidget     = new QWidget();
    chatWidget->setFixedWidth(chatList->width());
    QHBoxLayout *chatLayout = new QHBoxLayout(chatWidget);

    // make text and name layout
    QVBoxLayout *textLayout  = new QVBoxLayout();
    QLabel *chatNameLabel    = new QLabel(chat_name);
    QLabel *lastMessageLabel = new QLabel(last_message);
    textLayout->addWidget(chatNameLabel);
    textLayout->addWidget(lastMessageLabel);

    // make time label
    QLabel *timeLabel = new QLabel(time_stamp);
    chatLayout->addLayout(textLayout);
    chatLayout->addStretch(1);
    chatLayout->addWidget(timeLabel);

    // set chat widget
    QListWidgetItem *item;
    if (item_index == -1) {
        item = new QListWidgetItem();
        item->setSizeHint(chatWidget->sizeHint());
    }
    else {
        item = chatList->takeItem(item_index);
    }
    chatList->insertItem(0, item);
    chatList->setItemWidget(item, chatWidget);
    chatList->setCurrentItem(item);

    // set data 
    item->setData(Qt::UserRole, chat_id);
    item->setData(Qt::UserRole + 1, chat_page_index);
    item->setData(Qt::UserRole + 2, chatNameLabel->text());
    item->setData(Qt::UserRole + 3, is_group_chat);
    item->setData(Qt::UserRole + 4, description);

    chatWidget->setCursor(Qt::PointingHandCursor);
}

void Chats::make_invite_item(const invitation::Invite& invite, bool am_i_sender)
{
    // get invite data
    QString sender_name    = QString::fromStdString(invite.sender().login());
    int invite_id          = invite.invite_id();
    QString invite_text    = QString::fromStdString(invite.invite_text());
    QString time_stamp     = QString::fromStdString(invite.time_stamp());
    int sender_id          = am_i_sender ? manager->ID : invite.sender().user_id();
    int receiver_id        = am_i_sender ? invite.sender().user_id() : manager->ID;

    qDebug() << "Chat id: " << invite.chat_preview().chat_id();

    invitesData[invite_id] = invite;

    qDebug() << "Chat id: " << invitesData[invite_id].chat_preview().chat_id();

    if (am_i_sender) {
        invite_text = "(You) " + invite_text;
    }

    // make invite widget
    QWidget *inviteWidget     = new QWidget();
    inviteWidget->setFixedWidth(inviteList->width());
    QHBoxLayout *inviteLayout = new QHBoxLayout(inviteWidget);

    // make text and name layout
    QVBoxLayout *textLayout  = new QVBoxLayout();
    QLabel *senderNameLabel    = new QLabel(sender_name);
    QLabel *inviteMessageLabel = new QLabel(invite_text);
    textLayout->addWidget(senderNameLabel);
    textLayout->addWidget(inviteMessageLabel);

    // make time label
    QLabel *timeLabel = new QLabel(time_stamp);
    inviteLayout->addLayout(textLayout);
    inviteLayout->addStretch(1);
    inviteLayout->addWidget(timeLabel);

    // set invite widget
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(inviteWidget->sizeHint());
    inviteList->addItem(item);
    inviteList->setItemWidget(item, inviteWidget);
    inviteList->setCurrentItem(item);

    auto *invitePage = new QWidget();
    int index = invitePages->addWidget(invitePage);

    // set data 
    item->setData(Qt::UserRole, invite_id);
    item->setData(Qt::UserRole + 1, index);
    item->setData(Qt::UserRole + 2, senderNameLabel->text());
    item->setData(Qt::UserRole + 3, sender_id);
    item->setData(Qt::UserRole + 4, receiver_id);

    inviteWidget->setCursor(Qt::PointingHandCursor);
}

void Chats::delete_invite_item(int invite_id)
{
    // get current invite
    QListWidgetItem* current_item;
    for(int i = 0; i < inviteList->count(); ++i) {
        QListWidgetItem* item = inviteList->item(i);
        if(item->data(Qt::UserRole).toInt() == invite_id) {
            current_item = item;
            break;
        }
    }

    invitesData.remove(invite_id);

    int item_index                = inviteList->row(current_item);
    int receiver_id               = current_item->data(Qt::UserRole + 3).toInt();
    QString sender_name           = current_item->data(Qt::UserRole + 2).toString();
    int page_index                = current_item->data(Qt::UserRole + 1).toInt();

    // delete invite from list
    QListWidgetItem* item         = inviteList->takeItem(item_index);
    delete item;

    // delete corresponding page
    QWidget* page = invitePages->widget(page_index);
    invitePages->removeWidget(page);

    if (currentInviteId == invite_id) {
        currentInviteId = -1;

        if (invitePageLayout != nullptr) {
            while (invitePageLayout->count() != 0) {
                auto item = invitePageLayout->takeAt(0);
                delete item->widget();
                delete item;
            }
        }

        inviteTitle->setText("");
    }
    
}

void Chats::delete_chat_item(int chat_id)
{
    // get current chat
    QListWidgetItem* current_item;
    for(int i = 0; i < chatList->count(); ++i) {
        QListWidgetItem* item = chatList->item(i);
        if(item->data(Qt::UserRole).toInt() == chat_id) {
            current_item = item;
            break;
        }
    }

    chatsData.remove(chat_id);
    chatMessagesData.remove(chat_id);

    int item_index                = chatList->row(current_item);
    QString chat_name             = current_item->data(Qt::UserRole + 2).toString();
    int page_index                = current_item->data(Qt::UserRole + 1).toInt();
    bool is_group_chat            = current_item->data(Qt::UserRole + 3).toBool();

    // delete chat from list
    QListWidgetItem* item         = chatList->takeItem(item_index);
    delete item;

    // delete corresponding page
    QWidget* page = chatPages->widget(page_index);
    chatPages->removeWidget(page);

    if (currentChatId == chat_id) {
        currentChatId = -1;

        if (scrollWidgetLayout != nullptr) {
            while (scrollWidgetLayout->count() != 0) {
                auto item = scrollWidgetLayout->takeAt(0);
                delete item->widget();
                delete item;
            }
        }

        chatTitle->setText("");
        
        if (is_group_chat) {
            chatInfoButton->hide();
            addMemberToChatButton->hide();
            leaveChatButton->hide();
        }

        generateFilmButton->hide();
        messageInput->hide();
        sendButton->hide();

    }
}

void Chats::updateChatList(const QString &input)
{
    for(int i = 0; i < chatList->count(); i++) {
        QListWidgetItem* item = chatList->item(i);
        bool matches = item->data(Qt::UserRole + 2).toString().contains(input, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

void Chats::updateInviteList(const QString &input)
{
    for(int i = 0; i < inviteList->count(); i++) {
        QListWidgetItem* item = inviteList->item(i);
        bool matches = item->data(Qt::UserRole + 2).toString().contains(input, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

void Chats::build()
{
    // main layout
    mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout); 

    // left layout
    chatList = new QListWidget();
    chatList->setFixedWidth(500);
    chatList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    inviteList = new QListWidget();
    inviteList->setFixedWidth(500);
    inviteList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    inviteList->hide();
    
    // right layout
    chatWidget = new QWidget();
    chatWidgetLayout = new QVBoxLayout();
    chatWidget->setLayout(chatWidgetLayout);
    inviteWidget = new QWidget();
    inviteWidgetLayout = new QVBoxLayout();
    inviteWidget->setLayout(inviteWidgetLayout);
    inviteWidget->hide();
    chatPages = new QStackedWidget();
    invitePages = new QStackedWidget();

    chatTitle = new QLabel();
    chatTitle->setAlignment(Qt::AlignCenter);
    chatTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    inviteTitle = new QLabel();
    inviteTitle->setAlignment(Qt::AlignCenter);
    inviteTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    // input layout
    messageInput = new QLineEdit();
    messageInput->hide();
    sendButton   = new QPushButton("Send");
    sendButton->hide();
    messageInput->setFixedHeight(50);
    sendButton->setFixedHeight(50);
    sendButton->setCursor(Qt::PointingHandCursor);

    search_line_chats = new QLineEdit;
    search_line_chats->setPlaceholderText("Search chats...");
    search_line_chats->setFixedWidth(chatList->width());
    search_line_invites = new QLineEdit;
    search_line_invites->setPlaceholderText("Search invites...");
    search_line_invites->setFixedWidth(inviteList->width());
    search_line_invites->hide();

    toggleButton = new QPushButton();
    toggleButton->setText("Check invites");

    addChatButton = new QPushButton();
    addChatButton->setText("+");

    chatInfoButton = new QPushButton();
    chatInfoButton->setText("?");
    chatInfoButton->hide();
    chatInfoButton->setFixedWidth(50);

    addMemberToChatButton = new QPushButton();
    addMemberToChatButton->setText("+");
    addMemberToChatButton->hide();
    addMemberToChatButton->setFixedWidth(50);

    leaveChatButton = new QPushButton();
    leaveChatButton->setText("X");
    leaveChatButton->hide();
    leaveChatButton->setFixedWidth(50);

    generateFilmButton = new QPushButton();
    generateFilmButton->setText("F");
    generateFilmButton->hide();
    generateFilmButton->setFixedWidth(50);

    toolBarLayout = new QHBoxLayout();
    toolBarLayout->addWidget(chatTitle);
    toolBarLayout->addWidget(chatInfoButton);
    toolBarLayout->addWidget(addMemberToChatButton);
    toolBarLayout->addWidget(leaveChatButton);
    toolBarLayout->addWidget(generateFilmButton);

    inputLayout = new QHBoxLayout();
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    rightLayout = new QStackedWidget();
    chatWidgetLayout->addLayout(toolBarLayout);
    chatWidgetLayout->addWidget(chatPages);
    chatWidgetLayout->addLayout(inputLayout);
    inviteWidgetLayout->addWidget(inviteTitle);
    inviteWidgetLayout->addWidget(invitePages);
    rightLayout->addWidget(chatWidget);
    rightLayout->addWidget(inviteWidget);

    QHBoxLayout *chatButtonsLayout = new QHBoxLayout();
    chatButtonsLayout->addWidget(toggleButton);
    chatButtonsLayout->addWidget(addChatButton);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(search_line_chats);
    leftLayout->addWidget(search_line_invites);
    leftLayout->addLayout(chatButtonsLayout);
    leftLayout->addWidget(chatList);
    leftLayout->addWidget(inviteList);

    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(rightLayout);

    // chat page layout
    chatPageLayout     = new QVBoxLayout();
    scrollArea         = new QScrollArea();
    scrollWidget       = new QWidget();
    scrollWidgetLayout = new QVBoxLayout();

    scrollArea->setWidgetResizable(true);
    chatPageLayout->addWidget(scrollArea);
    scrollWidget->setLayout(scrollWidgetLayout);
    scrollArea->setWidget(scrollWidget);

    // invite page layout
    invitePageLayout = new QVBoxLayout();
    acceptButton = new QPushButton("Accept", this);
    acceptButton->hide();
    declineButton = new QPushButton("Decline", this);
    declineButton->hide();
}

void Chats::connect_slots()
{
    connect(messageInput, &QLineEdit::returnPressed, this, &Chats::sendMessage);
    connect(sendButton, &QPushButton::clicked, this, &Chats::sendMessage);
    connect(chatList, &QListWidget::itemClicked, this, &Chats::onChatClicked);
    connect(inviteList, &QListWidget::itemClicked, this, &Chats::onInviteClicked);
    connect(manager, &Manager::newMessageReceived, this, &Chats::onNewMessageReceived);
    connect(manager, &Manager::NewInviteReceived, this, &Chats::onNewInviteReceived);
    connect(manager, &Manager::InviteRespondReceived, this, &Chats::onInviteRespondReceived);
    connect(manager, &Manager::RemoveFromChatReceived, this, &Chats::onRemoveFromChatReceived);
    connect(acceptButton, &QPushButton::clicked, this, [this]() { onInviteRespond(true); });
    connect(declineButton, &QPushButton::clicked, this, [this]() { onInviteRespond(false); });
    connect(search_line_chats, &QLineEdit::textChanged, this, &Chats::updateChatList);
    connect(search_line_invites, &QLineEdit::textChanged, this, &Chats::updateInviteList);
    connect(addChatButton, &QPushButton::clicked, this, &Chats::onAddChatButtonClicked);
    connect(chatInfoButton, &QPushButton::clicked, this, [this]() { showChatInfo(false); });
    connect(addMemberToChatButton, &QPushButton::clicked, this, &Chats::onAddMemberToChatButtonClicked);
    connect(leaveChatButton, &QPushButton::clicked, this, &Chats::onLeaveChatButtonClicked);
    connect(generateFilmButton, &QPushButton::clicked, this, &Chats::onGenerateFilmButtonClicked);
    connect(toggleButton, &QPushButton::clicked, [this]() {
        bool isInvitesSelected = chatList->isVisible();
        chatList->setVisible(!isInvitesSelected);
        inviteList->setVisible(isInvitesSelected);
        rightLayout->setCurrentIndex(isInvitesSelected);
        toggleButton->setText(!isInvitesSelected ? "Check invites" : "Check chats");
        search_line_chats->setVisible(!isInvitesSelected);
        search_line_invites->setVisible(isInvitesSelected);
    });
}

Chats::~Chats()
{
    delete ui;
}
