#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Manager* managerPtr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(managerPtr)
{
    ui->setupUi(this);

    qDebug() << "Main window start\n";

    auto *centralWidget = new QWidget(this);
    auto *layout = new QHBoxLayout(centralWidget);

    auto *sidebar = new QWidget();
    auto *sidebar_layout = new QVBoxLayout(sidebar);
    sidebar_layout->addStretch();

    auto *chats_button = new QPushButton();
    QPixmap chatPixmap(":/img/chat.png");
    chats_button->setIcon(QIcon(chatPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    chats_button->setIconSize(QSize(100, 100));
    chats_button->setCursor(Qt::PointingHandCursor);
    chats_button->setFlat(true);

    auto *rec_button = new QPushButton();
    QPixmap peoplePixmap(":/img/people.png");
    rec_button->setIcon(QIcon(peoplePixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    rec_button->setIconSize(QSize(100, 100));
    rec_button->setCursor(Qt::PointingHandCursor);
    rec_button->setFlat(true);

    auto *search_button = new QPushButton();
    QPixmap searchPixmap(":/img/search.png");
    search_button->setIcon(QIcon(searchPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    search_button->setIconSize(QSize(100, 100));
    search_button->setCursor(Qt::PointingHandCursor);
    search_button->setFlat(true);

    sidebar_layout->addWidget(chats_button);
    sidebar_layout->addStretch();
    sidebar_layout->addWidget(rec_button);
    sidebar_layout->addStretch();
    sidebar_layout->addWidget(search_button);
    sidebar_layout->addStretch();

    layout->addWidget(sidebar, 1);

    pages = new QStackedWidget();
    layout->addWidget(pages, 4);
    
    auto *chatsPage = new Chats(managerPtr);
    auto *recommendationsPage = new Recommendations();
    auto *followsPage = new Follows(managerPtr);
    auto *profilePage = new Profile(managerPtr);

    otherPage = new OtherPage(0, manager, this);

    pages->addWidget(chatsPage);
    pages->addWidget(recommendationsPage);
    pages->addWidget(followsPage);
    pages->addWidget(profilePage);
    pages->addWidget(otherPage);

    setCentralWidget(centralWidget);

    auto *profileButton = new QToolButton();
    profileButton->setCursor(Qt::PointingHandCursor);
    connect(profileButton, &QToolButton::clicked, this, &MainWindow::openProfilePage);

    auto *toolbar = addToolBar("Profile toolbar");
    toolbar->addWidget(profileButton);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setAllowedAreas(Qt::TopToolBarArea);
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolbar->setCursor(Qt::PointingHandCursor);

    toolbar->addAction(QIcon(":/img/cross.png"), "Exit", this, &QApplication::quit);
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolbar->addWidget(spacer);
    toolbar->addAction(QIcon(":/img/person.png"), "Profile", this, &MainWindow::openProfilePage);

    connect(chats_button, &QPushButton::clicked, this, [this]() { pages->setCurrentIndex(0); });
    connect(rec_button, &QPushButton::clicked, this, [this]() { pages->setCurrentIndex(1); });
    connect(search_button, &QPushButton::clicked, this, [this]() { pages->setCurrentIndex(2); });
    connect(followsPage, &Follows::userClicked, this, &MainWindow::openOtherPage);

    qDebug() << "Main window end\n";
}


MainWindow::~MainWindow()
{
    delete ui;
    qDebug() << "Main window deleted\n";
    manager->new_client.leave_chat_stream(manager->ID);
    manager->new_client.leave_invite_stream(manager->ID);
}

void MainWindow::openProfilePage() {
    pages->setCurrentIndex(3);
}

void MainWindow::openOtherPage(int id)
{
    otherPage->updatePage(id);
    pages->setCurrentWidget(otherPage);
}

