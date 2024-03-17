#include "registrationpage.h"
#include "ui_registrationpage.h"
#include "mainwindow.h"

#include <QMessageBox>

RegistrationPage::RegistrationPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationPage)
{
    ui->setupUi(this);
    connect(this, &RegistrationPage::registrationSuccessful, this, &RegistrationPage::onRegistrationSuccess);
}

RegistrationPage::~RegistrationPage()
{
    delete ui;
}

void RegistrationPage::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if (username != "" && password != "") {
        emit registrationSuccessful();
    }
    else {
        QMessageBox::warning(this, "Error", "User not found");
    }
}

void RegistrationPage::onRegistrationSuccess()
{

}

