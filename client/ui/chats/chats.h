#ifndef CHATS_H
#define CHATS_H

#include "manager.hpp"

#include <grpcpp/grpcpp.h>
#include <QDebug>
#include <QWidget>
#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtConcurrent/QtConcurrent>
#include <QLabel>
#include <QThread>

namespace Ui {
class Chats;
}

class Chats : public QWidget
{
    Q_OBJECT

public:
    explicit Chats(Manager* managerPtr, QWidget *parent = nullptr);
    ~Chats();

signals:
    void messageReceived(const QString& message);

private slots:
    void sendMessage();
    void onChatClicked(QListWidgetItem* item);

private:
    Ui::Chats *ui;
    Manager *manager;
    int currentChatId;
    QStackedWidget *chatPages;
    QListWidget *chatList;
    QTextEdit *chatMessages;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QLabel *chatTitle;
};

#endif // CHATS_H
