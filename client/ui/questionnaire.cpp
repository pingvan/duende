#include "questionnaire.h"
#include "ui_questionnaire.h"

#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QPushButton>

Questionnaire::Questionnaire(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Questionnaire)
    , manager(managerPtr)
{
    ui->setupUi(this);
    qDebug() << "Questionnaire page start\n";
    connect(this, &Questionnaire::questionnaireFilled, this, &Questionnaire::onQuestionnaireFilled);

    QFormLayout *formLayout = new QFormLayout;

    lineEditActors = new QLineEdit;
    lineEditMovies = new QLineEdit;

    formLayout->addRow(tr("Favorite Actors:"), lineEditActors);
    formLayout->addRow(tr("Favorite Movies:"), lineEditMovies);

    QPushButton *saveButton = new QPushButton(tr("Save"));
    formLayout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, &Questionnaire::on_pushButton_save_questionnaire_clicked);

    this->setLayout(formLayout);
    this->setFixedSize(300, 200);

    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    this->move((screenGeometry.width() - this->width()) / 2, (screenGeometry.height() - this->height()) / 2);

    qDebug() << "Questionnaire page end\n";
}

Questionnaire::~Questionnaire()
{
    delete ui;
}

void Questionnaire::on_pushButton_save_questionnaire_clicked()
{
    QString favourite_actors = lineEditActors->text();
    QString favourite_films = lineEditMovies->text();

    // TODO: actor and film id by name and rating
    manager->new_client.add_actor(manager->ID, 0);
    manager->new_client.add_movie(manager->ID, 0, 0);
    emit questionnaireFilled();
}

void Questionnaire::onQuestionnaireFilled() {
}


