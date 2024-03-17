#include "chats.h"
#include "ui_chats.h"
#include "sidebar.h"

Chats::Chats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chats)
{
    ui->setupUi(this);
}

Chats::~Chats()
{
    delete ui;
}
