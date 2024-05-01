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

    qDebug() << "Chats size: " << chats.size();
    qDebug() << "Received invites size: " << received_invites.size();
    qDebug() << "Sent invites size: " << sent_invites.size();

    // deal with invites
    for (const auto &invite : received_invites) {
        auto *invitePage = new QWidget();
        int index = invitePages->addWidget(invitePage);
        make_invite_item(invite, index);
    }

    for (const auto &invite : sent_invites) {
        auto *invitePage = new QWidget();
        int index = invitePages->addWidget(invitePage);
        make_invite_item(invite, index);
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
    int item_index                = inviteList->row(current_item);
    QString sender_name           = current_item->data(Qt::UserRole + 2).toString();
    int page_index                = current_item->data(Qt::UserRole + 1).toInt();
    int invite_id                 = current_item->data(Qt::UserRole).toInt();
    int receiver_id               = 1; //TODO

    // delete invite from list
    QListWidgetItem* item         = inviteList->takeItem(item_index);
    delete item;

    // delete corresponding page
    QWidget* page = invitePages->widget(page_index);
    invitePages->removeWidget(page);
    currentInviteId = -1;

    if (invitePageLayout != nullptr) {
        while (invitePageLayout->count() != 0) {
            auto item = invitePageLayout->takeAt(0);
            delete item->widget();
            delete item;
        }
    }

    inviteTitle->setText("");

    manager->new_client.respond_invite(invite_id, receiver_id, accept);
}

void Chats::onInviteRespondReceived(const invitation::RespondToInvitationResponse&) {
    qDebug() << "New invite respond received";
}

void Chats::onNewInviteSent(const invitation::Invite& invite)
{
    make_invite_item(invite, invitePages->currentIndex());
}

void Chats::onNewInviteReceived(const invitation::SendInvitationResponse& invite) {
    qDebug() << "New invite received";
    make_invite_item(invite.invite(), invitePages->currentIndex());
}

void Chats::onNewMessageReceived(const chat_service::Message& message) {
    qDebug() << "New message received : " << QString::fromStdString(message.message_text());
    qDebug() << "New message received time_stamp: " << QString::fromStdString(message.timestamp_());
    qDebug() << "New message received sender: " << QString::number(message.sender_id());

    // TODO sender name
    QLabel* senderLabel    = new QLabel(QString::number(message.sender_id()));
    QLabel* textLabel      = new QLabel(QString::fromStdString(message.message_text()));
    QLabel* timestampLabel = new QLabel(QString::fromStdString(message.timestamp_()));

    // current message
    chat_service::MessageData *current_message = new chat_service::MessageData();
    authservice::UserPreview *sender = new authservice::UserPreview();
    sender->set_login(senderLabel->text().toStdString());
    current_message->set_allocated_sender(sender);
    current_message->set_message_text(message.message_text());
    current_message->set_time_stamp(message.timestamp_());

    // check if current chat
    if (currentChatId == message.chat_id()) {
        make_message_item(*current_message);
    }

    // search item that corresponds to the chat
    QListWidgetItem *current_item;
    for(int i = 0; i < chatList->count(); ++i) {
        QListWidgetItem* item = chatList->item(i);
        if(item->data(Qt::UserRole).toInt() == message.chat_id()) {
            current_item = item;
            break;
        }
    }

    if (current_item == nullptr) return;

    // current chat
    chat_service::ChatPreview current_chat;
    current_chat.set_chat_id(message.chat_id());
    current_chat.set_name(current_item->data(Qt::UserRole + 2).toString().toStdString());
    current_chat.set_time_stamp(timestampLabel->text().toStdString());
    current_chat.set_allocated_last_message(current_message);

    make_chat_item(current_chat, chatPages->currentIndex(), chatList->row(current_item));
}

