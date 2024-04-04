#include "chats.h"
#include "ui_chats.h"
#include "sidebar.h"

Chats::Chats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chats)
{
    ui->setupUi(this);

    QHBoxLayout *mainLayout = new QHBoxLayout(this); // Измените QVBoxLayout на QHBoxLayout

    auto *chatList = new QListWidget(); // Список для отображения чатов
    chatList->setFixedWidth(350);
    auto *chatMessages = new QTextEdit(); // Область для отображения сообщений
    chatMessages->setReadOnly(true);

    QLineEdit *messageInput = new QLineEdit(); // Поле ввода сообщения
    QPushButton *sendButton = new QPushButton("Отправить");

    // Слой для поля ввода и кнопки отправки
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    QVBoxLayout *rightLayout = new QVBoxLayout(); // Создайте новый QVBoxLayout для правой части
    rightLayout->addWidget(chatMessages);
    rightLayout->addLayout(inputLayout);

    mainLayout->addWidget(chatList); // Это будет слева
    mainLayout->addLayout(rightLayout); // Это будет справа

    connect(sendButton, &QPushButton::clicked, this, &Chats::sendMessage);
    // Здесь может быть код для подключения сигналов выбора чата из списка и т.д.
}

Chats::~Chats()
{
    delete ui;
}

void Chats::sendMessage()
{
//    QString message = messageLineEdit->text().trimmed();
//    if (!message.isEmpty())
//    {
//        chatTextEdit->append("Вы: " + message);
//        messageLineEdit->clear();
//        // Здесь можно добавить код для отправки сообщения на сервер или в модель данных
//    }
}
