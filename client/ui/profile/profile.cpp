#include "profile.h"
#include "ui_profile.h"

// i will change
bool was_watchlist_changed = false;

std::string repeat_symbols(const std::string &str, int times) {
    std::string result;
    for (int i = 0; i < times; ++i) {
        result += str;
    }
    return result;
}

class MyGraphicsItem : public QGraphicsPixmapItem {
public:
    MyGraphicsItem(
        const QPixmap &pixmap,
        const std::string &title,
        int rating,
        int movieId,
        Manager *manager,
        QGraphicsItem *parent = nullptr
    )
        : QGraphicsPixmapItem(pixmap, parent),
          movieId(movieId),
          manager(manager),
          isEditingMode(false) {
        QGraphicsTextItem *titleItem =
            new QGraphicsTextItem(QString::fromStdString(title), this);
        titleItem->setPos(0, pixmap.height());

        QGraphicsSimpleTextItem *ratingItem = new QGraphicsSimpleTextItem(
            QString::fromStdString(
                repeat_symbols("★", rating) + repeat_symbols("☆", (10 - rating))
            ),
            this
        );
        ratingItem->setPos(
            0, pixmap.height() + titleItem->boundingRect().height()
        );
    }

    void setEditingMode(bool mode) {
        isEditingMode = mode;
    }

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override {
        if (!isEditingMode || !scene()) {
            return;
        }
        was_watchlist_changed = true;
        int movieId = data(Qt::UserRole).toInt();
        int rating = data(Qt::UserRole + 1).toInt();
        QGraphicsProxyWidget *proxy =
            data(Qt::UserRole + 2).value<QGraphicsProxyWidget *>();
        qDebug() << "Double click detected. Removing movie with ID:" << movieId;
        manager->new_client.remove_movie(manager->ID, movieId, rating);
        // auto deleted_movie = remove(
        //     manager->watchlist.begin(), manager->watchlist.end(),
        //     movie{movieId, rating}
        // );
        // manager->watchlist.erase(deleted_movie, manager->watchlist.end());
        auto it = std::find_if(
            manager->watchlist.begin(), manager->watchlist.end(),
            [movieId, rating](const movie &m) {
                return m.id == movieId && m.ratio == rating;
            }
        );

        if (it != manager->watchlist.end()) {
            // Удаляем найденный объект movie
            manager->watchlist.erase(it);
            qDebug() << "Movie removed successfully.";
        } else {
            qDebug() << "Movie not found in watchlist.";
        }
        qDebug() << "Movie removed successfully.";
        scene()->removeItem(this);
        delete this;
    }

private:
    int movieId;
    Manager *manager;
    bool isEditingMode;
};

class MyGraphicsItem2 : public QGraphicsPixmapItem {
public:
    MyGraphicsItem2(
        const QPixmap &pixmap,
        const std::string &name,
        int actorId,
        Manager *manager,
        QGraphicsItem *parent = nullptr
    )
        : QGraphicsPixmapItem(pixmap, parent),
          actorId(actorId),
          manager(manager),
          isEditingMode(false) {
        QGraphicsTextItem *nameItem =
            new QGraphicsTextItem(QString::fromStdString(name), this);
        nameItem->setPos(0, pixmap.height());
    }

    void setEditingMode(bool mode) {
        isEditingMode = mode;
    }

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override {
        if (!isEditingMode || !scene()) {
            return;
        }

        int actorId = data(Qt::UserRole).toInt();
        QGraphicsProxyWidget *proxy =
            data(Qt::UserRole + 1).value<QGraphicsProxyWidget *>();
        qDebug() << "Double click detected. Removing actor with ID:" << actorId;
        manager->new_client.remove_actor(manager->ID, actorId);
        qDebug() << "Actor removed successfully.";
        scene()->removeItem(this);
        delete this;
    }

private:
    int actorId;
    Manager *manager;
    bool isEditingMode;
};

