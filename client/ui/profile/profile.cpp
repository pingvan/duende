#include "profile.h"
#include "ui_profile.h"

Profile::Profile(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::Profile), manager(managerPtr) {
    ui->setupUi(this);

    qDebug() << "Profile page start\n";

    user = manager->new_client.get_user_full_page(manager->ID);

    assert(user != nullptr);

    build();

    qDebug() << "Profile page end\n";

    genresList = new QListWidget;
    genresList->setStyleSheet("border: none;");
    genresList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    genresList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    genresList->setViewMode(QListView::IconMode);
    genresList->setFlow(QListView::LeftToRight);
    genresLayout->addWidget(genresList);
    update_favorite_genres();
}

void Profile::button_edit_clicked() {
    editMode = true;
    update_favorite_genres();
    // replace labels with lineEdits
    lineEdit_status->setText(label_status->text());
    lineEdit_username->setText(label_username->text());
    textEdit_about->setText(label_about->text());
    lineEdit_status->show();
    label_status->hide();
    textEdit_about->show();
    label_about->hide();
    lineEdit_username->show();
    label_username->hide();
    button_edit_genres->show();
    button_save->setVisible(true);
    button_edit->setVisible(false);
    leftBox->replaceWidget(label_status, lineEdit_status);
    leftBox->replaceWidget(label_about, textEdit_about);
    rightBox->replaceWidget(label_username, lineEdit_username);

    // update scenes
    build_scenes(true);
}

void Profile::button_save_clicked() {
    editMode = false;
    update_favorite_genres();
    // update user
    user = manager->new_client.get_user_full_page(manager->ID);

    // change user data
    manager->new_client.change_username(
        manager->ID, lineEdit_username->text().toStdString()
    );
    manager->username = lineEdit_username->text().toStdString();
    manager->new_client.change_quote(
        manager->ID, lineEdit_status->text().toStdString()
    );
    manager->new_client.change_bio(
        manager->ID, textEdit_about->toPlainText().toStdString()
    );

    // replace lineEdits with labels
    label_status->setText(lineEdit_status->text());
    label_about->setText(textEdit_about->toPlainText());
    label_username->setText(lineEdit_username->text());
    label_status->show();
    lineEdit_status->hide();
    label_about->show();
    textEdit_about->hide();
    label_username->show();
    lineEdit_username->hide();
    button_edit->setVisible(true);
    button_edit_genres->hide();
    button_save->setVisible(false);
    leftBox->replaceWidget(lineEdit_status, label_status);
    leftBox->replaceWidget(textEdit_about, label_about);
    rightBox->replaceWidget(lineEdit_username, label_username);

    // update scenes
    build_scenes();
}

void Profile::plusItem_favActors_clicked() {
    // open search window
    Actors_SearchWindow *searchWindow = new Actors_SearchWindow(manager, this);
    searchWindow->setWindowModality(Qt::ApplicationModal);
    searchWindow->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    searchWindow->move(
        QGuiApplication::screens().at(0)->geometry().center() -
        searchWindow->rect().center()
    );
    searchWindow->show();
}

void Profile::plusItem_watchlist_clicked() {
    // open search window
    Movies_SearchWindow *searchWindow = new Movies_SearchWindow(manager, this);
    searchWindow->setWindowModality(Qt::ApplicationModal);
    searchWindow->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    searchWindow->move(
        QGuiApplication::screens().at(0)->geometry().center() -
        searchWindow->rect().center()
    );
    searchWindow->show();
}

void Profile::button_edit_genres_clicked() {
    // open search window
    genres_searchWindow = new Genres_SearchWindow(manager, this);
    connect(
        genres_searchWindow, &Genres_SearchWindow::windowClosed, this,
        &Profile::update_favorite_genres
    );
    genres_searchWindow->setWindowModality(Qt::ApplicationModal);
    genres_searchWindow->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    genres_searchWindow->move(
        QGuiApplication::screens().at(0)->geometry().center() -
        genres_searchWindow->rect().center()
    );
    genres_searchWindow->show();
}

void Profile::update_favorite_genres() {
    genresList->clear();
    genresList->setSpacing(10);

    disconnect(genresList, &QListWidget::itemDoubleClicked, this, nullptr);

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
    if (editMode) {
        connect(
            genresList, &QListWidget::itemDoubleClicked,
            [this](QListWidgetItem *item) {
                int row = genresList->row(item);
                std::string genre_name = item->text().toStdString();
                manager->new_client.remove_genre(manager->ID, genre_name);
                genresList->takeItem(row);
                delete item;
            }
        );
    }
}

