#include "chats.h"
#include "ui_chats.h"


Chats::Chats(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chats)
    , manager(managerPtr)
{   
    ui->setupUi(this);

    qDebug() << "Chats page start\n";

    build();

    std::vector<chat_service::ChatPreview> chats = manager->new_client.get_user_chats(manager->ID);
    std::sort(chats.begin(), chats.end(), [](const chat_service::ChatPreview& a, const chat_service::ChatPreview& b) {
        return a.time_stamp() > b.time_stamp();
    });

    for (const auto &chat : chats) {
        auto *chatPage = new QWidget();
        int index = chatPages->addWidget(chatPage);
        make_chat_item(chat, index, -1);
    }

    connect_slots();

    qDebug() << "Chats page end\n";
}

void Chats::onInviteRespondReceived() {
    qDebug() << "New invite respond received";
}

void Chats::onNewInviteSended() {
    qDebug() << "New invite sended";
}

void Chats::onNewMessageReceived(const chat_service::Message& message) {
    qDebug() << "New message received : " << QString::fromStdString(message.message_text());
    qDebug() << "New message received time_stamp: " << QString::fromStdString(message.timestamp_());
    qDebug() << "New message received sender: " << QString::number(message.sender_id());

    // TODO sender name
    QLabel* senderLabel    = new QLabel(QString::number(message.sender_id()));
    QLabel* textLabel      = new QLabel(QString::fromStdString(message.message_text()));
    QLabel* timestampLabel = new QLabel(QString::fromStdString(message.timestamp_()));

    chat_service::MessageData *current_message = new chat_service::MessageData();
    authservice::UserPreview *sender = new authservice::UserPreview();
    sender->set_login(senderLabel->text().toStdString());
    current_message->set_allocated_sender(sender);
    current_message->set_message_text(message.message_text());
    current_message->set_time_stamp(message.timestamp_());

    if (currentChatId == message.chat_id()) {
        make_message_item(*current_message);
    }

    QListWidgetItem *current_item;
    for(int i = 0; i < chatList->count(); ++i) {
        QListWidgetItem* item = chatList->item(i);
        if(item->data(Qt::UserRole).toInt() == message.chat_id()) {
            current_item = item;
            break;
        }
    }

    if (current_item == nullptr) return;

    chat_service::ChatPreview current_chat;
    current_chat.set_chat_id(message.chat_id());
    current_chat.set_name(current_item->data(Qt::UserRole + 2).toString().toStdString());
    current_chat.set_time_stamp(timestampLabel->text().toStdString());
    current_chat.set_allocated_last_message(current_message);

    make_chat_item(current_chat, chatPages->currentIndex(), chatList->row(current_item));
}

