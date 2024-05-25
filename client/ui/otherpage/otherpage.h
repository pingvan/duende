#ifndef OTHERPAGE_H
#define OTHERPAGE_H

#include "manager.h"

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QDebug>
#include <QCompleter>
#include <QStringListModel>
#include <QGraphicsPixmapItem>
#include <QApplication>
#include <QScreen>
#include <QListWidget>
#include <QMessageBox>

namespace Ui {
class OtherPage;
}

class OtherPage : public QWidget
{
    Q_OBJECT

public:
    explicit OtherPage(int id, Manager* managerPtr, QWidget *parent = nullptr);
    ~OtherPage();
    void updatePage(int newId);
    void build();
    void build_scenes();

signals:
    void inviteSent(const invitation::Invite&);
    void subscriptionMade(const common::UserPreview&);

public slots:
    void sendInvite();
    void toggleSubscription();
    void showBlockDialog(QPushButton* blockButton);

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
    QPushButton* subscribeButton;

    QGraphicsView *view_favActors = nullptr;
    QGraphicsView *view_watchlist = nullptr;

    QGraphicsScene *scene_favActors = nullptr;
    QGraphicsScene *scene_watchlist = nullptr;

    QStringList favActorsImages;
    QStringList watchlistImages;

    QListWidget *genresList;

};

#endif // OTHERPAGE_H
