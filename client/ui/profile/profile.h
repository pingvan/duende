#ifndef PROFILE_H
#define PROFILE_H

#include "manager.hpp"
#include "actors_searchwindow/actors_searchwindow.h"
#include "movies_searchwindow/movies_searchwindow.h"

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
class Profile;
}

class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(Manager* managerPtr, QWidget *parent = nullptr);
    ~Profile();

private slots:
    void button_edit_clicked();
    void button_save_clicked();
    void plusItem_favActors_clicked();
    void plusItem_watchlist_clicked();

private:
    Ui::Profile *ui;
    Manager *manager;

    QVBoxLayout *leftBox;
    QVBoxLayout *rightBox;
    
    QLabel *label_status;
    QLabel *label_about;
    QLabel *label_username;

    QLineEdit *lineEdit_status;
    QTextEdit *textEdit_about;
    QLineEdit *lineEdit_username;
    QLineEdit *lineEdit_favActors;

    QGraphicsView *view_favActors;
    QGraphicsView *view_watchlist;

    QPushButton *button_edit;
    QPushButton *button_save;

    QGraphicsScene *scene_favActors;
    QGraphicsScene *scene_watchlist;

    QStringList favActorsImages;
    QStringList watchlistImages;
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
