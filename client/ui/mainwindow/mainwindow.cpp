#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Manager *managerPtr, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), manager(managerPtr) {
    ui->setupUi(this);

    qDebug() << "Main window start\n";

    auto *centralWidget = new QWidget(this);
    auto *layout = new QHBoxLayout(centralWidget);

    auto *sidebar = new QWidget();
    sidebar->setFixedWidth(200);
    auto *sidebar_layout = new QVBoxLayout(sidebar);
    sidebar_layout->addStretch();
    // QLabel *imageLabel = new QLabel(sidebar);
    // imageLabel->setPixmap(QPixmap("/path/to/your/image.jpg"));
    // imageLabel->show();
    // sidebar->setStyleSheet("background-color: rgba(128, 35, 168, 128);");

    auto *chats_button = new QPushButton();
    chats_button->setIcon(createIcon(":/img/chat.png", ":/img/film.png"));
    chats_button->setIconSize(QSize(180, 180));
    chats_button->setCursor(Qt::PointingHandCursor);
    chats_button->setFlat(true);

    auto *rec_button = new QPushButton();
    rec_button->setIcon(createIcon(":/img/person-add.png", ":/img/film.png"));
    // rec_button->setIcon(QIcon(peoplePixmap.scaled(
    //     50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation
    // )));
    rec_button->setIconSize(QSize(180, 180));
    rec_button->setCursor(Qt::PointingHandCursor);
    rec_button->setFlat(true);

    auto *search_button = new QPushButton();
    search_button->setIcon(createIcon(":/img/people.png", ":/img/film.png"));
    search_button->setIconSize(QSize(180, 180));
    search_button->setCursor(Qt::PointingHandCursor);
    search_button->setFlat(true);

    chats_button->setObjectName("myButton");
    rec_button->setObjectName("myButton");
    search_button->setObjectName("myButton");

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
    auto *recommendationsPage = new Recommendations(managerPtr);
    auto *followsPage = new Follows(managerPtr);
    auto *profilePage = new Profile(managerPtr);

    otherPage = new OtherPage(0, manager, this);

    pages->addWidget(profilePage);
    pages->addWidget(chatsPage);
    pages->addWidget(recommendationsPage);
    pages->addWidget(followsPage);
    pages->addWidget(otherPage);

    setCentralWidget(centralWidget);

    // auto *profileButton = new QToolButton();
    // QPixmap personPixmap(":/img/person-circle.png");
    // profileButton->setIcon(QIcon(personPixmap.scaled(
    //     60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation
    // )));
    // profileButton->setCursor(Qt::PointingHandCursor);
    // connect(
    //     profileButton, &QToolButton::clicked, this, &MainWindow::openProfilePage
    // );

    auto *toolbar = addToolBar("Profile toolbar");
    toolbar->setFixedHeight(60);
    toolbar->setStyleSheet("QToolBar { border: none; }");
    // toolbar->setStyleSheet("background-color: rgba(128, 35, 168, 128);");
    // toolbar->addWidget(profileButton);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setAllowedAreas(Qt::TopToolBarArea);
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolbar->setCursor(Qt::PointingHandCursor);
    toolbar->setIconSize(QSize(60, 60));
    // toolbar->setStyleSheet("QToolButton { background: qlineargradient(x1: 0,
    // y1: 0, x2: 0, y2: 1, stop: 0 #b24ee7, stop: 1 #982ecb); border-radius:
    // 5px;}");

    toolbar->addAction(
        QIcon(":/img/box-arrow-left.png"), "Exit", this, &QApplication::quit
    );
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolbar->addWidget(spacer);
    toolbar->addAction(
        QIcon(":/img/person-circle.png"), "Profile", this,
        &MainWindow::openProfilePage
    );
    // toolbar->addWidget(profileButton);
    // connect(
    //     this->pages, &QStackedWidget::currentChanged,
    //     [this, profileButton]() {
    //         profileButton->setVisible(this->pages->currentIndex() != 3);
    //     }
    // );

    // connect(
    //     profileButton, &QToolButton::clicked, this,
    //     &MainWindow::openProfilePage
    // );

    connect(chats_button, &QPushButton::clicked, this, [this]() {
        pages->setCurrentIndex(1);
    });
    connect(rec_button, &QPushButton::clicked, this, [this]() {
        pages->setCurrentIndex(2);
    });
    connect(search_button, &QPushButton::clicked, this, [this]() {
        pages->setCurrentIndex(3);
    });
    connect(
        followsPage, &Follows::userClicked, this, &MainWindow::openOtherPage
    );
    connect(
        followsPage, &Follows::userFromListClicked, this,
        &MainWindow::openOtherPage
    );
    connect(
        recommendationsPage, &Recommendations::userClicked, this,
        &MainWindow::openOtherPage
    );
    connect(
        chatsPage, &Chats::userClicked, this,
        &MainWindow::openOtherPage
    );
    connect(
        recommendationsPage, &Recommendations::userFromListClicked, this,
        &MainWindow::openOtherPage
    );

    qDebug() << "Main window end\n";
}

MainWindow::~MainWindow() {
    delete ui;
    qDebug() << "Main window deleted\n";
    manager->new_client.leave_chat_stream(manager->ID);
    manager->new_client.leave_invite_stream(manager->ID);
    manager->new_client.leave_invite_chat_stream(manager->ID);
}

void MainWindow::openProfilePage() {
    pages->setCurrentIndex(0);
}

void MainWindow::openOtherPage(int id) {
    if (manager->new_client.is_blocked(manager->ID, id)) {
        QMessageBox::information(
            this, "Blocked User", "You can't view this page :("
        );
    }
    qDebug() << "openOtherPage called with id:" << id;
    otherPage->updatePage(id);
    pages->setCurrentWidget(otherPage);
}

QIcon MainWindow::createIcon(const QString &path1, const QString &path2) {
    QPixmap pixmap1(path1);
    QPixmap pixmap2(path2);
    QTransform transform;
    transform.rotate(90);
    QPixmap scaledPixmap1 =
        pixmap1.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap rotatedPixmap2 =
        pixmap2.scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            .transformed(transform);
    QPixmap combinedPixmap(
        std::max(scaledPixmap1.width(), rotatedPixmap2.width()),
        std::max(scaledPixmap1.height(), rotatedPixmap2.height())
    );
    combinedPixmap.fill(Qt::transparent);

    QPainter painter(&combinedPixmap);
    painter.drawPixmap(0, 0, rotatedPixmap2);
    int x = (rotatedPixmap2.width() - scaledPixmap1.width()) / 2;
    int y = (rotatedPixmap2.height() - scaledPixmap1.height()) / 2;
    painter.drawPixmap(x, y, scaledPixmap1);

    return QIcon(combinedPixmap);
}
