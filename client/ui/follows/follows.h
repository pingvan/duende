#ifndef FOLLOWS_H
#define FOLLOWS_H

#include <QButtonGroup>
#include <QComboBox>
#include <QCompleter>
#include <QDebug>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>
#include "manager.h"
#include "otherpage/otherpage.h"

namespace Ui {
class Follows;
}

class Follows : public QWidget {
    Q_OBJECT

public:
    explicit Follows(Manager *managerPtr, QWidget *parent = nullptr);
    QStringList searchUsers(const QString &input);
    ~Follows();

signals:
    void userClicked(int id);
    void userFromListClicked(int id);

public slots:
    void UpdateSubscribers();
    void UpdateSubscriptions();
    void UpdateBlocked();
    
private slots:
    void updateUserList(const QString &input);
    void onUserClicked(const QString &username);
    void onUserFromListClicked(QListWidgetItem *item);
    void UpdateList(QListWidget &list, const std::vector<uint32_t> &ids);

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
    enum ListType { Subscriptions, Subscribers, Blocked };
    ListType currentList;
};

#endif  // FOLLOWS_H
