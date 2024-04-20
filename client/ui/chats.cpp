#include "chats.h"
#include "ui_chats.h"
#include "sidebar.h"

#include <grpcpp/grpcpp.h>
#include <QDebug>

Chats::Chats(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chats),
    manager(managerPtr)
{
    ui->setupUi(this);

    qDebug() << "Chats page start\n";

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    
    std::vector<chat_service::ChatPreview> chats = manager->new_client.get_user_chats(manager->ID);

    auto *chatList = new QListWidget();
    chatList->setFixedWidth(350);
    auto *chatMessages = new QTextEdit();
    chatMessages->setReadOnly(true);

    chatPages = new QStackedWidget();

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

        auto *chatPage = new QTextEdit();
        chatPage->setReadOnly(true);
        int index = chatPages->addWidget(chatPage);
        item->setData(Qt::UserRole + 1, index);
    }

    QLineEdit *messageInput = new QLineEdit();
    QPushButton *sendButton = new QPushButton("Send");

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    QVBoxLayout *rightLayout = new QVBoxLayout();
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

void Chats::sendMessage()
{
   QString message = messageInput->text().trimmed();
   if (!message.isEmpty())
   {
    //    chatTextEdit->append("Вы: " + message);
       messageInput->clear();
       // Здесь можно добавить код для отправки сообщения на сервер или в модель данных
   }
}

void Chats::onChatClicked(QListWidgetItem* item) {
    int chatId = item->data(Qt::UserRole).toInt();
    int pageIndex = item->data(Qt::UserRole + 1).toInt();
    chatPages->setCurrentIndex(pageIndex);
    // Затем вы можете использовать chatId для получения сообщений этого чата и отображения их в текущей странице
    // ...
}