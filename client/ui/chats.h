#ifndef CHATS_H
#define CHATS_H

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
    explicit Chats(QWidget *parent = nullptr);
    ~Chats();

private slots:
    void sendMessage(); // Слот для отправки сообщения

private:
    Ui::Chats *ui;
    QListWidget *chatList; // Список для отображения доступных чатов
    QTextEdit *chatMessages; // Область для отображения сообщений чата
    QLineEdit *messageInput; // Поле ввода нового сообщения
    QPushButton *sendButton; // Кнопка отправки сообщения
};

#endif // CHATS_H