void Profile::build_scenes(bool edit_mode) {
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
    actors_text->setText("Your favourite actors");
    actors_text->setStyleSheet("font-size: 14px; color: #8023a8;");

    movies_text = new QLabel();
    movies_text->setText("Your favourite movies");
    movies_text->setStyleSheet("font-size: 14px; color: #8023a8;");

    int start_actors = 0;
    int start_films = 0;

    // add plus item in edit mode
    if (edit_mode) {
        QPixmap plusPixmap(":/img/plus.png");
        ClickablePixmapItem *plusItem_favActors = new ClickablePixmapItem(
            plusPixmap.scaledToHeight(view_favActors->height())
        );
        ClickablePixmapItem *plusItem_watchlist = new ClickablePixmapItem(
            plusPixmap.scaledToHeight(view_watchlist->height())
        );
        connect(
            plusItem_favActors, &ClickablePixmapItem::clicked, this,
            &Profile::plusItem_favActors_clicked
        );
        connect(
            plusItem_watchlist, &ClickablePixmapItem::clicked, this,
            &Profile::plusItem_watchlist_clicked
        );
        plusItem_favActors->setPos(0, 0);
        plusItem_watchlist->setPos(0, 0);
        plusItem_favActors->setCursor(Qt::PointingHandCursor);
        plusItem_watchlist->setCursor(Qt::PointingHandCursor);
        scene_favActors->addItem(plusItem_favActors);
        scene_watchlist->addItem(plusItem_watchlist);
        start_actors = plusItem_favActors->boundingRect().width() + 10;
        start_films = plusItem_watchlist->boundingRect().width() + 10;
    }

    // build favorite actors
    for (auto it = favActorsImages.rbegin(); it != favActorsImages.rend();
         ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(
            pixmap.scaledToHeight(view_favActors->height())
        );
        item->setPos(start_actors, 0);
        scene_favActors->addItem(item);
        start_actors += item->boundingRect().width() + 10;
    }

    view_favActors->setScene(scene_favActors);
    view_favActors->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_favActors->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // build watch list
    for (auto it = watchlistImages.rbegin(); it != watchlistImages.rend();
         ++it) {
        QPixmap pixmap(*it);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(
            pixmap.scaledToHeight(view_watchlist->height())
        );

        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(10);
        effect->setOffset(0);
        item->setGraphicsEffect(effect);

        item->setPos(start_films, 0);
        scene_watchlist->addItem(item);

        start_films += item->boundingRect().width() + 20;  // Увеличено до 20
    }

    view_watchlist->setScene(scene_watchlist);
    view_watchlist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_watchlist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // right layout
    rightBox->addWidget(actors_text);
    rightBox->addWidget(view_favActors);
    rightBox->addWidget(movies_text);
    rightBox->addWidget(view_watchlist);
}

void Profile::build() {
    // main layout
    leftBox = new QVBoxLayout;
    rightBox = new QVBoxLayout;
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addLayout(leftBox, 0, 0);
    mainLayout->addLayout(rightBox, 0, 1);
    mainLayout->setColumnMinimumWidth(0, 500);
    mainLayout->setColumnStretch(0, 0);
    this->setLayout(mainLayout);

    QLabel *greeting = new QLabel();
    greeting->setText(
        "Hello, " + QString::fromStdString(user->preview().login().c_str()) +
        "!"
    );
    greeting->setStyleSheet("font-size: 50px; color: #8023a8;");
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
    label_status->setStyleSheet(
        "color: #8023a8; border-radius: 5px; border: 1px solid #8023a8; "
        "font-size: 14px;"
    );
    lineEdit_status = new QLineEdit();
    lineEdit_status->setMaximumWidth(500);
    lineEdit_status->hide();

    // about
    label_about = new QLabel(QString::fromStdString(user->about().c_str()));
    label_about->setMaximumWidth(500);
    label_about->setMaximumHeight(500);
    label_about->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label_about->setStyleSheet(
        "color: #8023a8; border-radius: 5px; border: 1px solid #8023a8; "
        "font-size: 14px;"
    );
    label_about->setWordWrap(true);
    textEdit_about = new QTextEdit();
    textEdit_about->setMaximumWidth(500);
    textEdit_about->hide();

    lineEdit_status->setPlaceholderText("My favourite quote");
    textEdit_about->setPlaceholderText("About me");

    // username
    label_username =
        new QLabel(QString::fromStdString(user->preview().login().c_str()));

    lineEdit_username = new QLineEdit();
    lineEdit_username->hide();

    // genres
    std::string genres_str = "Favorite genres: ";

    genresLayout = new QHBoxLayout;

    button_edit_genres = new QPushButton("Add");
    button_edit_genres->setFixedWidth(100);
    button_edit_genres->setCursor(Qt::PointingHandCursor);
    button_edit_genres->hide();
    genresLayout->addWidget(genresList);
    genresLayout->addWidget(button_edit_genres);

    connect(
        button_edit_genres, &QPushButton::clicked, this,
        &Profile::button_edit_genres_clicked
    );
    label_username->setStyleSheet(
        "background-color: #8023a8; color: white; border-radius: 5px; "
        "font-size: 16px;"
    );
    label_username->setFixedWidth(150);
    label_username->setFixedHeight(30);
    label_username->setAlignment(Qt::AlignCenter);

    // edit button
    button_edit = new QPushButton("Edit");
    button_edit->setCursor(Qt::PointingHandCursor);
    connect(
        button_edit, &QPushButton::clicked, this, &Profile::button_edit_clicked
    );

    // save button
    button_save = new QPushButton("Save");
    button_save->setCursor(Qt::PointingHandCursor);
    connect(
        button_save, &QPushButton::clicked, this, &Profile::button_save_clicked
    );
    button_save->setVisible(false);

    // spacer
    QSpacerItem *verticalSpacer =
        new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // left layout
    leftBox->addWidget(greeting);
    leftBox->addWidget(label_photo);
    leftBox->addWidget(label_status);
    leftBox->addWidget(label_about);
    leftBox->addItem(verticalSpacer);
    leftBox->addWidget(button_edit);
    leftBox->addWidget(button_save);

    genres_text = new QLabel();
    genres_text->setText("Your favourite genres");
    genres_text->setStyleSheet("font-size: 14px; color: #8023a8;");

    // right layout
    rightBox->addWidget(label_username);
    rightBox->addWidget(lineEdit_username);
    rightBox->addWidget(genres_text);
    rightBox->addLayout(genresLayout);

    // gallery
    favActorsImages << ":/img/tarantino.jpg" << ":/img/tarantino.jpg"
                    << ":/img/tarantino.jpg";
    watchlistImages << ":/img/titanik.jpeg" << ":/img/titanik.jpeg"
                    << ":/img/titanik.jpeg";

    // scenes
    build_scenes();
}

Profile::~Profile() {
    delete ui;
}