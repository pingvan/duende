#include "chats.h"
#include "ui_chats.h"


Chats::Chats(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chats)
    , manager(managerPtr)
{   
    ui->setupUi(this);

    qDebug() << "Chats page start\n";

    // build layout
    build();

    // user chats and invites
    std::vector<chat_service::ChatPreview> chats = manager->new_client.get_user_chats(manager->ID);
    std::vector<invitation::Invite> received_invites = manager->new_client.get_received_invites(manager->ID);
    std::vector<invitation::Invite> sent_invites = manager->new_client.get_sent_invites(manager->ID);
    std::sort(chats.begin(), chats.end(), [](const chat_service::ChatPreview& a, const chat_service::ChatPreview& b) {
        return a.time_stamp() > b.time_stamp();
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

void Chats::onInviteRespond(bool accept)
{
    // get current invite
    QListWidgetItem* current_item = inviteList->currentItem();
    int receiver_id               = current_item->data(Qt::UserRole + 3).toInt();
    int invite_id                 = current_item->data(Qt::UserRole).toInt();

    // delete corresponding page
    delete_invite_item(invite_id);

    manager->new_client.respond_invite(invite_id, receiver_id, accept);
}

void Chats::onInviteRespondReceived(const invitation::RespondToInvitationResponse& response) {
    // get response data
    int invite_id = response.invite_id();
    bool accept   = (response.status() == invitation::InviteStatus::ACCEPTED);

    delete_invite_item(invite_id);
    // TODO add chat
}

void Chats::onNewInviteReceived(const invitation::SendInvitationResponse& invite_response) {
    // get invite data
    bool am_i_sender = (invite_response.sender().user_id() == manager->ID);
    invitation::Invite *invite = new invitation::Invite();
    am_i_sender ? invite->set_allocated_sender(new common::UserPreview(invite_response.receiver())) : invite->set_allocated_sender(new common::UserPreview(invite_response.sender()));
    invite->set_invite_id(invite_response.invite_id());
    invite->set_invite_text(invite_response.invite_text());
    make_invite_item(*invite, am_i_sender);
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
    chat_service::ChatPreview current_chat;
    current_chat.set_chat_id(chat_id);
    current_chat.set_name(current_item->data(Qt::UserRole + 2).toString().toStdString());
    current_chat.set_time_stamp(timestampLabel->text().toStdString());
    chat_service::MessageReceived* message_copy = new chat_service::MessageReceived(message);
    current_chat.set_allocated_last_message(message_copy);

    make_chat_item(current_chat, chatPages->currentIndex(), chatList->row(current_item));
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
    currentChatId = item->data(Qt::UserRole).toInt();
    chatPages->setCurrentIndex(item->data(Qt::UserRole + 1).toInt());
    chatTitle->setText(item->data(Qt::UserRole + 2).toString());

    // get messages
    if (chatMessagesData[currentChatId].empty()) {
        auto messages = manager->new_client.get_last_messages(manager->ID, currentChatId);
        std::sort(messages.begin(), messages.end(), [](const chat_service::MessageReceived& a, const chat_service::MessageReceived& b) {
            return a.time_stamp() < b.time_stamp();
        });
        chatMessagesData[currentChatId] = messages;
    }

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

    if (invitePageLayout != nullptr) {
        while (invitePageLayout->count() != 0) {
            auto item = invitePageLayout->takeAt(0);
            delete item->widget();
            delete item;
        }
    }

    QFrame *invite = new QFrame();
    invite->setFixedSize(600, 700);
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(10);
    shadowEffect->setYOffset(10);
    shadowEffect->setColor(QColor(136, 136, 136));

    invite->setGraphicsEffect(shadowEffect);
    invite->setStyleSheet("background-color: #FFFFFF; border-radius: 10px;");

    QVBoxLayout *inviteLayout = new QVBoxLayout(invite);

    QLabel* photoLabel = new QLabel(this);
    QLabel* inviteTextLabel = new QLabel(this);
    QLabel* descriptionTitleLabel = new QLabel("Some information about me:", this);
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

    inviteTextLabel->setText("Invite text");
    descriptionLabel->setText("Short description");

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

    invitePageLayout->addWidget(invite, 0, Qt::AlignCenter);
    invitePages->currentWidget()->setLayout(invitePageLayout);
}

void Chats::make_message_item(const chat_service::MessageReceived& message)
{
    // get message data
    QString sender_username = QString::fromStdString(message.sender().login());
    QString my_username     = QString::fromStdString(manager->username);
    QString message_text    = QString::fromStdString(message.message_text());
    QString time_stamp      = QString::fromStdString(message.time_stamp());

    bool am_i_sender = (sender_username == my_username);

    // make message widget
    messageWidget = new QWidget();
    messageLayout = new QVBoxLayout();

    QLabel *senderLabel = am_i_sender
        ? new QLabel(my_username + " (You)")
        : new QLabel(sender_username);

    QLabel *textLabel      = new QLabel(message_text);
    textLabel->setWordWrap(true);
    textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLabel *timestampLabel = new QLabel(time_stamp);

    messageLayout->addWidget(senderLabel);
    messageLayout->addWidget(textLabel);
    messageLayout->addWidget(timestampLabel);
    messageWidget->setLayout(messageLayout);
    messageWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    am_i_sender
        ? scrollWidgetLayout->addWidget(messageWidget, 1, Qt::AlignRight)
        : scrollWidgetLayout->addWidget(messageWidget, 1, Qt::AlignLeft);

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

    if (sender_id == manager->ID) {
        last_message = "(You) " + last_message;
    }

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
        chatList->addItem(item);
    }
    else {
        item = chatList->takeItem(item_index);
        chatList->insertItem(0, item);
    }
    chatList->setItemWidget(item, chatWidget);
    chatList->setCurrentItem(item);

    // set data 
    item->setData(Qt::UserRole, chat_id);
    item->setData(Qt::UserRole + 1, chat_page_index);
    item->setData(Qt::UserRole + 2, chatNameLabel->text());

    chatWidget->setCursor(Qt::PointingHandCursor);
}

void Chats::make_invite_item(const invitation::Invite& invite, bool am_i_sender)
{
    // get invite data
    QString sender_name    = QString::fromStdString(invite.sender().login());
    int invite_id          = invite.invite_id();
    QString invite_text    = QString::fromStdString(invite.invite_text());
    QString time_stamp     = "2024-04-24 15:00:00"; //TODO
    int sender_id          = am_i_sender ? manager->ID : invite.sender().user_id();
    int receiver_id        = am_i_sender ? invite.sender().user_id() : manager->ID;

    if (am_i_sender) {
        invite_text = "(You) " + invite_text;
    }

    qDebug() << sender_name;
    qDebug() << invite_text;

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
    chatPages = new QStackedWidget();
    invitePages = new QStackedWidget();
    invitePages->hide();

    chatTitle = new QLabel();
    chatTitle->setAlignment(Qt::AlignCenter);
    chatTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    inviteTitle = new QLabel();
    inviteTitle->setAlignment(Qt::AlignCenter);
    inviteTitle->setStyleSheet("font-size: 18px; font-weight: bold;");
    inviteTitle->hide();

    // input layout
    messageInput = new QLineEdit();
    sendButton   = new QPushButton("Send");
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

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(chatTitle);
    rightLayout->addWidget(inviteTitle);
    rightLayout->addWidget(chatPages);
    rightLayout->addWidget(invitePages);
    rightLayout->addLayout(inputLayout);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(search_line_chats);
    leftLayout->addWidget(search_line_invites);
    leftLayout->addWidget(toggleButton);
    leftLayout->addWidget(chatList);
    leftLayout->addWidget(inviteList);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

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
    connect(acceptButton, &QPushButton::clicked, this, [this]() { onInviteRespond(true); });
    connect(declineButton, &QPushButton::clicked, this, [this]() { onInviteRespond(false); });
    connect(search_line_chats, &QLineEdit::textChanged, this, &Chats::updateChatList);
    connect(search_line_invites, &QLineEdit::textChanged, this, &Chats::updateInviteList);
    connect(toggleButton, &QPushButton::clicked, [this]() {
        bool isChatsVisible = chatList->isVisible();
        chatList->setVisible(!isChatsVisible);
        chatTitle->setVisible(!isChatsVisible);
        chatPages->setVisible(!isChatsVisible);
        messageInput->setVisible(!isChatsVisible);
        sendButton->setVisible(!isChatsVisible);
        inviteList->setVisible(isChatsVisible);
        inviteTitle->setVisible(isChatsVisible);
        invitePages->setVisible(isChatsVisible);
        toggleButton->setText(!isChatsVisible ? "Check invites" : "Check chats");
        search_line_chats->setVisible(!isChatsVisible);
        search_line_invites->setVisible(isChatsVisible);
    });
}

Chats::~Chats()
{
    delete ui;
}