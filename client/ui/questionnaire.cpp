#include "questionnaire.h"
#include "ui_questionnaire.h"
#include <QGuiApplication>
#include <QScreen>

Questionnaire::Questionnaire(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Questionnaire)
{
    ui->setupUi(this);
    connect(this, &Questionnaire::questionnaireFilled, this, &Questionnaire::onQuestionnaireFilled);
}

Questionnaire::~Questionnaire()
{
    delete ui;
}

void Questionnaire::on_pushButton_save_questionnaire_clicked()
{
    QString favourite_actors = ui->lineEdit_actors->text();
    QString favourite_films = ui->lineEdit_movies->text();
    emit questionnaireFilled();
}

void Questionnaire::onQuestionnaireFilled() {
}