Profile::Profile(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::Profile), manager(managerPtr) {
    ui->setupUi(this);

    qDebug() << "Profile page start\n";

    user = manager->new_client.get_user_full_page(manager->ID);

    assert(user != nullptr);

    build();

    qDebug() << "Profile page end\n";
}

void Profile::button_edit_clicked() {
    // replace labels with lineEdits
    label_photo->setCursor(Qt::PointingHandCursor);
    genresList->setCursor(Qt::PointingHandCursor);
    editMode = true;
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
    label_photo->setToolTip(tr("Click to upload a new photo"));
    genresList->setToolTip(tr("Double click to delete this genre"));
    connection = connect(
        genresList, &QListWidget::itemDoubleClicked,
        [this](QListWidgetItem *item) {
            int row = genresList->row(item);
            std::string genre_name = item->text().toLower().toStdString();
            manager->new_client.remove_genre(manager->ID, genre_name);
            genresList->takeItem(row);
            delete item;
            // update_favorite_genres();
        }
    );
    // update scenes
    build_scenes(true);
}

void Profile::button_save_clicked() {
    editMode = false;
    disconnect(connection);
    label_photo->setCursor(Qt::ArrowCursor);
    genresList->setCursor(Qt::ArrowCursor);
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
    greeting->clear();
    greeting->setText(QString("Hello, %1!").arg(lineEdit_username->text()));
    update_favorite_genres();
    update_actors(0, false);
    if (was_watchlist_changed) {
        update_watchlist(0, false);
        was_watchlist_changed = false;
    }

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
    label_photo->setToolTip("");
    genresList->setToolTip("");

    // update scenes
    build_scenes();
}

bool Profile::eventFilter(QObject *watched, QEvent *event) {
    if (watched == label_photo && event->type() == QEvent::MouseButtonPress) {
        uploadPhoto();
        return true;
    }
    return false;
}

void Profile::uploadPhoto() {
    if (!editMode) {
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)")
    );
    if (fileName != "") {
        QPixmap pixmap(fileName);
        manager->new_client.change_photo(manager->ID, fileName.toStdString());

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
    }
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
    was_watchlist_changed = true;
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

void Profile::update_watchlist(int start_films, bool edit_mode) {
    // TODO: without new vector
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
        auto *full_page =
            manager->new_client.get_film_full_page(movie.preview().film_id());
        QUrl url;
        if (full_page == nullptr) {
            qDebug() << "aaaaaaaaa";
            url = movie.preview().preview_cover_url().c_str();
        } else {
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

            MyGraphicsItem *item = new MyGraphicsItem(
                pixmap, movie.preview().title(), movie.ratio(),
                movie.preview().film_id(), manager
            );
            int32_t movieId = movie.preview().film_id();
            item->setData(Qt::UserRole, movieId);
            if (edit_mode) {
                item->setToolTip(tr("Double click to delete this movie"));
                item->setCursor(Qt::PointingHandCursor);
                item->setEditingMode(true);
            } else {
                item->setToolTip("");
                item->setCursor(Qt::ArrowCursor);
                item->setEditingMode(false);
            }
            item->setPos(start_films, 0);
            scene_watchlist->addItem(item);
        } else {
            qDebug() << "Failed to load image from url:"
                     << reply->errorString();
        }
        reply->deleteLater();
        start_films += 200;
    }
}

void Profile::update_actors(int start_actors, bool edit_mode) {
    for (const auto &actor : user->favorite_actors()) {
        QPixmap pixmap;
        pixmap.load(":/img/tarantino.jpg");
        pixmap = pixmap.scaledToHeight(view_favActors->height() - 50);

        MyGraphicsItem2 *item = new MyGraphicsItem2(
            pixmap, actor.full_name(), actor.actor_id(), manager
        );
        int32_t actorId = actor.actor_id();
        qDebug() << "Setting actor:" << actorId;
        item->setData(Qt::UserRole, actorId);
        if (edit_mode) {
            item->setToolTip(tr("Double click to delete this actor"));
            item->setCursor(Qt::PointingHandCursor);
            item->setEditingMode(true);
        } else {
            item->setToolTip("");
            item->setCursor(Qt::ArrowCursor);
            item->setEditingMode(false);
        }
        item->setPos(start_actors, 0);
        scene_favActors->addItem(item);

        start_actors += item->boundingRect().width() + 20;
    }
}

