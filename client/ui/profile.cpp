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

Profile::Profile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Profile)
{
    ui->setupUi(this);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setColumnMinimumWidth(0, 500);
    mainLayout->setColumnStretch(0, 0);

    QLabel *label_photo = new QLabel();
    QPixmap pixmap("img/tarantino.jpg");
    label_photo->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    mainLayout->addWidget(label_photo, 0, 0);

    label_status = new QLabel("this is my status");
    mainLayout->addWidget(label_status, 1, 0);

    label_about = new QLabel("there are some information about me");
    mainLayout->addWidget(label_about, 2, 0);

    label_username = new QLabel("quentin");
    mainLayout->addWidget(label_username, 0, 1);

    view_favActors = new QGraphicsView();
    QGraphicsScene *scene_favActors = new QGraphicsScene();
    // Добавьте ваши фотографии актеров в scene_favActors
    view_favActors->setScene(scene_favActors);
    mainLayout->addWidget(view_favActors, 1, 1);

    view_watchlist = new QGraphicsView();
    QGraphicsScene *scene_watchlist = new QGraphicsScene();
    // Добавьте ваши фотографии watchlist в scene_watchlist
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
}

Profile::~Profile()
{
    delete ui;
}

void Profile::on_button_edit_clicked()
{
    // Включить режим редактирования
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

    // Показать кнопку "Save"
    button_save->setVisible(true);

    // Скрыть кнопку "Edit"
    button_edit->setVisible(false);
}

void Profile::on_button_save_clicked()
{
    // Сохранить изменения
    // ...

    // Выключить режим редактирования
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

    // Скрыть кнопку "Save"
    button_save->setVisible(false);

    // Показать кнопку "Edit"
    button_edit->setVisible(true);
}