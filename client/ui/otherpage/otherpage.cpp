#include "otherpage.h"
#include "ui_otherpage.h"

std::string repeat_symbol(const std::string &str, int times) {
    std::string result;
    for (int i = 0; i < times; ++i) {
        result += str;
    }
    return result;
}

OtherPage::OtherPage(int id, Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::OtherPage), id(id), manager(managerPtr) {
    ui->setupUi(this);

    qDebug() << "Other page start";

    user = manager->new_client.get_user_full_page(id);

    login = user->preview().login();

    assert(user != nullptr);

    build();

    qDebug() << "Other page end";
}

void OtherPage::sendInvite() {
    assert(manager != nullptr);
    if (manager->new_client.is_blocked(id, manager->ID)) {
        QMessageBox::information(
            this, "Blocked User", "You should unblock this user first."
        );
        return;
    }
    manager->new_client.send_invite(manager->ID, id);
}

void OtherPage::showBlockDialog(QPushButton *blockButton) {
    QMessageBox::StandardButton reply;
    if (manager->new_client.is_blocked(id, manager->ID)) {
        reply = QMessageBox::question(
            this, "Unblock User", "Are you sure you want to unblock this user?",
            QMessageBox::Yes | QMessageBox::No
        );
        if (reply == QMessageBox::Yes) {
            manager->new_client.unblock(manager->ID, id);
            blockButton->setStyleSheet(
                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: "
                "0 #b24ee7, stop: 1 #982ecb); "
            );
        }
    } else {
        reply = QMessageBox::question(
            this, "Block User", "Are you sure you want to block this user?",
            QMessageBox::Yes | QMessageBox::No
        );
        if (reply == QMessageBox::Yes) {
            manager->new_client.block(manager->ID, id);
            manager->new_client.unsubscribe(manager->ID, id);
            manager->new_client.unsubscribe(id, manager->ID);
            // delete invite if it was?..
            blockButton->setStyleSheet(
                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: "
                "0 #8023A8, stop: 1 #691e8a); "
            );
            subscribeButton->setText("Subscribe");
        }
    }
}

void OtherPage::toggleSubscription() {
    assert(manager != nullptr);
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (manager->new_client.is_blocked(id, manager->ID)) {
        QMessageBox::information(
            this, "Blocked User", "You should unblock this user first."
        );
        return;
    }
    if (button) {
        if (button->text() == "Subscribe") {
            manager->new_client.subscribe(manager->ID, id);
            qDebug() << "Subscribed";
            button->setText("Unsubscribe");
        } else {
            manager->new_client.unsubscribe(manager->ID, id);
            qDebug() << "Unsubscribed";
            button->setText("Subscribe");
        }
    }
    updatePage(id);
}

void OtherPage::updatePage(int newId) {
    qDebug() << "Update page start";

    id = newId;

    user = manager->new_client.get_user_full_page(id);

    login = user->preview().login();

    assert(user != nullptr);

    // update all labels
    label_status->setText(QString::fromStdString(user->quote().c_str()));
    label_status->setMaximumWidth(500);
    // label_status->setStyleSheet(
    //     "color: #8023a8; border-radius: 5px; border: 1px solid #8023a8; "
    //     "font-size: 14px;"
    // );
    label_status->setStyleSheet(
        "color: black;"
        "font-size: 20px;"
    );
    label_about->setText(QString::fromStdString(user->about().c_str()));
    label_about->setMaximumWidth(500);
    label_about->setMaximumHeight(500);
    label_about->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // label_about->setStyleSheet(
    //     "color: #8023a8; border-radius: 5px; border: 1px solid #8023a8; "
    //     "font-size: 14px;"
    // );
    label_about->setStyleSheet(
        "color: #8023a8; "
        "font-size: 20px;"
    );
    label_username->setText(
        QString::fromStdString(user->preview().login().c_str())
    );
    label_username->setStyleSheet(
        "color: #8023a8; "
        "font-size: 24px;"
    );
    label_username->setFixedHeight(30);
    // label_username->setAlignment(Qt::AlignCenter);
    genres_text->setText(
        QString::fromStdString(user->preview().login().c_str()) +
        "'s favourite genres"
    );

    // update genres
    genresList->clear();
    genresList->setSpacing(10);
    for (const auto &genre : user->favorite_genres()) {
        QListWidgetItem *item =
            new QListWidgetItem(common::Genre_Name(genre).c_str());
        item->setTextAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(30);
        // font.setBold(true);
        item->setFont(font);
        item->setForeground(Qt::white);
        item->setBackground(QBrush(QColor("#8023a8")));
        genresList->addItem(item);
    }

    if (manager->new_client.is_blocked(id, manager->ID)) {
        blockButton->setStyleSheet(
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
            "#8023A8, stop: 1 #691e8a); "
        );
    } else {
        blockButton->setStyleSheet(
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
            "#b24ee7, stop: 1 #982ecb); "
        );
    }
    if (manager->new_client.is_subscribed(manager->ID, id)) {
        subscribeButton->setText("Unsubscribe");
    } else {
        subscribeButton->setText("Subscribe");
    }
    // subscribeButton->setChecked(
    //     manager->new_client.is_subscribed(manager->ID, id)
    // );
    // subscribeButton->setText(
    //     subscribeButton->isChecked() ? "Unsubscribe" : "Subscribe"
    // );

    // update scenes
    build_scenes();

    qDebug() << "Update page end";
}

