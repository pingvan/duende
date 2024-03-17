#include "mainwindow.h"
#include "chats.h"
#include "recommendations.h"
#include "follows.h"
#include "./ui_mainwindow.h"
#include "sidebar.h"
#include <QDockWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    stackedWidget = new QStackedWidget(this);
    sidebar = new SideBar(this);
    Chats *chatsPage = new Chats;
    Recommendations *recommendationsPage = new Recommendations;
    Follows *followsPage = new Follows;
    Profile *profile = new Profile;

    stackedWidget->addWidget(chatsPage);
    stackedWidget->addWidget(recommendationsPage);
    stackedWidget->addWidget(followsPage);
    stackedWidget->addWidget(profile);

    connect(sidebar, &SideBar::showChatsPage, this, &MainWindow::showChatsPageSlot);
    connect(sidebar, &SideBar::showRecommendationsPage, this, &MainWindow::showRecommendationsPageSlot);
    connect(sidebar, &SideBar::showFollowsPage, this, &MainWindow::showFollowsPageSlot);

    QDockWidget *dockWidget = new QDockWidget(this);
    dockWidget->setTitleBarWidget(new QWidget());
    dockWidget->setWidget(sidebar);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dockWidget->setFixedWidth(100);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    QToolButton *profileButton = new QToolButton(this);
    profileButton->setIcon(QIcon("./img/tarantino.jpg"));
    connect(profileButton, &QToolButton::clicked, this, &MainWindow::openProfilePage);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addStretch();
    titleLayout->addWidget(profileButton);
    titleLayout->setContentsMargins(0, 0, 5, 0);

    QWidget *titleWidget = new QWidget(this);
    titleWidget->setLayout(titleLayout);

    setMenuWidget(titleWidget);

    setCentralWidget(stackedWidget);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showChatsPageSlot()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showRecommendationsPageSlot()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showFollowsPageSlot()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::openProfilePage() {
    stackedWidget->setCurrentIndex(3);
}

