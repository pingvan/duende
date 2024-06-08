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
    void build();
    void connect_slots();
    ~RegistrationPage();

private slots:
    void loginButton_clicked();
    void signupButton_clicked();

signals:
    void registrationSuccessful();
    void loginSuccessful();

private:
    Ui::RegistrationPage *ui;
    Manager *manager;
    QLineEdit *login_username;
    QLineEdit *login_password;
    QLineEdit *signup_username;
    QLineEdit *signup_email;
    QLineEdit *signup_password;
    QLineEdit *signup_confirm;
    QPushButton *signupButton;
    QPushButton *loginButton;
};

#endif // REGISTRATIONPAGE_H