void OtherPage::update_watchlist() {
    int start_films = 0;
    std::vector<common::FilmOnUserPage> current_watchlist;
    for (const auto &movie : user->watched_films()) {
        current_watchlist.push_back(movie);
    }
    std::sort(
        current_watchlist.begin(), current_watchlist.end(),
        [](const common::FilmOnUserPage &a, const common::FilmOnUserPage &b) {
            return a.ratio() > b.ratio();
        }
    );
    for (const auto &movie : current_watchlist) {
        QPixmap pixmap;
        QNetworkAccessManager *nam = new QNetworkAccessManager(this);
        auto full_page =
            manager->new_client.get_film_full_page(movie.preview().film_id());
        QUrl url;
        if (full_page == nullptr){
            qDebug() << "aaaaaaaaa";
            url = movie.preview().preview_cover_url().c_str();
        }
        else{
            url = full_page->full_page_cover_url().c_str();
        }
        QNetworkReply *reply = nam->get(QNetworkRequest(url));

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QImage image;
            image.loadFromData(data);
            pixmap = QPixmap::fromImage(image);
            pixmap = pixmap.scaledToHeight(view_watchlist->height() - 50);

            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
            item->setPos(start_films, 0);
            scene_watchlist->addItem(item);

            QGraphicsTextItem *titleItem = new QGraphicsTextItem(
                QString::fromStdString(movie.preview().title())
            );
            titleItem->setPos(start_films, pixmap.height());
            scene_watchlist->addItem(titleItem);

            QGraphicsTextItem *ratingItem =
                new QGraphicsTextItem(QString::fromStdString(
                    repeat_symbol("★", movie.ratio()) +
                    repeat_symbol("☆", (10 - movie.ratio()))
                ));
            ratingItem->setPos(
                start_films,
                pixmap.height() + titleItem->boundingRect().height()
            );
            scene_watchlist->addItem(ratingItem);
        } else {
            qDebug() << "Failed to load image from url:"
                     << reply->errorString();
        }
        reply->deleteLater();
        start_films += 200;
    }
}

void OtherPage::build_scenes() {
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
    if (actors_text != nullptr) {
        delete actors_text;
        actors_text = nullptr;
    }
    if (movies_text != nullptr) {
        delete movies_text;
        movies_text = nullptr;
    }

    view_favActors = new QGraphicsView();
    scene_favActors = new QGraphicsScene();
    view_watchlist = new QGraphicsView();
    scene_watchlist = new QGraphicsScene();

    // fav actors
    favActorsImages.clear();
    favActorsImages << ":/img/tarantino.jpg" << ":/img/tarantino.jpg"
                    << ":/img/tarantino.jpg";
    view_favActors->setScene(scene_favActors);
    view_favActors->setFixedHeight(150);
    view_favActors->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_favActors->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_watchlist->setFrameShape(QFrame::NoFrame);
    view_favActors->setFrameShape(QFrame::NoFrame);

    view_watchlist->setScene(scene_watchlist);
    view_watchlist->setFixedHeight(300);
    view_watchlist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_watchlist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    int x = 0;
    for (auto it = favActorsImages.rbegin(); it != favActorsImages.rend();
         ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(
            pixmap.scaledToHeight(view_favActors->height())
        );
        item->setPos(x, 0);
        scene_favActors->addItem(item);
        x += item->boundingRect().width() + 10;
    }

    update_watchlist();

    if (actors_text != nullptr) {
        rightBox->removeWidget(actors_text);
        delete actors_text;
        actors_text = nullptr;
    }
    if (movies_text != nullptr) {
        rightBox->removeWidget(movies_text);
        delete movies_text;
        movies_text = nullptr;
    }

    actors_text = new QLabel();
    actors_text->setText(
        QString::fromStdString(user->preview().login().c_str()) +
        "'s favourite actors"
    );
    actors_text->setStyleSheet("font-size: 20px; color: black;");

    movies_text = new QLabel();
    movies_text->setText(
        QString::fromStdString(user->preview().login().c_str()) + "'s watchlist"
    );
    movies_text->setStyleSheet("font-size: 20px; color: black;");

    // add to layout
    rightBox->addWidget(actors_text);
    rightBox->addWidget(view_favActors);
    rightBox->addWidget(movies_text);
    rightBox->addWidget(view_watchlist);
}

