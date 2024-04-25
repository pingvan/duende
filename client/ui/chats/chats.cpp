#include "chats.h"
#include "ui_chats.h"


Chats::Chats(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chats)
    , manager(managerPtr)
{   
    ui->setupUi(this);

    qDebug() << "Chats page start\n";

    qDebug() << "ID: " << manager->ID;

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    std::vector<chat_service::ChatPreview> chats = managerPtr->new_client.get_user_chats(manager->ID);

    std::sort(chats.begin(), chats.end(), [](const chat_service::ChatPreview& a, const chat_service::ChatPreview& b) {
        return a.time_stamp() > b.time_stamp();
    });

    qDebug() << "Chats count: " << chats.size();

    chatList = new QListWidget();
    chatList->setFixedWidth(350);
    chatMessages = new QTextEdit();
    chatMessages->setReadOnly(true);

    chatPages = new QStackedWidget();

    chatTitle = new QLabel();
    chatTitle->setAlignment(Qt::AlignCenter);
    chatTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    for (const auto &chat : chats) {
        QWidget *chatWidget = new QWidget();
        QHBoxLayout *chatLayout = new QHBoxLayout(chatWidget);

        QVBoxLayout *textLayout = new QVBoxLayout();
        QLabel *chatNameLabel = new QLabel(QString::fromStdString(chat.name()));
        QLabel *lastMessageLabel = new QLabel(QString::fromStdString(chat.last_message().message_text()));
        textLayout->addWidget(chatNameLabel);
        textLayout->addWidget(lastMessageLabel);

        QLabel *timeLabel = new QLabel(QString::fromStdString(chat.time_stamp()));
        chatLayout->addLayout(textLayout);
        chatLayout->addWidget(timeLabel);

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(chatWidget->sizeHint());
        item->setData(Qt::UserRole, chat.chat_id());
        chatList->addItem(item);
        chatList->setItemWidget(item, chatWidget);

        auto *chatPage = new QWidget();
        int index = chatPages->addWidget(chatPage);
        item->setData(Qt::UserRole + 1, index);

        item->setData(Qt::UserRole + 2, chatNameLabel->text());

        chatWidget->setCursor(Qt::PointingHandCursor);
    }

    messageInput = new QLineEdit();
    messageInput->setFixedHeight(50);
    sendButton = new QPushButton("Send");
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

    connect(messageInput, &QLineEdit::returnPressed, this, &Chats::sendMessage);
    connect(sendButton, &QPushButton::clicked, this, &Chats::sendMessage);
    connect(chatList, &QListWidget::itemClicked, this, &Chats::onChatClicked);
    connect(manager, &Manager::newMessageReceived, this, &Chats::onNewMessageReceived);

    this->setLayout(mainLayout); 

    chatPageLayout = new QVBoxLayout();
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollWidget = new QWidget();
    scrollWidgetLayout = new QVBoxLayout();
    chatPageLayout->addWidget(scrollArea);
    scrollWidget->setLayout(scrollWidgetLayout);
    scrollArea->setWidget(scrollWidget);

    qDebug() << "Chats page end\n";
}

Chats::~Chats()
{
    delete ui;
}

void Chats::onNewMessageReceived(const chat_service::Message& message) {
    // TODO не всегда сообщения приходят, когда ты с человеком в чате. определять это, поднимать чат наверх

    messageWidget = new QWidget();
    messageLayout = new QVBoxLayout();

    // TODO sender name
    QLabel* senderLabel = new QLabel(QString::number(message.sender_id()));
    QLabel* textLabel = new QLabel(QString::fromStdString(message.message_text()));
    QLabel* timestampLabel = new QLabel(QString::fromStdString(message.timestamp_()));

    messageLayout->addWidget(senderLabel);
    messageLayout->addWidget(textLabel);
    messageLayout->addWidget(timestampLabel);
    messageWidget->setLayout(messageLayout);

    scrollWidgetLayout->addWidget(messageWidget, 1, Qt::AlignLeft);

    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
    scrollArea->update();

    // FIX THIS!!
    QList<QListWidgetItem *> items = chatList->findItems(QString::number(message.chat_id()), Qt::MatchExactly);
    if (!items.empty()) {
        QWidget *chatWidget = new QWidget();
        QHBoxLayout *chatLayout = new QHBoxLayout(chatWidget);

        QVBoxLayout *textLayout = new QVBoxLayout();
        // TODO chat name
        QLabel *chatNameLabel = new QLabel("CHAT NAME");
        QLabel *lastMessageLabel = new QLabel(textLabel);
        textLayout->addWidget(chatNameLabel);
        textLayout->addWidget(lastMessageLabel);

        QLabel *timeLabel = new QLabel(timestampLabel);
        chatLayout->addLayout(textLayout);
        chatLayout->addWidget(timeLabel);

        QListWidgetItem *item = items.first();
        int itemIndex = chatList->row(item);
        item = chatList->takeItem(itemIndex);
        chatList->insertItem(0, item);
        chatList->setItemWidget(item, chatWidget);
        chatList->setCurrentItem(item);

        item->setData(Qt::UserRole, message.chat_id());
        item->setData(Qt::UserRole + 1, chatPages->currentIndex());
        // TODO chat name
        item->setData(Qt::UserRole + 2, "CHAT NAME");

        chatWidget->setCursor(Qt::PointingHandCursor);
    }
}

