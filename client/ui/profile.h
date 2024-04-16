#ifndef PROFILE_H
#define PROFILE_H

#include "../client.hpp"
#include "../manager.hpp"

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>


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
    void on_button_edit_clicked();

    void on_button_save_clicked();
    void on_plusItem_favActors_clicked();
    void on_plusItem_watchlist_clicked();

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
