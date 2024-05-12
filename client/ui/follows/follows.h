#ifndef FOLLOWS_H
#define FOLLOWS_H

#include "manager.h"
#include "otherpage/otherpage.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QStringListModel>
#include <QCompleter>
#include <QSpacerItem>
#include <QDebug>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QButtonGroup>

namespace Ui {
class Follows;
}

class Follows : public QWidget
{
    Q_OBJECT

public:
    explicit Follows(Manager* managerPtr, QWidget *parent = nullptr);
    QStringList searchUsers(const QString &input);
    ~Follows();

signals:
    void userClicked(int id);
    void subscribersChanged();
    void subscriptionsChanged();
    void blockedChanged();

private slots:
    void updateUserList(const QString &input);
    void onUserClicked(const QString &username);
    void UpdateSubscribers();
    void UpdateSubscriptions();
    void UpdateBlocked();

private:
    Ui::Follows *ui;
    Manager *manager;
    QLineEdit *search_line;
    QListWidget *subscribersList;
    QListWidget *subscriptionsList;
    QListWidget *blockedList;
    QStringListModel *model;
    QCompleter *completer;
    QMap<QString, int> users_id;
};

#endif // FOLLOWS_H
