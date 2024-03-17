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
    ui->textEdit_about->setReadOnly(false);
    ui->lineEdit_genres->setReadOnly(false);

    ui->lineEdit_username->setVisible(true);
    ui->lineEdit_status->setVisible(true);
    ui->textEdit_about->setVisible(true);
    ui->lineEdit_genres->setVisible(true);

    ui->lineEdit_username->setText(ui->label_username->text());
    ui->lineEdit_status->setText(ui->label_status->text());
    ui->textEdit_about->setPlainText(ui->label_about->text());
    ui->lineEdit_genres->setText(ui->label_genres->text());

    ui->label_username->setVisible(false);
    ui->label_status->setVisible(false);
    ui->label_about->setVisible(false);
    ui->label_genres->setVisible(false);

    ui->toolButton_edit->setEnabled(false);
    ui->pushButton_save->setVisible(true);
}


void Profile::on_pushButton_save_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString status = ui->lineEdit_status->text();
    QString description = ui->textEdit_about->toPlainText();
    QString genres = ui->lineEdit_genres->text();

    ui->label_username->setText(username);
    ui->label_status->setText(status);
    ui->label_about->setText(description);
    ui->label_genres->setText(genres);

    ui->lineEdit_username->setVisible(false);
    ui->lineEdit_status->setVisible(false);
    ui->textEdit_about->setVisible(false);
    ui->lineEdit_genres->setVisible(false);

    ui->label_username->setVisible(true);
    ui->label_status->setVisible(true);
    ui->label_about->setVisible(true);
    ui->label_genres->setVisible(true);

    ui->toolButton_edit->setEnabled(true);
    ui->pushButton_save->setVisible(false);
}

