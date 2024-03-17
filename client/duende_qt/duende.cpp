#include "duende.h"
#include "./ui_duende.h"
#include <QMessageBox>

#include "services/authservice/authservice.pb.h"
#include "services/authservice/authservice.grpc.pb.h"

Duende::Duende(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Duende)
{
    ui->setupUi(this);
}

Duende::~Duende()
{
    delete ui;
}

void Duende::on_pushButton_login_clicked()
{
    QString login = ui->lineEdit_login->text();
    QString password = ui->lineEdit_password->text();
    authservice::User user;
    user.set_username(login.toStdString());
    // user.set_password(password.toStdString());  

    if (login != "" && password != "") {
        QMessageBox::information(this, "Log in", "Access!");
        hide();
        duendeDesktop = new DuendeDesktop(this);
        duendeDesktop->show();
    }
    else {
        QMessageBox::warning(this, "Log in", "Fail!");
    }
}

