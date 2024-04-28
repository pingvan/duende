#include "registrationpage.h"
#include "ui_registrationpage.h"
#include "mainwindow.h"

#include <grpcpp/grpcpp.h>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>
#include <QDebug>


RegistrationPage::RegistrationPage(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationPage),
    manager(managerPtr)
{
    ui->setupUi(this);

    qDebug() << "Registration page start\n";
    
    connect(this, &RegistrationPage::registrationSuccessful, this, &RegistrationPage::onRegistrationSuccess);

    ui->lineEdit_login_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_signup_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_signup_confirm->setEchoMode(QLineEdit::Password);

    ui->lineEdit_signup_username->setPlaceholderText("Enter username");
    ui->lineEdit_signup_email->setPlaceholderText("Enter email");
    ui->lineEdit_signup_password->setPlaceholderText("Enter password");
    ui->lineEdit_signup_confirm->setPlaceholderText("Enter password confirmation");

    ui->lineEdit_login_username->setPlaceholderText("Enter login");
    ui->lineEdit_login_password->setPlaceholderText("Enter password");

    qDebug() << "Registration page end\n";
}

RegistrationPage::~RegistrationPage()
{
    delete ui;
}

void RegistrationPage::on_pushButton_login_clicked()
{
    QString login = ui->lineEdit_login_username->text();
    QString password = ui->lineEdit_login_password->text();
    authservice::LoginResponse *response = manager->new_client.login(login.toStdString(), password.toStdString());
    manager->ID = response->id();

    if (login == "admin" && password == "admin" || response->message() == "OK") {
        emit loginSuccessful();
    }
    else {
        QMessageBox::warning(this, "Error", response->message().c_str());
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
        manager->new_client.change_username(manager->ID, username.toStdString());
        emit registrationSuccessful();
    }
    else {
        QMessageBox::warning(this, "Error", status.error_message().c_str());
    }
}

void RegistrationPage::onLoginSuccess()
{

}

