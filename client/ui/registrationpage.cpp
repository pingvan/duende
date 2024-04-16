#include "registrationpage.h"
#include "ui_registrationpage.h"
#include "mainwindow.h"

#include <grpcpp/grpcpp.h>
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


RegistrationPage::RegistrationPage(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationPage),
    manager(managerPtr)
{
    ui->setupUi(this);

    qDebug() << "Registration page start\n";
    
    connect(this, &RegistrationPage::registrationSuccessful, this, &RegistrationPage::onRegistrationSuccess);

    QTabWidget *tabWidget = new QTabWidget(this);

    lineEdit_login_username = new QLineEdit(this);
    lineEdit_login_password = new QLineEdit(this);
    lineEdit_signup_username = new QLineEdit(this);
    lineEdit_signup_email = new QLineEdit(this);
    lineEdit_signup_password = new QLineEdit(this);
    lineEdit_signup_confirm = new QLineEdit(this);

    lineEdit_login_password->setEchoMode(QLineEdit::Password);
    lineEdit_signup_password->setEchoMode(QLineEdit::Password);
    lineEdit_signup_confirm->setEchoMode(QLineEdit::Password);

    lineEdit_signup_username->setPlaceholderText("Enter username");
    lineEdit_signup_email->setPlaceholderText("Enter email");
    lineEdit_signup_password->setPlaceholderText("Enter password");
    lineEdit_signup_confirm->setPlaceholderText("Enter password confirmation");

    lineEdit_login_username->setPlaceholderText("Enter login");
    lineEdit_login_password->setPlaceholderText("Enter password");

    QWidget *tab1 = new QWidget;
    QWidget *tab2 = new QWidget;
    tabWidget->addTab(tab1, "Signup");
    tabWidget->addTab(tab2, "Login");

    QPushButton *signupButton = new QPushButton("Sign up", tab1);

    QWidget *formWidget1 = new QWidget(tab1);
    QFormLayout *formLayout1 = new QFormLayout(formWidget1);
    formLayout1->addRow(lineEdit_signup_username);
    formLayout1->addRow(lineEdit_signup_email);
    formLayout1->addRow(lineEdit_signup_password);
    formLayout1->addRow(lineEdit_signup_confirm);

    QPushButton *loginButton = new QPushButton("Log in", tab2);

    QWidget *formWidget2 = new QWidget(tab2);
    QFormLayout *formLayout2 = new QFormLayout(formWidget2);
    formLayout2->addRow(lineEdit_login_username);
    formLayout2->addRow(lineEdit_login_password);

    QVBoxLayout *vLayout1 = new QVBoxLayout(tab1);
    vLayout1->addStretch(1);
    vLayout1->addWidget(formWidget1);
    vLayout1->addStretch(1);
    vLayout1->addWidget(signupButton, 0, Qt::AlignCenter);

    QVBoxLayout *vLayout2 = new QVBoxLayout(tab2);
    vLayout2->addStretch(1);
    vLayout2->addWidget(formWidget2);
    vLayout2->addStretch(1);
    vLayout2->addWidget(loginButton, 0, Qt::AlignCenter);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 2);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 2);
    mainLayout->addWidget(tabWidget, 1, 1);

    connect(signupButton, &QPushButton::clicked, this, &RegistrationPage::on_signupButton_clicked);
    connect(loginButton, &QPushButton::clicked, this, &RegistrationPage::on_loginButton_clicked);

    qDebug() << "Registration page end\n";
}

RegistrationPage::~RegistrationPage()
{
    delete ui;
}

void RegistrationPage::on_loginButton_clicked()
{
    QString login = lineEdit_login_username->text();
    QString password = lineEdit_login_password->text();
    authservice::LoginResponse *response = manager->new_client.login(login.toStdString(), password.toStdString());
    manager->ID = response->id();

    qDebug() << "ID: " << manager->ID;

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


void RegistrationPage::on_signupButton_clicked()
{
    QString username = lineEdit_signup_username->text();
    QString email = lineEdit_signup_email->text();
    QString password = lineEdit_signup_password->text();
    QString password_confirmation = lineEdit_signup_confirm->text();
    authservice::SignupResponse *response = manager->new_client.registration(username.toStdString(), email.toStdString(), password.toStdString(), password_confirmation.toStdString());

    if (response->message() == "OK") {
        authservice::LoginResponse *response = manager->new_client.login(username.toStdString(), password.toStdString());
        manager->ID = response->id();
        manager->new_client.change_username(manager->ID, username.toStdString());
        emit registrationSuccessful();
    }
    else {
        QMessageBox::warning(this, "Error", response->message().c_str());
    }
}

void RegistrationPage::onLoginSuccess()
{

}

