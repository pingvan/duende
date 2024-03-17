#include "profile.h"
#include "ui_profile.h"

Profile::Profile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Profile)
{
    ui->setupUi(this);
}

Profile::~Profile()
{
    delete ui;
}

void Profile::on_toolButton_edit_clicked()
{
    ui->lineEdit_username->setReadOnly(false);
    ui->lineEdit_status->setReadOnly(false);
    ui->lineEdit_genres->setReadOnly(false);
    ui->textEdit_about->setReadOnly(false);

    QString editableStyle = "background-color: white;";
    ui->lineEdit_username->setStyleSheet(editableStyle);
    ui->lineEdit_status->setStyleSheet(editableStyle);
    ui->textEdit_about->setStyleSheet(editableStyle);
    ui->lineEdit_genres->setStyleSheet(editableStyle);

    ui->toolButton_edit->setEnabled(false);
}