void OtherPage::build() {
    genresList = new QListWidget;
    genresList->setStyleSheet("border: none;");
    genresList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    genresList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    genresList->setViewMode(QListView::IconMode);
    genresList->setFlow(QListView::LeftToRight);

    if (genres_text != nullptr) {
        rightBox->removeWidget(genres_text);
        delete genres_text;
        genres_text = nullptr;
    }

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
    common::UserPreview user_preview = user->preview();
    // std::string photo_data(
    //     user_preview.photo().begin(), user_preview.photo().end()
    // );
    QPixmap pixmap;
    if (user_preview.photo().empty()) {
        pixmap.load(":/img/person-circle2.png");
    } else {
        pixmap = QPixmap::fromImage(
            QImage::fromData(QByteArray::fromStdString(user_preview.photo()))
        );
    }

    int size = qMin(pixmap.width(), pixmap.height());
    QPixmap squarePixmap = pixmap.copy(0, 0, size, size);

    QPixmap scaledPixmap = squarePixmap.scaled(
        300, 300, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation
    );

    QPixmap rounded = QPixmap(scaledPixmap.size());
    rounded.fill(Qt::transparent);

    QPainter painter(&rounded);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(scaledPixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(rounded.rect());

    label_photo->setPixmap(rounded);

    // status
    label_status = new QLabel(QString::fromStdString(user->quote().c_str()));
    label_status->setMaximumWidth(500);

    // about
    label_about = new QLabel(QString::fromStdString(user->about().c_str()));
    label_about->setMaximumWidth(500);
    label_about->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label_about->setWordWrap(true);

    // username
    label_username =
        new QLabel(QString::fromStdString(user->preview().login().c_str()));

    // genres
    genresList->clear();
    genresList->setSpacing(10);
    for (const auto &genre : user->favorite_genres()) {
        QListWidgetItem *item =
            new QListWidgetItem(common::Genre_Name(genre).c_str());
        item->setTextAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(20);
        // font.setBold(true);
        item->setFont(font);
        item->setForeground(Qt::white);
        item->setBackground(QBrush(QColor("#8023a8")));
        genresList->addItem(item);
    }

    genres_text = new QLabel();
    genres_text->setText(
        QString::fromStdString(user->preview().login().c_str()) +
        "'s favourite genres"
    );
    genres_text->setStyleSheet("font-size: 20px; color: black;");
    QHBoxLayout *genresLayout = new QHBoxLayout;
    genresLayout->addWidget(genresList);
    genresList->show();

    // spacer
    QSpacerItem *verticalSpacer =
        new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // buttons
    inviteButton = new QPushButton("Send Invitation");
    subscribeButton = new QPushButton("Subscribe");
    blockButton = new QPushButton();
    blockButton->setIcon(QIcon(":/img/ban.png"));
    blockButton->setIconSize(QSize(24, 24));
    blockButton->setFixedSize(40, 40);
    if (manager->new_client.is_blocked(id, manager->ID)) {
        blockButton->setStyleSheet(
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
            "#8023A8, stop: 1 #691e8a); "
        );
    } else {
        blockButton->setStyleSheet(
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
            "#b24ee7, stop: 1 #982ecb); "
        );
    }
    subscribeButton->setChecked(
        manager->new_client.is_subscribed(manager->ID, id)
    );
    subscribeButton->setText(
        subscribeButton->isChecked() ? "Unsubscribe" : "Subscribe"
    );
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(inviteButton);
    buttonsLayout->addWidget(subscribeButton);
    buttonsLayout->addWidget(blockButton);

    // left layout
    leftBox->addWidget(label_photo);
    leftBox->addWidget(label_status);
    leftBox->addWidget(label_about);
    leftBox->addItem(verticalSpacer);
    leftBox->addLayout(buttonsLayout);

    // right layout
    rightBox->addWidget(label_username);
    rightBox->addWidget(genres_text);
    rightBox->addLayout(genresLayout);

    build_scenes();

    connect(inviteButton, &QPushButton::clicked, this, &OtherPage::sendInvite);
    connect(
        subscribeButton, &QPushButton::clicked, this,
        &OtherPage::toggleSubscription
    );
    connect(blockButton, &QPushButton::clicked, this, [this]() {
        showBlockDialog(blockButton);
    });
}

OtherPage::~OtherPage() {
    delete ui;
}