void Chats::sendMessage() {
    QString message_text     = messageInput->text().trimmed();
    QString sender_username  = QString::fromStdString(manager->username);
    // TODO timestamp
    QString time_stamp       = "2024-04-24 15:00:00";

    if (message_text.isEmpty()) {
        return;
    }

    if (currentChatId == -1) {
        return;
    }

    messageInput->clear();

    QListWidgetItem* current_item = chatList->currentItem();
    int item_index                = chatList->row(current_item);
    QString chat_name             = current_item->data(Qt::UserRole + 2).toString();

    manager->new_client.send_message(manager->ID, currentChatId, message_text.toStdString());

    chat_service::MessageData *last_message = new chat_service::MessageData();
    last_message->set_message_text(message_text.toStdString());

    chat_service::ChatPreview current_chat;
    current_chat.set_chat_id(currentChatId);
    current_chat.set_name(chat_name.toStdString());
    current_chat.set_time_stamp(time_stamp.toStdString());
    current_chat.set_allocated_last_message(last_message);

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
    currentChatId = item->data(Qt::UserRole).toInt();
    chatPages->setCurrentIndex(item->data(Qt::UserRole + 1).toInt());
    chatTitle->setText(item->data(Qt::UserRole + 2).toString());

    auto messages  = manager->new_client.get_last_messages(manager->ID, currentChatId);
    std::sort(messages.begin(), messages.end(), [](const chat_service::MessageData& a, const chat_service::MessageData& b) {
        return a.time_stamp() < b.time_stamp();
    });

    if (scrollWidgetLayout != nullptr) {
        while ( scrollWidgetLayout->count() != 0 ) {
               auto item = scrollWidgetLayout->takeAt(0);
               delete item->widget();
               delete item;
        }
    }

    chatPages->currentWidget()->setLayout(chatPageLayout);

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

void Chats::make_message_item(const chat_service::MessageData& message)
{
    QString sender_username = QString::fromStdString(message.sender().login());
    QString my_username     = QString::fromStdString(manager->username);
    QString message_text    = QString::fromStdString(message.message_text());
    QString time_stamp      = QString::fromStdString(message.time_stamp());

    bool am_i_sender = (sender_username == my_username);

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
    QString chat_name    = QString::fromStdString(chat.name());
    int chat_id          = chat.chat_id();
    QString time_stamp   = QString::fromStdString(chat.time_stamp());
    QString last_message = QString::fromStdString(chat.last_message().message_text());

    QWidget *chatWidget     = new QWidget();
    chatWidget->setFixedWidth(chatList->width());
    QHBoxLayout *chatLayout = new QHBoxLayout(chatWidget);

    QVBoxLayout *textLayout  = new QVBoxLayout();
    QLabel *chatNameLabel    = new QLabel(chat_name);
    QLabel *lastMessageLabel = new QLabel(last_message);
    textLayout->addWidget(chatNameLabel);
    textLayout->addWidget(lastMessageLabel);

    QLabel *timeLabel = new QLabel(time_stamp);
    chatLayout->addLayout(textLayout);
    chatLayout->addStretch(1);
    chatLayout->addWidget(timeLabel);

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

    item->setData(Qt::UserRole, chat_id);
    item->setData(Qt::UserRole + 1, chat_page_index);
    item->setData(Qt::UserRole + 2, chatNameLabel->text());

    chatWidget->setCursor(Qt::PointingHandCursor);
}

void Chats::build()
{
    mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout); 

    chatList = new QListWidget();
    chatList->setFixedWidth(500);
    chatList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    chatPages = new QStackedWidget();

    chatTitle = new QLabel();
    chatTitle->setAlignment(Qt::AlignCenter);
    chatTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    messageInput = new QLineEdit();
    sendButton   = new QPushButton("Send");
    messageInput->setFixedHeight(50);
    sendButton->setFixedHeight(50);
    sendButton->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(chatTitle);
    rightLayout->addWidget(chatPages);
    rightLayout->addLayout(inputLayout);

    mainLayout->addWidget(chatList);
    mainLayout->addLayout(rightLayout);

    chatPageLayout     = new QVBoxLayout();
    scrollArea         = new QScrollArea();
    scrollWidget       = new QWidget();
    scrollWidgetLayout = new QVBoxLayout();

    scrollArea->setWidgetResizable(true);
    chatPageLayout->addWidget(scrollArea);
    scrollWidget->setLayout(scrollWidgetLayout);
    scrollArea->setWidget(scrollWidget);
}

void Chats::connect_slots()
{
    connect(messageInput, &QLineEdit::returnPressed, this, &Chats::sendMessage);
    connect(sendButton, &QPushButton::clicked, this, &Chats::sendMessage);
    connect(chatList, &QListWidget::itemClicked, this, &Chats::onChatClicked);
    connect(manager, &Manager::newMessageReceived, this, &Chats::onNewMessageReceived);
    connect(manager, &Manager::NewInviteSended, this, &Chats::onNewInviteSended);
    connect(manager, &Manager::InviteRespondReceived, this, &Chats::onInviteRespondReceived);
}

Chats::~Chats()
{
    delete ui;
}