#include "sidebar.h"
#include "ui_sidebar.h"
#include "chats.h"
#include "recommendations.h"
#include "follows.h"
#include <QPushButton>

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SideBar)
{
    ui->setupUi(this);

    connect(ui->toolButton_chats, &QPushButton::clicked, this, &SideBar::on_toolButton_chats_clicked);
    connect(ui->toolButton_recommendations, &QPushButton::clicked, this, &SideBar::on_toolButton_recommendations_clicked);
    connect(ui->toolButton_follows, &QPushButton::clicked, this, &SideBar::on_toolButton_follows_clicked);
}

SideBar::~SideBar()
{
    delete ui;
}

void SideBar::on_toolButton_chats_clicked()
{
    emit showChatsPage();
}

void SideBar::on_toolButton_recommendations_clicked()
{
    emit showRecommendationsPage();
}

void SideBar::on_toolButton_follows_clicked()
{
    emit showFollowsPage();
}
