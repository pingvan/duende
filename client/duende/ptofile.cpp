#include "ptofile.h"
#include "ui_ptofile.h"

Ptofile::Ptofile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Ptofile)
{
    ui->setupUi(this);
}

Ptofile::~Ptofile()
{
    delete ui;
}
