#include "questionnaire.h"
#include "ui_questionnaire.h"

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
    qDebug() << favourite_actors << " " << favourite_films;
    emit questionnaireFilled();
}

void Questionnaire::onQuestionnaireFilled() {
}

