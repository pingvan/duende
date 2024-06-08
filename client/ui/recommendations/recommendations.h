#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include <QComboBox>
#include <QCompleter>
#include <QDebug>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSpacerItem>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>
#include "manager.h"
#include "otherpage/otherpage.h"

namespace Ui {
class Recommendations;
}

class Recommendations : public QWidget {
    Q_OBJECT

public:
    explicit Recommendations(Manager *managerPtr, QWidget *parent = nullptr);
    QStringList searchUsers(const QString &input);

    ~Recommendations();

signals:
    void userClicked(int id);
    void userFromListClicked(int id);

private slots:
    void updateUserList(const QString &input);
    void onUserClicked(const QString &username);
    void updateRecommendations();
    void onUserFromListClicked(int id);

private:
    Ui::Recommendations *ui;
    Manager *manager;
    QLineEdit *search_line;
    QStringListModel *model;
    QCompleter *completer;
    QMap<QString, int> users_id;
    QListWidget *usersGallery;
    QListWidget *filmsGallery;
    QGraphicsView *view_users = nullptr;
    QGraphicsView *view_films = nullptr;

    QGraphicsScene *scene_users = nullptr;
    QGraphicsScene *scene_films = nullptr;
    // common::UserFullPage *user;
    // common::FilmFullPage *film;
};

#endif  // RECOMMENDATIONS_H