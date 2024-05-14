#include "otherpage.h"
#include "ui_otherpage.h"

OtherPage::OtherPage(int id, Manager* managerPtr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherPage),
    id(id),
    manager(managerPtr)
{
    ui->setupUi(this);

    qDebug() << "Other page start";

    user = manager->new_client.get_user_full_page(id);

    login = user->preview().login();

    assert(user != nullptr);

    build();

    qDebug() << "Other page end";
}

void OtherPage::sendInvite() 
{
    assert(manager != nullptr);
    manager->new_client.send_invite(manager->ID, id, manager->username, login);
}

void OtherPage::Subscribe() 
{
    assert(manager != nullptr);
    manager->new_client.subscribe(manager->ID, id);
    qDebug() << "Subscription made";
}

void OtherPage::updatePage(int newId)
{
    qDebug() << "Update page start";

    id = newId;

    user = manager->new_client.get_user_full_page(id);

    login = user->preview().login();

    assert(user != nullptr);

    // update all labels
    label_status->setText(QString::fromStdString(user->quote().c_str()));
    label_about->setText(QString::fromStdString(user->about().c_str()));
    label_username->setText(QString::fromStdString(user->preview().login().c_str()));

    // update genres
    std::string genres_str = "Favorite genres: ";

    if (!user->favorite_genres().empty()) {
        for (const auto& genre : user->favorite_genres()) {
            std::string favorite_genres_str = common::Genre_Name(genre);
            genres_str += favorite_genres_str + ", ";
        }
        genres_str = genres_str.substr(0, genres_str.size()-2);
    }

    label_favorite_genres->setText(QString::fromStdString(genres_str));

    // update scenes
    build_scenes();

    qDebug() << "Update page end";
}

void OtherPage::build_scenes()
{
    // delete if something
    if (view_favActors != nullptr) {
        delete view_favActors;
        view_favActors = nullptr;
    }
    if (scene_favActors != nullptr) {
        delete scene_favActors;
        scene_favActors = nullptr;
    }
    if (view_watchlist != nullptr) {
        delete view_watchlist;
        view_watchlist = nullptr;
    }
    if (scene_watchlist != nullptr) {
        delete scene_watchlist;
        scene_watchlist = nullptr;
    }

    view_favActors = new QGraphicsView();
    scene_favActors = new QGraphicsScene();
    view_watchlist = new QGraphicsView();
    scene_watchlist = new QGraphicsScene();

    // fav actors
    favActorsImages.clear();
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

    // watchlist
    watchlistImages.clear();
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

    // add to layout
    rightBox->addWidget(view_favActors);
    rightBox->addWidget(view_watchlist);
}

void OtherPage::build()
{
    // main layout
    leftBox = new QVBoxLayout;
    rightBox = new QVBoxLayout;
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addLayout(leftBox, 0, 0);
    mainLayout->addLayout(rightBox, 0, 1);
    mainLayout->setColumnMinimumWidth(0, 500);
    mainLayout->setColumnStretch(0, 0);
    this->setLayout(mainLayout);

    // photo
    QLabel *label_photo = new QLabel();
    QPixmap pixmap(":/img/robertdeniro.jpg");
    pixmap = pixmap.scaledToWidth(mainLayout->columnMinimumWidth(0), Qt::SmoothTransformation);
    label_photo->setPixmap(pixmap);

    // status
    label_status = new QLabel(QString::fromStdString(user->quote().c_str()));
    label_status->setMaximumWidth(500);

    // about
    label_about = new QLabel(QString::fromStdString(user->about().c_str()));
    label_about->setMaximumWidth(500);
    label_about->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label_about->setWordWrap(true);

    // username
    label_username = new QLabel(QString::fromStdString(user->preview().login().c_str()));

    // genres
    std::string genres_str = "Favorite genres: ";

    if (!user->favorite_genres().empty()) {
        for (const auto& genre : user->favorite_genres()) {
            std::string favorite_genres_str = common::Genre_Name(genre);
            genres_str += favorite_genres_str + ", ";
        }
        genres_str = genres_str.substr(0, genres_str.size()-2);
    }

    QHBoxLayout *genresLayout = new QHBoxLayout;
    label_favorite_genres = new QLabel(QString::fromStdString(genres_str));
    genresLayout->addWidget(label_favorite_genres);
    
    // spacer
    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    
    // buttons
    QPushButton *inviteButton = new QPushButton("Send Invitation");
    QPushButton *subscribeButton = new QPushButton("Subscribe");
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(inviteButton);
    buttonsLayout->addWidget(subscribeButton);

    // left layout
    leftBox->addWidget(label_photo);
    leftBox->addWidget(label_status);
    leftBox->addWidget(label_about);
    leftBox->addItem(verticalSpacer);
    leftBox->addLayout(buttonsLayout);

    // right layout
    rightBox->addWidget(label_username);
    rightBox->addLayout(genresLayout);

    build_scenes();
   
    connect(inviteButton, &QPushButton::clicked, this, &OtherPage::sendInvite);
    connect(subscribeButton, &QPushButton::clicked, this, &OtherPage::Subscribe);
}

OtherPage::~OtherPage()
{
    delete ui;
}