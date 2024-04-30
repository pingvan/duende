#ifndef REGISTRATIONPAGE_H
#define REGISTRATIONPAGE_H

#include "manager.h"

#include <grpcpp/grpcpp.h>
#include <QWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>
#include <QDebug>
#include <QTabWidget>
#include <QFormLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QLineEdit>
#include <QPushButton>

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
    void loginButton_clicked();
    void onRegistrationSuccess();
    void onLoginSuccess();
    void signupButton_clicked();

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
