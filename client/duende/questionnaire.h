#ifndef QUESTIONNAIRE_H
#define QUESTIONNAIRE_H

#include <QWidget>

namespace Ui {
class Questionnaire;
}

class Questionnaire : public QWidget
{
    Q_OBJECT

public:
    explicit Questionnaire(QWidget *parent = nullptr);
    ~Questionnaire();

signals:
    void questionnaireFilled();

private slots:
    void on_pushButton_save_questionnaire_clicked();
    void onQuestionnaireFilled();

private:
    Ui::Questionnaire *ui;
};

#endif // QUESTIONNAIRE_H
