#ifndef QUESTIONNAIRE_H
#define QUESTIONNAIRE_H

#include "manager.hpp"

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QPushButton>
#include <QCompleter>

namespace Ui {
class Questionnaire;
}

class Questionnaire : public QWidget
{
    Q_OBJECT

public:
    explicit Questionnaire(Manager* managerPtr, QWidget *parent = nullptr);
    ~Questionnaire();

signals:
    void questionnaireFilled();

private slots:
    void on_pushButton_save_questionnaire_clicked();
    void onQuestionnaireFilled();

private:
    Ui::Questionnaire *ui;
    Manager *manager;
    QLineEdit *lineEditActors;
    QLineEdit *lineEditMovies;
    QLineEdit *lineEditGenres;
};

#endif // QUESTIONNAIRE_H
