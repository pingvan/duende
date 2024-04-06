#include "profile.h"
#include "ui_profile.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QDebug>

Profile::Profile(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Profile)
    , manager(managerPtr)
{
    ui->setupUi(this);

    qDebug() << "Profile page start\n";

    authservice::UserFullPage *user = manager->new_client.get_user_full_page(manager->ID);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setColumnMinimumWidth(0, 500);
    mainLayout->setColumnStretch(0, 0);

    QLabel *label_photo = new QLabel();
    QPixmap pixmap("img/tarantino.jpg");
    label_photo->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    mainLayout->addWidget(label_photo, 0, 0);

    label_status = new QLabel(QString::fromStdString(user->quote().c_str()));
    mainLayout->addWidget(label_status, 1, 0);

    label_about = new QLabel(QString::fromStdString(user->about().c_str()));
    mainLayout->addWidget(label_about, 2, 0);

    label_username = new QLabel(QString::fromStdString(user->preview().login().c_str()));
    mainLayout->addWidget(label_username, 0, 1);

    view_favActors = new QGraphicsView();
    QGraphicsScene *scene_favActors = new QGraphicsScene();

    view_favActors->setScene(scene_favActors);
    mainLayout->addWidget(view_favActors, 1, 1);

    view_watchlist = new QGraphicsView();
    QGraphicsScene *scene_watchlist = new QGraphicsScene();

    view_watchlist->setScene(scene_watchlist);
    mainLayout->addWidget(view_watchlist, 2, 1);

    button_edit = new QPushButton("Edit");
    connect(button_edit, &QPushButton::clicked, this, &Profile::on_button_edit_clicked);
    mainLayout->addWidget(button_edit, 3, 0);

    button_save = new QPushButton("Save");
    connect(button_save, &QPushButton::clicked, this, &Profile::on_button_save_clicked);
    button_save->setVisible(false);
    mainLayout->addWidget(button_save, 3, 0);

    this->setLayout(mainLayout);
    qDebug() << "Profile page end\n";
}

Profile::~Profile()
{
    delete ui;
}

void Profile::on_button_edit_clicked()
{
    lineEdit_status = new QLineEdit(label_status->text());
    lineEdit_about = new QLineEdit(label_about->text());
    lineEdit_username = new QLineEdit(label_username->text());

    lineEdit_status->setMaximumWidth(500);
    lineEdit_about->setMaximumWidth(500);
    lineEdit_username->setMaximumWidth(500);

    layout()->replaceWidget(label_status, lineEdit_status);
    layout()->replaceWidget(label_about, lineEdit_about);
    layout()->replaceWidget(label_username, lineEdit_username);

    view_favActors->setInteractive(true);
    view_watchlist->setInteractive(true);

    button_save->setVisible(true);

    button_edit->setVisible(false);
}

void Profile::on_button_save_clicked()
{   
    manager->new_client.change_username(manager->ID, lineEdit_username->text().toStdString());
    manager->new_client.change_quote(manager->ID, lineEdit_status->text().toStdString());
    manager->new_client.change_bio(manager->ID, lineEdit_about->text().toStdString());

    label_status->setText(lineEdit_status->text());
    label_about->setText(lineEdit_about->text());
    label_username->setText(lineEdit_username->text());

    layout()->replaceWidget(lineEdit_status, label_status);
    layout()->replaceWidget(lineEdit_about, label_about);
    layout()->replaceWidget(lineEdit_username, label_username);

    lineEdit_status->hide();
    lineEdit_about->hide();
    lineEdit_username->hide();

    view_favActors->setInteractive(false);
    view_watchlist->setInteractive(false);

    button_save->setVisible(false);

    button_edit->setVisible(true);
}