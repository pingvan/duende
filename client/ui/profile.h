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

private:
    Ui::Profile *ui;
    Manager *manager;
    
    QLabel *label_status;
    QLabel *label_about;
    QLabel *label_username;

    QLineEdit *lineEdit_status;
    QLineEdit *lineEdit_about;
    QLineEdit *lineEdit_username;

    QGraphicsView *view_favActors;
    QGraphicsView *view_watchlist;

    QPushButton *button_edit;
    QPushButton *button_save;
};

#endif // PROFILE_H
