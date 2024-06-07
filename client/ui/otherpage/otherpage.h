#ifndef OTHERPAGE_H
#define OTHERPAGE_H

#include <QApplication>
#include <QCompleter>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QImageReader>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QStringListModel>
#include <QTextEdit>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include "manager.h"

namespace Ui {
class OtherPage;
}

class OtherPage : public QWidget {
    Q_OBJECT

public:
    explicit OtherPage(int id, Manager *managerPtr, QWidget *parent = nullptr);
    ~OtherPage();
    void updatePage(int newId);
    void build();
    void build_scenes();

signals:
    void inviteSent(const invitation::Invite &);
    void subscriptionMade(const common::UserPreview &);

public slots:
    void sendInvite();
    void toggleSubscription();
    void showBlockDialog(QPushButton *blockButton);

private slots:
    void update_watchlist();

private:
    Ui::OtherPage *ui;
    Manager *manager;
    common::UserFullPage *user;
    int id;
    std::string login;

    QVBoxLayout *leftBox;
    QVBoxLayout *rightBox;

    QLabel *label_status;
    QLabel *label_about;
    QLabel *label_username;
    QLabel *label_favorite_genres;
    QLabel *actors_text = nullptr;
    QLabel *movies_text = nullptr;
    QLabel *genres_text = nullptr;
    QPushButton *subscribeButton;

    QGraphicsView *view_favActors = nullptr;
    QGraphicsView *view_watchlist = nullptr;

    QGraphicsScene *scene_favActors = nullptr;
    QGraphicsScene *scene_watchlist = nullptr;

    QStringList favActorsImages;
    QStringList watchlistImages;

    QListWidget *genresList;
    QPushButton *inviteButton;
    QPushButton *blockButton;
};

#endif  // OTHERPAGE_H
