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

    qDebug() << "ID: " << id;

    authservice::UserFullPage *user = manager->new_client.get_user_full_page(id);

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

    label_about = new QLabel(QString::fromStdString(user->about().c_str()));
    label_about->setMaximumWidth(500);
    label_about->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label_about->setWordWrap(true);

    label_username = new QLabel(QString::fromStdString(user->preview().login().c_str()));

    std::string genres_str = "Favorite genres: ";

    if (!user->favorite_genres().empty()) {
        for (const auto& genre : user->favorite_genres()) {
            std::string favorite_genres_str = authservice::Genre_Name(genre);
            genres_str += favorite_genres_str + ", ";
        }
        genres_str = genres_str.substr(0, genres_str.size()-2);
    }

    QHBoxLayout *genresLayout = new QHBoxLayout;
    label_favorite_genres = new QLabel(QString::fromStdString(genres_str));
    genresLayout->addWidget(label_favorite_genres);

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

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    
    QPushButton *inviteButton = new QPushButton("Send Invitation");
    QPushButton *subscribeButton = new QPushButton("Subscribe");
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(inviteButton);
    buttonsLayout->addWidget(subscribeButton);

    leftBox->addWidget(label_photo);
    leftBox->addWidget(label_status);
    leftBox->addWidget(label_about);
    leftBox->addItem(verticalSpacer);
    leftBox->addLayout(buttonsLayout);

    rightBox->addWidget(label_username);
    rightBox->addLayout(genresLayout);
    rightBox->addWidget(view_favActors);
    rightBox->addWidget(view_watchlist);
   
    connect(inviteButton, &QPushButton::clicked, this, &OtherPage::sendInvite);

    this->setLayout(mainLayout);

    qDebug() << "Other page end";
}

OtherPage::~OtherPage()
{
    delete ui;
}

void OtherPage::sendInvite() 
{
    assert(manager != nullptr);
    manager->new_client.send_invite(manager->ID, id, "Hello", invitation::InviteStatus::SENDED);
}

void OtherPage::updatePage(int newId)
{
    id = newId;

    qDebug() << "Update page start";

    qDebug() << "New ID: " << id;

    authservice::UserFullPage *user = manager->new_client.get_user_full_page(id);

    assert(user != nullptr);

    label_status->setText(QString::fromStdString(user->quote().c_str()));
    label_about->setText(QString::fromStdString(user->about().c_str()));
    label_username->setText(QString::fromStdString(user->preview().login().c_str()));

    std::string genres_str = "Favorite genres: ";

    if (!user->favorite_genres().empty()) {
        for (const auto& genre : user->favorite_genres()) {
            std::string favorite_genres_str = authservice::Genre_Name(genre);
            genres_str += favorite_genres_str + ", ";
        }
        genres_str = genres_str.substr(0, genres_str.size()-2);
    }

    label_favorite_genres->setText(QString::fromStdString(genres_str));

    scene_favActors->clear();
    int x = 0;
    for (auto it = favActorsImages.rbegin(); it != favActorsImages.rend(); ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap.scaledToHeight(view_favActors->height()));
        item->setPos(x, 0);
        scene_favActors->addItem(item);
        x += item->boundingRect().width() + 10;
    }

    scene_watchlist->clear();
    x = 0;
    for (auto it = watchlistImages.rbegin(); it != watchlistImages.rend(); ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap.scaledToHeight(view_watchlist->height()));
        item->setPos(x, 0);
        scene_watchlist->addItem(item);
        x += item->boundingRect().width() + 10;
    }

    qDebug() << "Update page end";
}
