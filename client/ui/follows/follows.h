#ifndef FOLLOWS_H
#define FOLLOWS_H

#include "manager.hpp"
#include "otherpage/otherpage.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QStringListModel>
#include <QCompleter>
#include <QSpacerItem>
#include <QDebug>

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

private slots:
    void updateUserList(const QString &input);
    void onUserClicked(const QString &username);

private:
    Ui::Follows *ui;
    Manager *manager;
    QLineEdit *search_line;
    QStringListModel *model;
    QCompleter *completer;
    QMap<QString, int> users_id;
};

#endif // FOLLOWS_H
