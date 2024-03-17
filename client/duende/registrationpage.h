#ifndef REGISTRATIONPAGE_H
#define REGISTRATIONPAGE_H

#include <QWidget>

namespace Ui {
class RegistrationPage;
}

class RegistrationPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationPage(QWidget *parent = nullptr);
    ~RegistrationPage();

private slots:
    void on_pushButton_login_clicked();
    void onRegistrationSuccess();

signals:
    void registrationSuccessful();

private:
    Ui::RegistrationPage *ui;
};

#endif // REGISTRATIONPAGE_H
