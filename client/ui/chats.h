#ifndef CHATS_H
#define CHATS_H

#include "../client.hpp"
#include "../manager.hpp"

#include <QWidget>
#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
class Chats;
}

class Chats : public QWidget
{
    Q_OBJECT

public:
    explicit Chats(Manager* managerPtr, QWidget *parent = nullptr);
    ~Chats();

private slots:
    void sendMessage(); // Слот для отправки сообщения
    void onChatClicked(QListWidgetItem* item);

private:
    Ui::Chats *ui;
    Manager *manager;
    QStackedWidget *chatPages; // Виджет для отображения страниц чатов
    QListWidget *chatList; // Список для отображения доступных чатов
    QTextEdit *chatMessages; // Область для отображения сообщений чата
    QLineEdit *messageInput; // Поле ввода нового сообщения
    QPushButton *sendButton; // Кнопка отправки сообщения
};

#endif // CHATS_H
