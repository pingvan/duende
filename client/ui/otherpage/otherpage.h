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

public slots:
    void sendInvite();

private:
    Ui::OtherPage *ui;
    Manager *manager;
    int id;

    QVBoxLayout *leftBox;
    QVBoxLayout *rightBox;
    
    QLabel *label_status;
    QLabel *label_about;
    QLabel *label_username;
    QLabel *label_favorite_genres;

    QGraphicsView *view_favActors;
    QGraphicsView *view_watchlist;

    QGraphicsScene *scene_favActors;
    QGraphicsScene *scene_watchlist;

    QStringList favActorsImages;
    QStringList watchlistImages;
};

#endif // OTHERPAGE_H
