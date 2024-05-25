#include "otherpage.h"
#include "ui_otherpage.h"

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
    if (manager->new_client.invite_already_sent(manager->ID, id)) {
        QMessageBox msgBox;
        msgBox.setText("The invitation has already been sent.");
        msgBox.exec();
    } else {
        manager->new_client.send_invite(
            manager->ID, id, manager->username, login
        );
    }
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
    label_status->setStyleSheet(
        "color: #8023a8; border-radius: 5px; border: 1px solid #8023a8; "
        "font-size: 14px;"
    );
    label_about->setText(QString::fromStdString(user->about().c_str()));
    label_about->setMaximumWidth(500);
    label_about->setMaximumHeight(500);
    label_about->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label_about->setStyleSheet(
        "color: #8023a8; border-radius: 5px; border: 1px solid #8023a8; "
        "font-size: 14px;"
    );
    label_username->setText(
        QString::fromStdString(user->preview().login().c_str())
    );
    label_username->setStyleSheet(
        "background-color: #8023a8; color: white; border-radius: 5px; "
        "font-size: 16px;"
    );
    label_username->setFixedWidth(150);
    label_username->setFixedHeight(30);
    label_username->setAlignment(Qt::AlignCenter);

    // update genres
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

    // update scenes
    build_scenes();

    qDebug() << "Update page end";
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

    view_favActors = new QGraphicsView();
    scene_favActors = new QGraphicsScene();
    view_watchlist = new QGraphicsView();
    scene_watchlist = new QGraphicsScene();

    // fav actors
    favActorsImages.clear();
    favActorsImages << ":/img/tarantino.jpg" << ":/img/tarantino.jpg"
                    << ":/img/tarantino.jpg";
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

    view_favActors->setScene(scene_favActors);
    view_favActors->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_favActors->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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

    // watchlist
    watchlistImages.clear();
    watchlistImages << ":/img/titanik.jpeg" << ":/img/titanik.jpeg"
                    << ":/img/titanik.jpeg";
    x = 0;
    for (auto it = watchlistImages.rbegin(); it != watchlistImages.rend();
         ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(
            pixmap.scaledToHeight(view_watchlist->height())
        );
        item->setPos(x, 0);
        scene_watchlist->addItem(item);
        x += item->boundingRect().width() + 10;
    }

    actors_text = new QLabel();
    actors_text->setText(
        QString::fromStdString(user->preview().login().c_str()) +
        "'s favourite actors"
    );
    actors_text->setStyleSheet("font-size: 14px; color: #8023a8;");

    movies_text = new QLabel();
    movies_text->setText(
        QString::fromStdString(user->preview().login().c_str()) +
        "'s favourite movies"
    );
    movies_text->setStyleSheet("font-size: 14px; color: #8023a8;");

    view_watchlist->setScene(scene_watchlist);
    view_watchlist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_watchlist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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
    QPixmap pixmap(":/img/robertdeniro.jpg");

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
    genres_text->setStyleSheet("font-size: 14px; color: #8023a8;");
    QHBoxLayout *genresLayout = new QHBoxLayout;
    genresLayout->addWidget(genresList);
    genresList->show();

    // spacer
    QSpacerItem *verticalSpacer =
        new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // buttons
    QPushButton *inviteButton = new QPushButton("Send Invitation");
    QPushButton *subscribeButton = new QPushButton("Subscribe");
    QPushButton *blockButton = new QPushButton();
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
    connect(blockButton, &QPushButton::clicked, this, [this, blockButton]() {
        showBlockDialog(blockButton);
    });
}

OtherPage::~OtherPage() {
    delete ui;
}