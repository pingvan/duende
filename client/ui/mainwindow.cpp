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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *centralWidget = new QWidget(this);
    auto *layout = new QHBoxLayout(centralWidget);

    auto *sidebar = new QListWidget();
    sidebar->addItem("Чаты");
    sidebar->addItem("Рекомендации");
    sidebar->addItem("Подписки");
    layout->addWidget(sidebar, 1);

    pages = new QStackedWidget();
    layout->addWidget(pages, 4);

    auto *chatsPage = new Chats();
    auto *recomendationsPage = new Recommendations();
    auto *followsPage = new Follows();
    auto *profilePage = new Profile();

    pages->addWidget(chatsPage);
    pages->addWidget(recomendationsPage);
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
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openProfilePage() {
    pages->setCurrentIndex(3);
}

