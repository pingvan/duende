#include "mainwindow.h"
#include "chats.h"
#include "recommendations.h"
#include "follows.h"
#include "./ui_mainwindow.h"
#include "sidebar.h"

#include <QDockWidget>
#include <QToolButton>
#include <QToolBar>
#include <QHBoxLayout>
#include <QPixmap>
#include <QListWidget>
#include <QDebug>

MainWindow::MainWindow(Manager* managerPtr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(managerPtr)
{
    ui->setupUi(this);

    qDebug() << "Main window start\n";

    qDebug() << "ID: " << manager->ID;

    auto *centralWidget = new QWidget(this);
    auto *layout = new QHBoxLayout(centralWidget);

    auto *sidebar = new QListWidget();
    sidebar->addItem("Chats");
    sidebar->addItem("Recommendations");
    sidebar->addItem("Follows");
    layout->addWidget(sidebar, 1);

    pages = new QStackedWidget();
    layout->addWidget(pages, 4);
    
    auto *chatsPage = new Chats(managerPtr);
    auto *recommendationsPage = new Recommendations();
    auto *followsPage = new Follows();
    auto *profilePage = new Profile(managerPtr);

    pages->addWidget(chatsPage);
    pages->addWidget(recommendationsPage);
    pages->addWidget(followsPage);
    pages->addWidget(profilePage);

    setCentralWidget(centralWidget);

    connect(sidebar, &QListWidget::currentRowChanged, pages, &QStackedWidget::setCurrentIndex);

    auto *profileButton = new QToolButton();
    profileButton->setIcon(QIcon("./img/tarantino.jpg"));
    connect(profileButton, &QToolButton::clicked, this, &MainWindow::openProfilePage);

    auto *toolbar = addToolBar("Profile toolbar");
    toolbar->addWidget(profileButton);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setAllowedAreas(Qt::TopToolBarArea);
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolbar->addWidget(spacer);
    toolbar->addAction(QIcon("./img/tarantino.jpg"), "Profile", this, &MainWindow::openProfilePage);

    qDebug() << "Main window end\n";
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openProfilePage() {
    pages->setCurrentIndex(3);

    // // Remove the old profile page
    // QWidget* oldProfilePage = pages->widget(3);
    // pages->removeWidget(oldProfilePage);
    // delete oldProfilePage;

    // // Create a new profile page
    // auto *profilePage = new Profile(managerPtr);

    // // Add the new profile page to the QStackedWidget
    // pages->insertWidget(3, profilePage);

    // // Show the new profile page
    // pages->setCurrentIndex(3);
}