void Chats::sendMessage() {
    // get message data
    QString message_text     = messageInput->text().trimmed();
    QString sender_username  = QString::fromStdString(manager->username);
    // TODO timestamp
    QString time_stamp       = "2024-04-24 15:00:00";

    // check something
    if (message_text.isEmpty()) {
        return;
    }

    if (currentChatId == -1) {
        return;
    }

    messageInput->clear();

    // get current chat
    QListWidgetItem* current_item = chatList->currentItem();
    int item_index                = chatList->row(current_item);
    QString chat_name             = current_item->data(Qt::UserRole + 2).toString();

    // send message
    manager->new_client.send_message(manager->ID, currentChatId, message_text.toStdString());

    // make last message instance
    chat_service::MessageData *last_message = new chat_service::MessageData();
    last_message->set_message_text(message_text.toStdString());

    // make current chat instance
    chat_service::ChatPreview current_chat;
    current_chat.set_chat_id(currentChatId);
    current_chat.set_name(chat_name.toStdString());
    current_chat.set_time_stamp(time_stamp.toStdString());
    current_chat.set_allocated_last_message(last_message);

    // make message instance
    chat_service::MessageData message;
    authservice::UserPreview *sender = new authservice::UserPreview();
    sender->set_login(sender_username.toStdString());
    message.set_allocated_sender(sender);
    message.set_message_text(message_text.toStdString());
    message.set_time_stamp(time_stamp.toStdString());

    make_message_item(message);
    make_chat_item(current_chat, chatPages->currentIndex(), item_index);
}

void Chats::onChatClicked(QListWidgetItem* item)
{
    // get chat data
    currentChatId = item->data(Qt::UserRole).toInt();
    chatPages->setCurrentIndex(item->data(Qt::UserRole + 1).toInt());
    chatTitle->setText(item->data(Qt::UserRole + 2).toString());

    // get messages
    auto messages  = manager->new_client.get_last_messages(manager->ID, currentChatId);
    std::sort(messages.begin(), messages.end(), [](const chat_service::MessageData& a, const chat_service::MessageData& b) {
        return a.time_stamp() < b.time_stamp();
    });

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
    for (const auto& message : messages) {
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
    acceptButton->show();
    acceptButton->setFixedHeight(50);
    declineButton = new QPushButton("Decline", this);
    declineButton->show();
    declineButton->setFixedHeight(50);
    acceptButton->setStyleSheet("background-color: #A18594;");
    declineButton->setStyleSheet("background-color: #E4717A;");
    acceptButton->setCursor(Qt::PointingHandCursor);
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

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(acceptButton);
    buttonLayout->addWidget(declineButton);

    inviteLayout->addLayout(buttonLayout);

    invitePageLayout->addWidget(invite, 0, Qt::AlignCenter);
    invitePages->currentWidget()->setLayout(invitePageLayout);
}

void Chats::make_message_item(const chat_service::MessageData& message)
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

void Chats::make_invite_item(const invitation::Invite& invite, int invite_page_index)
{
    // get chat data
    QString sender_name    = QString::fromStdString(invite.sender().login());
    int invite_id          = invite.invite_id();
    QString invite_text    = QString::fromStdString(invite.invite_text());
    QString time_stamp     = "2024-04-24 15:00:00"; //TODO

    // make chat widget
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

    // set chat widget
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(inviteWidget->sizeHint());
    inviteList->addItem(item);
    inviteList->setItemWidget(item, inviteWidget);
    inviteList->setCurrentItem(item);

    // set data 
    item->setData(Qt::UserRole, invite_id);
    item->setData(Qt::UserRole + 1, invite_page_index);
    item->setData(Qt::UserRole + 2, senderNameLabel->text());

    inviteWidget->setCursor(Qt::PointingHandCursor);
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

    toggleButton = new QPushButton();
    toggleButton->setText("Check invites");
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
    });

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
    connect(manager, &Manager::NewInviteSent, this, &Chats::onNewInviteSent);
    connect(acceptButton, &QPushButton::clicked, this, [this]() { onInviteRespond(true); });
    connect(declineButton, &QPushButton::clicked, this, [this]() { onInviteRespond(false); });
}

Chats::~Chats()
{
    delete ui;
}