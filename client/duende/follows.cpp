#include "follows.h"
#include "ui_follows.h"
#include "sidebar.h"

Follows::Follows(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Follows)
{
    ui->setupUi(this);
}

Follows::~Follows()
{
    delete ui;
}