void Profile::button_edit_genres_clicked() {
    // open search window
    genres_searchWindow = new Genres_SearchWindow(manager, this);
    // connect(
    //     genres_searchWindow, &Genres_SearchWindow::windowClosed, this,
    //     &Profile::update_favorite_genres
    // );
    connect(
        genres_searchWindow, &Genres_SearchWindow::genreAdded, this,
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

    // disconnect(genresList, &QListWidget::itemDoubleClicked, this, nullptr);

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
    view_favActors->setFrameShape(QFrame::NoFrame);
    scene_favActors = new QGraphicsScene();

    view_watchlist = new QGraphicsView();
    view_watchlist->setFrameShape(QFrame::NoFrame);
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
    actors_text->setStyleSheet("font-size: 20px; color: black;");

    movies_text = new QLabel();
    movies_text->setText("Your watchlist");
    movies_text->setStyleSheet("font-size: 20px; color: black;");

    int start_actors = 0;
    int start_films = 0;

    view_favActors->setScene(scene_favActors);
    view_favActors->setFixedHeight(150);
    view_favActors->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_favActors->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view_watchlist->setScene(scene_watchlist);
    view_watchlist->setFixedHeight(300);
    view_watchlist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_watchlist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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

    update_actors(start_actors, edit_mode);

    update_watchlist(start_films, edit_mode);

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

    greeting = new QLabel();
    greeting->setText(
        "Hello, " + QString::fromStdString(user->preview().login().c_str()) +
        "!"
    );
    common::UserPreview user2 = user->preview();
    greeting->setStyleSheet("font-size: 50px; color: #8023a8;");
    label_photo = new QLabel();
    label_photo->installEventFilter(this);
    std::string photo_data(user2.photo().begin(), user2.photo().end());
    QPixmap pixmap;
    if (photo_data.empty()) {
        pixmap.load(":/img/person-circle2.png");
    } else {
        pixmap.loadFromData(
            reinterpret_cast<const uchar *>(photo_data.data()),
            photo_data.size()
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
    label_status->setMinimumHeight(40);
    label_status->setStyleSheet(
        "color: black;"
        "font-size: 20px;"
    );
    lineEdit_status = new QLineEdit();
    lineEdit_status->setMaximumWidth(500);
    lineEdit_status->hide();

    // about
    label_about = new QLabel(QString::fromStdString(user->about().c_str()));
    label_about->setMaximumWidth(500);
    label_about->setMaximumHeight(300);
    label_about->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label_about->setStyleSheet(
        "color: #8023a8; "
        "font-size: 20px;"
    );
    label_about->setWordWrap(true);
    textEdit_about = new QTextEdit();
    textEdit_about->hide();

    lineEdit_status->setPlaceholderText("My favourite quote");
    textEdit_about->setPlaceholderText("About me");
    textEdit_about->setStyleSheet("color: #8023a8; font-size: 18px;");
    textEdit_about->setMaximumHeight(300);

    // username
    label_username =
        new QLabel(QString::fromStdString(user->preview().login().c_str()));

    lineEdit_username = new QLineEdit();
    lineEdit_username->hide();

    // genres
    std::string genres_str = "Favorite genres: ";

    genresLayout = new QHBoxLayout;
    genresList = new QListWidget;
    genresList->setStyleSheet("border: none;");
    genresList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    genresList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    genresList->setViewMode(QListView::IconMode);
    genresList->setFlow(QListView::LeftToRight);
    update_favorite_genres();

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
        "color: #8023a8;"
        "font-size: 24px;"
    );
    label_username->setFixedHeight(40);

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
    genres_text->setStyleSheet("font-size: 20px; color: black;");

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