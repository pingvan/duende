#ifndef REGISTRATIONPAGE_H
#define REGISTRATIONPAGE_H

#include <grpcpp/grpcpp.h>
#include <QDebug>
#include <QFormLayout>
#include <QGridLayout>
#include <QGuiApplication>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTabWidget>
#include <QWidget>
#include "manager.h"

namespace Ui {
class RegistrationPage;
}

class RegistrationPage : public QWidget {
    Q_OBJECT

public:
    explicit RegistrationPage(Manager *managerPtr, QWidget *parent = nullptr);
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

#endif  // REGISTRATIONPAGE_H
