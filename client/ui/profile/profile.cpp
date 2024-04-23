#include "profile.h"
#include "ui_profile.h"


Profile::Profile(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Profile)
    , manager(managerPtr)
{
    ui->setupUi(this);

    qDebug() << "Profile page start\n";

    authservice::UserFullPage *user = manager->new_client.get_user_full_page(manager->ID);

    assert(user != nullptr);

    leftBox = new QVBoxLayout;
    rightBox = new QVBoxLayout;
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addLayout(leftBox, 0, 0);
    mainLayout->addLayout(rightBox, 0, 1);
    mainLayout->setColumnMinimumWidth(0, 500);
    mainLayout->setColumnStretch(0, 0);

    QLabel *label_photo = new QLabel();
    QPixmap pixmap(":/img/robertdeniro.jpg");
    pixmap = pixmap.scaledToWidth(mainLayout->columnMinimumWidth(0), Qt::SmoothTransformation);
    label_photo->setPixmap(pixmap);

    label_status = new QLabel(QString::fromStdString(user->quote().c_str()));
    label_status->setMaximumWidth(500);
    lineEdit_status = new QLineEdit();
    lineEdit_status->setMaximumWidth(500);
    lineEdit_status->hide();

    label_about = new QLabel(QString::fromStdString(user->about().c_str()));
    label_about->setMaximumWidth(500);
    label_about->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label_about->setWordWrap(true);
    textEdit_about = new QTextEdit();
    textEdit_about->setMaximumWidth(500);
    textEdit_about->hide();

    label_username = new QLabel(QString::fromStdString(user->preview().login().c_str()));
    lineEdit_username = new QLineEdit();
    lineEdit_username->hide();

    std::string genres_str = "Favorite genres: ";

    if (!user->favorite_genres().empty()) {
        for (const auto& genre : user->favorite_genres()) {
            genres_str += genre + ", ";
        }
        genres_str = genres_str.substr(0, genres_str.size()-2);
    }

    QHBoxLayout *genresLayout = new QHBoxLayout;
    label_favorite_genres = new QLabel(QString::fromStdString(genres_str));
    button_edit_genres = new QPushButton("Add");
    button_edit_genres->setFixedWidth(100);
    button_edit_genres->setCursor(Qt::PointingHandCursor);
    button_edit_genres->hide();
    genresLayout->addWidget(label_favorite_genres);
    genresLayout->addWidget(button_edit_genres);
    connect(button_edit_genres, &QPushButton::clicked, this, &Profile::button_edit_genres_clicked);

    view_favActors = new QGraphicsView();
    scene_favActors = new QGraphicsScene();

    favActorsImages << ":/img/tarantino.jpg" << ":/img/tarantino.jpg" << ":/img/tarantino.jpg";
    int x = 0;
    for (auto it = favActorsImages.rbegin(); it != favActorsImages.rend(); ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap.scaledToHeight(view_favActors->height()));
        item->setPos(x, 0);
        scene_favActors->addItem(item);
        x += item->boundingRect().width() + 10;
    }

    view_favActors->setScene(scene_favActors);
    view_favActors->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_favActors->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view_watchlist = new QGraphicsView();
    scene_watchlist = new QGraphicsScene();

    watchlistImages << ":/img/titanik.jpeg" << ":/img/titanik.jpeg" << ":/img/titanik.jpeg";
    x = 0;
    for (auto it = watchlistImages.rbegin(); it != watchlistImages.rend(); ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap.scaledToHeight(view_watchlist->height()));
        item->setPos(x, 0);
        scene_watchlist->addItem(item);
        x += item->boundingRect().width() + 10;
    }

    view_watchlist->setScene(scene_watchlist);
    view_watchlist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_watchlist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    button_edit = new QPushButton("Edit");
    button_edit->setCursor(Qt::PointingHandCursor);
    connect(button_edit, &QPushButton::clicked, this, &Profile::button_edit_clicked);

    button_save = new QPushButton("Save");
    button_save->setCursor(Qt::PointingHandCursor);
    connect(button_save, &QPushButton::clicked, this, &Profile::button_save_clicked);
    button_save->setVisible(false);

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    leftBox->addWidget(label_photo);
    leftBox->addWidget(label_status);
    leftBox->addWidget(label_about);
    leftBox->addItem(verticalSpacer);
    leftBox->addWidget(button_edit);
    leftBox->addWidget(button_save);

    rightBox->addWidget(label_username);
    rightBox->addWidget(lineEdit_username);
    rightBox->addLayout(genresLayout);
    rightBox->addWidget(view_favActors);
    rightBox->addWidget(view_watchlist);
   

    this->setLayout(mainLayout);
    qDebug() << "Profile page end\n";
}

Profile::~Profile()
{
    delete ui;
}

