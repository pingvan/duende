#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

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

namespace Ui {
class Recommendations;
}

class Recommendations : public QWidget
{
    Q_OBJECT

public:
    explicit Recommendations(Manager* managerPtr, QWidget *parent = nullptr);
    QStringList searchUsers(const QString &input);
    ~Recommendations();

signals:
    void userClicked(int id);

private slots:
    void updateUserList(const QString &input);
    void onUserClicked(const QString &username);

private:
    Ui::Recommendations *ui;
    Manager *manager;
    QLineEdit *search_line;
    QStringListModel *model;
    QCompleter *completer;
    QMap<QString, int> users_id;
};

#endif // RECOMMENDATIONS_H