void Chats::sendMessage() {
    QString messageText = messageInput->text().trimmed();

    QListWidgetItem* currentItem = chatList->currentItem();
    int itemIndex = chatList->row(currentItem);
    int chatId = currentItem->data(Qt::UserRole).toInt();

    manager->new_client.send_message(manager->ID, chatId, messageText.toStdString());

    messageInput->clear();

    // TODO timestamp
    QString time_stamp = "2024-04-24 15:00:00";

    messageWidget = new QWidget();
    messageLayout = new QVBoxLayout();

    authservice::UserFullPage *user = manager->new_client.get_user_full_page(manager->ID);

    QLabel* senderLabel = new QLabel(QString::fromStdString(user->preview().login()) + " (You)");
    QLabel* textLabel = new QLabel(messageText);
    QLabel* timestampLabel = new QLabel(time_stamp);

    messageLayout->addWidget(senderLabel);
    messageLayout->addWidget(textLabel);
    messageLayout->addWidget(timestampLabel);
    messageWidget->setLayout(messageLayout);

    scrollWidgetLayout->addWidget(messageWidget, 1, Qt::AlignRight);

    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
    scrollArea->update();

    QWidget *chatWidget = new QWidget();
    QHBoxLayout *chatLayout = new QHBoxLayout(chatWidget);

    QVBoxLayout *textLayout = new QVBoxLayout();
    // TODO chat name
    QLabel *chatNameLabel = new QLabel("CHAT NAME");
    QLabel *lastMessageLabel = new QLabel(messageText);
    textLayout->addWidget(chatNameLabel);
    textLayout->addWidget(lastMessageLabel);

    QLabel *timeLabel = new QLabel(time_stamp);
    chatLayout->addLayout(textLayout);
    chatLayout->addWidget(timeLabel);

    QListWidgetItem *item = chatList->takeItem(itemIndex);
    chatList->insertItem(0, item);
    chatList->setItemWidget(item, chatWidget);
    chatList->setCurrentItem(item);

    item->setData(Qt::UserRole, chatId);
    item->setData(Qt::UserRole + 1, chatPages->currentIndex());
    // TODO chat name
    item->setData(Qt::UserRole + 2, "CHAT NAME");

    chatWidget->setCursor(Qt::PointingHandCursor);
}

void Chats::onChatClicked(QListWidgetItem* item)
{
    int chatId = item->data(Qt::UserRole).toInt();
    auto pageIndex = item->data(Qt::UserRole + 1).toInt();

    auto messages  = manager->new_client.get_last_messages(manager->ID, chatId);
    std::sort(messages.begin(), messages.end(), [](const chat_service::MessageData& a, const chat_service::MessageData& b) {
        return a.time_stamp() < b.time_stamp();
    });
    qDebug() << "Messages count: " << messages.size();

    authservice::UserFullPage *user = manager->new_client.get_user_full_page(manager->ID);

    chatTitle->setText(item->data(Qt::UserRole + 2).toString());
    chatPages->setCurrentIndex(pageIndex);

    if (scrollWidgetLayout != nullptr) {
        while ( scrollWidgetLayout->count() != 0 ) {
               auto item = scrollWidgetLayout->takeAt(0);
               delete item->widget();
               delete item;
        }
    }

    chatPages->currentWidget()->setLayout(chatPageLayout);

    for (const auto& message : messages) {
        bool is_sender = (message.sender().login() == user->preview().login());

        messageWidget  = new QWidget();
        messageLayout  = new QVBoxLayout();

        QLabel *senderLabel = is_sender
            ? new QLabel(QString::fromStdString(message.sender().login()) + " (You)")
            : new QLabel(QString::fromStdString(message.sender().login()));

        QLabel *textLabel      = new QLabel(QString::fromStdString(message.message_text()));
        QLabel *timestampLabel = new QLabel(QString::fromStdString(message.time_stamp()));

        messageLayout->addWidget(senderLabel);
        messageLayout->addWidget(textLabel);
        messageLayout->addWidget(timestampLabel);
        messageWidget->setLayout(messageLayout);

        is_sender
            ? scrollWidgetLayout->addWidget(messageWidget, 1, Qt::AlignRight)
            : scrollWidgetLayout->addWidget(messageWidget, 1, Qt::AlignLeft);

    }

    scrollWidget->setLayout(scrollWidgetLayout);
    scrollArea->setWidget(scrollWidget);

    // TODO
    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
    scrollArea->update();

    qDebug() << "scrollArea->verticalScrollBar()->maximum(): " << scrollArea->verticalScrollBar()->maximum();
    qDebug() << "scrollWidgetLayout->count(): " << scrollWidgetLayout->count();
}

