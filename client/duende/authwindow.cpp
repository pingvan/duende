#include "authwindow.h"
#include "ui_authwindow.h"

authwindow::authwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::authwindow)
{
    ui->setupUi(this);
}

authwindow::~authwindow()
{
    delete ui;
}
