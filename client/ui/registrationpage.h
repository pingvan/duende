#ifndef REGISTRATIONPAGE_H
#define REGISTRATIONPAGE_H

#include <QWidget>
#include <QLineEdit>

#include "../manager.hpp"

namespace Ui {
class RegistrationPage;
}

class RegistrationPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationPage(Manager* managerPtr, QWidget *parent = nullptr);
    ~RegistrationPage();

private slots:
    void on_loginButton_clicked();
    void onRegistrationSuccess();
    void onLoginSuccess();
    void on_signupButton_clicked();

signals:
    void registrationSuccessful();
    void loginSuccessful();

private:
    Ui::RegistrationPage *ui;
    Manager *manager;
    QLineEdit *lineEdit_login_username;
    QLineEdit *lineEdit_login_password;
    QLineEdit *lineEdit_signup_username;
    QLineEdit *lineEdit_signup_email;
    QLineEdit *lineEdit_signup_password;
    QLineEdit *lineEdit_signup_confirm;
};

#endif // REGISTRATIONPAGE_H
