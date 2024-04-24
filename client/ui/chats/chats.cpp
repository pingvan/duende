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
        auto *chatPageLayout = new QVBoxLayout(chatPage);
        auto *chatTextEdit = new QTextEdit();
        chatTextEdit->setReadOnly(true);
        chatPageLayout->addWidget(chatTextEdit);

        int index = chatPages->addWidget(chatPage);
        item->setData(Qt::UserRole + 1, index);

        item->setData(Qt::UserRole + 2, chatNameLabel->text());

        chatWidget->setCursor(Qt::PointingHandCursor);
    }

    messageInput = new QLineEdit();
    sendButton = new QPushButton("Send");
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

    connect(sendButton, &QPushButton::clicked, this, &Chats::sendMessage);
    connect(chatList, &QListWidget::itemClicked, this, &Chats::onChatClicked);

    this->setLayout(mainLayout); 

    qDebug() << "Chats page end\n";
}

Chats::~Chats()
{
    delete ui;
}

void Chats::sendMessage() {
    QString messageText = messageInput->text().trimmed();

    QListWidgetItem* currentItem = chatList->currentItem();
    int itemIndex = chatList->row(currentItem);
    int chatId = currentItem->data(Qt::UserRole).toInt();

    manager->new_client.send_message(manager->ID, chatId, messageText.toStdString());

    messageInput->clear();

    QString time_stamp = "time???";

    QWidget* chatPage = qobject_cast<QWidget*>(chatPages->currentWidget());
    QTextEdit* chatTextEdit = chatPage->findChild<QTextEdit*>();
    chatTextEdit->insertHtml("<div style='text-align: right; color: black;'>" +  QString::fromStdString(manager->new_client.get_user_full_page(manager->ID)->preview().login()) + " (You)" + "</div><br><br>");
    chatTextEdit->insertHtml("<div style='text-align: left; color: black;'>" + messageText + "</div><br><br>");
    chatTextEdit->insertHtml("<div style='text-align: left; color: black;'>" + time_stamp + "</div><br><br>");
    chatTextEdit->insertHtml("<br><br>");

    // currentItem->setSizeHint(chatWidget->sizeHint());
    // currentItem->setData(Qt::UserRole, currentChatId);
    // currentItem->setText(messageText);
    // currentItem->setData(Qt::UserRole + 1, time_stamp);
    // currentItem->setData(Qt::UserRole + 2, chatTitle->text());

    QListWidgetItem *item = chatList->takeItem(itemIndex);
    chatList->insertItem(0, item);
    chatList->setCurrentItem(item);
    chatList->setCursor(Qt::PointingHandCursor);
}


void Chats::onChatClicked(QListWidgetItem* item) {
    int chatId = item->data(Qt::UserRole).toInt();
    int pageIndex = item->data(Qt::UserRole + 1).toInt();
    chatPages->setCurrentIndex(pageIndex);
    std::vector<chat_service::MessageData> messages = manager->new_client.get_last_messages(manager->ID, chatId);
    qDebug() << "Messages count: " << messages.size();

    QWidget* chatPage = qobject_cast<QWidget*>(chatPages->currentWidget());
    QTextEdit* chatTextEdit = chatPage->findChild<QTextEdit*>();
    chatTextEdit->clear();

    chatTitle->setText(item->data(Qt::UserRole + 2).toString());

    authservice::UserFullPage *user = manager->new_client.get_user_full_page(manager->ID);

    for (const auto& message : messages) {
        bool isUserMessage = (message.sender().login().c_str() == user->preview().login().c_str());
        if (isUserMessage) {
            chatTextEdit->insertHtml("<div style='text-align: right; color: blue;'>" + QString::fromStdString(message.sender().login()) + " (You)" + "</div><br><br>");
            chatTextEdit->insertHtml("<div style='text-align: right; color: blue;'>" + QString::fromStdString(message.message_text()) + "</div><br><br>");
            chatTextEdit->insertHtml("<div style='text-align: right; color: blue;'>" + QString::fromStdString(message.time_stamp()) + "</div><br><br>");
            chatTextEdit->insertHtml("<br><br>");
        } else {
            chatTextEdit->insertHtml("<div style='text-align: left; color: black;'>" + QString::fromStdString(message.sender().login()) + "</div><br><br>");
            chatTextEdit->insertHtml("<div style='text-align: left; color: black;'>" + QString::fromStdString(message.message_text()) + "</div><br><br>");
            chatTextEdit->insertHtml("<div style='text-align: left; color: black;'>" + QString::fromStdString(message.time_stamp()) + "</div><br><br>");
            chatTextEdit->insertHtml("<br><br>");
        }
    }
}
