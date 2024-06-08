#ifndef PROFILE_H
#define PROFILE_H

#include "manager.h"
#include "actors_searchwindow/actors_searchwindow.h"
#include "movies_searchwindow/movies_searchwindow.h"
#include "genres_searchwindow/genres_searchwindow.h"

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
#include <QGraphicsDropShadowEffect>
#include <QListWidget>
#include <QFileDialog>
#include <QGraphicsProxyWidget>
#include <QImageReader>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

namespace Ui {
class Profile;
}

class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(Manager* managerPtr, QWidget *parent = nullptr);
    ~Profile();
    void build();
    void build_scenes(bool edit_mode = false);

private slots:
    void button_edit_clicked();
    void button_save_clicked();
    void button_edit_genres_clicked();
    void button_edit_actors_clicked();
    // void plusItem_favActors_clicked();
    void plusItem_watchlist_clicked();
    void update_favorite_genres();
    void update_watchlist(int start_films, bool edit_mode);
    // void update_actors(int start_actors, bool edit_mode);
    void update_favorite_actors();
    void onEditButtonClicked() {
        editMode = !editMode;
    }
    void uploadPhoto();

private:
    Ui::Profile *ui;
    Manager *manager;
    common::UserFullPage *user;

    QVBoxLayout *leftBox;
    QVBoxLayout *rightBox;
    
    QLabel *label_status;
    QLabel *label_about;
    QLabel *label_username;
    QLabel *label_photo;
    QLabel *label_favorite_genres;
    QLabel *actors_text = nullptr;
    QLabel *movies_text = nullptr;
    QLabel *genres_text = nullptr;
    QLabel *greeting;

    QLineEdit *lineEdit_status;
    QTextEdit *textEdit_about;
    QLineEdit *lineEdit_username;
    QLineEdit *lineEdit_favActors;

    QGraphicsView *view_favActors = nullptr;
    QGraphicsView *view_watchlist = nullptr;

    QPushButton *button_edit;
    QPushButton *button_save;
    QPushButton *button_edit_genres;
    QPushButton *button_edit_actors;

    QGraphicsScene *scene_favActors = nullptr;
    QGraphicsScene *scene_watchlist = nullptr;

    QStringList favActorsImages;
    QStringList watchlistImages;

    Genres_SearchWindow *genres_searchWindow;
    Actors_SearchWindow *actors_searchWindow;

    QHBoxLayout *genresLayout;
    QListWidget *genresList;
    QHBoxLayout *actorsLayout;
    QListWidget *actorsList;
    bool editMode = false;
    QMetaObject::Connection genres_connection;
    QMetaObject::Connection actors_connection;

    bool eventFilter(QObject *watched, QEvent *event) override;
};

class ClickablePixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit ClickablePixmapItem(const QPixmap &pixmap) : QGraphicsPixmapItem(pixmap) {}

signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        emit clicked();
        QGraphicsPixmapItem::mousePressEvent(event);
    }
};

#endif // PROFILE_H
