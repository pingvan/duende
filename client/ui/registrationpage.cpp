#include "registrationpage.h"
#include "ui_registrationpage.h"
#include "mainwindow.h"

#include <grpcpp/grpcpp.h>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>


RegistrationPage::RegistrationPage(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationPage),
    manager(managerPtr)
{
    ui->setupUi(this);
    connect(this, &RegistrationPage::registrationSuccessful, this, &RegistrationPage::onRegistrationSuccess);

    ui->lineEdit_login_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_signup_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_signup_confirm->setEchoMode(QLineEdit::Password);

    ui->lineEdit_signup_username->setPlaceholderText("Enter username");
    ui->lineEdit_signup_email->setPlaceholderText("Enter email");
    ui->lineEdit_signup_password->setPlaceholderText("Enter password");
    ui->lineEdit_signup_confirm->setPlaceholderText("Enter password confirmation");

    ui->lineEdit_login_username->setPlaceholderText("Enter username");
    ui->lineEdit_login_password->setPlaceholderText("Enter password");
}

RegistrationPage::~RegistrationPage()
{
    delete ui;
}

void RegistrationPage::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_login_username->text();
    QString password = ui->lineEdit_login_password->text();
    grpc::Status status = manager->new_client.login(username.toStdString(), password.toStdString());

    if (status.ok()) {
        emit loginSuccessful();
    }
    else {
        QMessageBox::warning(this, "Error", status.error_message().c_str());
    }
}

void RegistrationPage::onRegistrationSuccess()
{

}


void RegistrationPage::on_pushButton_signup_clicked()
{
    QString username = ui->lineEdit_signup_username->text();
    QString email = ui->lineEdit_signup_email->text();
    QString password = ui->lineEdit_signup_password->text();
    QString password_confirmation = ui->lineEdit_signup_confirm->text();
    grpc::Status status = manager->new_client.registration(username.toStdString(), email.toStdString(), password.toStdString(), password_confirmation.toStdString());

    if (status.ok()) {
        emit registrationSuccessful();
    }
    else {
        QMessageBox::warning(this, "Error", status.error_message().c_str());
    }
}

void RegistrationPage::onLoginSuccess()
{

}

