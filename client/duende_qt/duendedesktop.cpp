#include "duendedesktop.h"
#include "ui_duendedesktop.h"

DuendeDesktop::DuendeDesktop(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DuendeDesktop)
{
    ui->setupUi(this);
}

DuendeDesktop::~DuendeDesktop()
{
    delete ui;
}