void Profile::button_edit_clicked()
{
    leftBox->replaceWidget(label_status, lineEdit_status);
    leftBox->replaceWidget(label_about, textEdit_about);
    lineEdit_status->setText(label_status->text());
    lineEdit_status->show();
    label_status->hide();
    textEdit_about->setText(label_about->text());
    textEdit_about->show();
    label_about->hide();

    rightBox->replaceWidget(label_username, lineEdit_username);
    lineEdit_username->setText(label_username->text());
    lineEdit_username->show();
    label_username->hide();
    button_edit_genres->show();


    QPixmap plusPixmap(":/img/plus.png");
    int x = 0;
    ClickablePixmapItem *plusItem_favActors = new ClickablePixmapItem(plusPixmap.scaledToHeight(view_favActors->height()));
    connect(plusItem_favActors, &ClickablePixmapItem::clicked, this, &Profile::plusItem_favActors_clicked);
    plusItem_favActors->setPos(x, 0);
    plusItem_favActors->setCursor(Qt::PointingHandCursor);
    scene_favActors->clear();
    scene_favActors->addItem(plusItem_favActors);
    connect(plusItem_favActors, &ClickablePixmapItem::clicked, this, &Profile::plusItem_favActors_clicked);

    x += plusItem_favActors->boundingRect().width() + 10;
    for (auto it = favActorsImages.rbegin(); it != favActorsImages.rend(); ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap.scaledToHeight(view_favActors->height()));
        item->setPos(x, 0);
        scene_favActors->addItem(item);
        x += item->boundingRect().width() + 10;
    }

    x = 0;
    ClickablePixmapItem *plusItem_watchlist = new ClickablePixmapItem(plusPixmap.scaledToHeight(view_watchlist->height()));
    connect(plusItem_watchlist, &ClickablePixmapItem::clicked, this, &Profile::plusItem_watchlist_clicked);
    plusItem_watchlist->setPos(x, 0);
    plusItem_watchlist->setCursor(Qt::PointingHandCursor);
    scene_watchlist->clear();
    scene_watchlist->addItem(plusItem_watchlist);
    connect(plusItem_watchlist, &ClickablePixmapItem::clicked, this, &Profile::plusItem_watchlist_clicked);

    x += plusItem_watchlist->boundingRect().width() + 10;
    for (auto it = watchlistImages.rbegin(); it != watchlistImages.rend(); ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap.scaledToHeight(view_watchlist->height()));
        item->setPos(x, 0);
        scene_watchlist->addItem(item);
        x += item->boundingRect().width() + 10;
    }

    button_save->setVisible(true);
    button_edit->setVisible(false);
}

void Profile::button_save_clicked()
{   
    manager->new_client.change_username(manager->ID, lineEdit_username->text().toStdString());
    manager->new_client.change_quote(manager->ID, lineEdit_status->text().toStdString());
    manager->new_client.change_bio(manager->ID, textEdit_about->toPlainText().toStdString());
    
    leftBox->replaceWidget(lineEdit_status, label_status);
    leftBox->replaceWidget(textEdit_about, label_about);
    label_status->setText(lineEdit_status->text());
    label_status->show();
    lineEdit_status->hide();
    label_about->setText(textEdit_about->toPlainText());
    label_about->show();
    textEdit_about->hide();

    rightBox->replaceWidget(lineEdit_username, label_username);
    label_username->setText(lineEdit_username->text());
    label_username->show();
    lineEdit_username->hide();
    button_edit_genres->hide();

    int x = 0;
    scene_favActors->clear();
    
    for (auto it = favActorsImages.rbegin(); it != favActorsImages.rend(); ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap.scaledToHeight(view_favActors->height()));
        item->setPos(x, 0);
        scene_favActors->addItem(item);
        x += item->boundingRect().width() + 10;
    }

    x = 0;
    scene_watchlist->clear();
    for (auto it = watchlistImages.rbegin(); it != watchlistImages.rend(); ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap.scaledToHeight(view_watchlist->height()));
        item->setPos(x, 0);
        scene_watchlist->addItem(item);
        x += item->boundingRect().width() + 10;
    }


    button_save->setVisible(false);
    button_edit->setVisible(true);
}

void Profile::plusItem_favActors_clicked()
{
    Actors_SearchWindow *searchWindow = new Actors_SearchWindow(manager, this);
    searchWindow->setWindowModality(Qt::ApplicationModal);
    searchWindow->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    searchWindow->move(QGuiApplication::screens().at(0)->geometry().center() - searchWindow->rect().center());
    searchWindow->show();
}

void Profile::plusItem_watchlist_clicked()
{
    Movies_SearchWindow *searchWindow = new Movies_SearchWindow(manager, this);
    searchWindow->setWindowModality(Qt::ApplicationModal);
    searchWindow->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    searchWindow->move(QGuiApplication::screens().at(0)->geometry().center() - searchWindow->rect().center());
    searchWindow->show();
}

void Profile::button_edit_genres_clicked()
{
    genres_searchWindow = new Genres_SearchWindow(manager, this);
    connect(genres_searchWindow, &Genres_SearchWindow::windowClosed, this, &Profile::update_favorite_genres);
    genres_searchWindow->setWindowModality(Qt::ApplicationModal);
    genres_searchWindow->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    genres_searchWindow->move(QGuiApplication::screens().at(0)->geometry().center() - genres_searchWindow->rect().center());
    genres_searchWindow->show();
}

void Profile::update_favorite_genres()
{
    std::string genres_str = "Favorite genres: ";

    auto user = manager->new_client.get_user_full_page(manager->ID);

    if (!user->favorite_genres().empty()) {
        for (const auto& genre : user->favorite_genres()) {
            genres_str += genre + ", ";
        }
        genres_str = genres_str.substr(0, genres_str.size()-2);
    }

    label_favorite_genres->setText(QString::fromStdString(genres_str));
}