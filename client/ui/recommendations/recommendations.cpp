#include "recommendations.h"
#include "ui_recommendations.h"

class FilmItem : public QGraphicsPixmapItem {
public:
    FilmItem(
        const QPixmap &pixmap,
        const std::string &title,
        int movieId,
        Manager *manager,
        QGraphicsItem *parent = nullptr
    )
        : QGraphicsPixmapItem(pixmap, parent),
          movieId(movieId),
          manager(manager) {
        QGraphicsTextItem *titleItem =
            new QGraphicsTextItem(QString::fromStdString(title), this);
        titleItem->setPos(0, 250);
        titleItem->setTextWidth(150);
        QFont font = titleItem->font();
        font.setPointSize(18);
        titleItem->setFont(font);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        if (manager->new_client.get_film_full_page(movieId) == nullptr) {
            return;
        }
        QDialog *dialog = new QDialog;
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        QVBoxLayout *layout = new QVBoxLayout(dialog);
        QLabel *label = new QLabel(QObject::tr(manager->new_client
                                                   .get_film_full_page(movieId)
                                                   ->description()
                                                   .c_str()));
        QFont font = label->font();
        font.setPointSize(14);
        label->setWordWrap(true);
        label->setMaximumWidth(500);
        layout->addWidget(label);
        dialog->setLayout(layout);
        dialog->setMaximumWidth(500);
        dialog->setWindowTitle(QObject::tr("Short description"));
        dialog->show();
    }

private:
    int movieId;
    Manager *manager;
};

class UserItem : public QGraphicsPixmapItem {
public:
    using Callback = std::function<void(int)>;

    UserItem(
        const QPixmap &pixmap,
        const std::string &username,
        int userId,
        Manager *manager,
        Callback callback,
        QGraphicsItem *parent = nullptr
    )
        : QGraphicsPixmapItem(pixmap, parent),
          userId(userId),
          manager(manager),
          callback(std::move(callback)) {
        QGraphicsTextItem *usernameItem =
            new QGraphicsTextItem(QString::fromStdString(username), this);
        // usernameItem->setPos(0, pixmap.height());
        usernameItem->setTextWidth(pixmap.width());
        usernameItem->setPos(
            (pixmap.width() - usernameItem->boundingRect().width()) / 2,
            pixmap.height()
        );
        QFont font = usernameItem->font();
        font.setPointSize(18);
        usernameItem->setFont(font);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        callback(userId);
    }

private:
    int userId;
    Manager *manager;
    Callback callback;
};

Recommendations::Recommendations(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::Recommendations), manager(managerPtr) {
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search other users...");
    search_line->setStyleSheet(
        "font-size: 18px; color: #5a1e71; border: 1px solid #8023a8; "
        "border-radius: 5px; padding: 5px; text-color: #8023a8;"
    );

    model = new QStringListModel(searchUsers(""), search_line);

    completer = new QCompleter(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    search_line->setCompleter(completer);

    layout->addWidget(search_line);

    QLabel *users_text =
        new QLabel("You may have something in common with these people:");
    users_text->setStyleSheet("font-size: 24px; color: #8023a8;");

    QLabel *movies_text =
        new QLabel("...and you may enjoy watching these movies:");
    movies_text->setStyleSheet("font-size: 24px; color: #8023a8;");

    if (view_films != nullptr) {
        delete view_films;
        view_films = nullptr;
    }
    if (scene_films != nullptr) {
        delete scene_films;
        scene_films = nullptr;
    }
    if (view_users != nullptr) {
        delete view_users;
        view_films = nullptr;
    }
    if (scene_users != nullptr) {
        delete scene_users;
        scene_users = nullptr;
    }
    view_users = new QGraphicsView();
    scene_users = new QGraphicsScene();
    view_users->setScene(scene_users);
    view_users->setFixedHeight(270);
    view_users->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_users->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_users->setFrameShape(QFrame::NoFrame);

    view_films = new QGraphicsView();
    scene_films = new QGraphicsScene();
    view_films->setScene(scene_films);
    view_films->setFixedHeight(350);
    view_films->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_films->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_films->setFrameShape(QFrame::NoFrame);

    updateRecommendations();

    layout->addSpacing(20);
    layout->addWidget(users_text);
    layout->addWidget(view_users);
    layout->addWidget(movies_text);
    layout->addWidget(view_films);

    layout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding)
    );

    connect(
        search_line, &QLineEdit::textChanged, this,
        &Recommendations::updateUserList
    );
    connect(
        completer, QOverload<const QString &>::of(&QCompleter::activated), this,
        &Recommendations::onUserClicked
    );
}

Recommendations::~Recommendations() {
    delete ui;
}

void Recommendations::updateRecommendations() {
    auto recommended_users =
        manager->new_client.get_user_recommendations(manager->ID);
    auto recommended_films =
        manager->new_client.get_film_recommendations(manager->ID);

    int start_users = 0;
    for (const auto &user : recommended_users) {
        if (manager->new_client.is_subscribed(manager->ID, user.user_id()) ||
            manager->new_client.is_blocked(manager->ID, user.user_id()) ||
            manager->new_client.is_blocked(user.user_id(), manager->ID)) {
            continue;
        }
        QPixmap pixmap;
        if (user.photo().empty()) {
            pixmap = QPixmap(":/img/person-circle2.png");
        } else {
            pixmap = QPixmap::fromImage(
                QImage::fromData(QByteArray::fromStdString(user.photo()))
            );
        }
        int size = qMin(pixmap.width(), pixmap.height());
        QPixmap squarePixmap = pixmap.copy(0, 0, size, size);

        QPixmap scaledPixmap = squarePixmap.scaled(
            160, 160, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation
        );

        QPixmap rounded = QPixmap(scaledPixmap.size());
        rounded.fill(Qt::transparent);

        QPainter painter(&rounded);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QBrush(scaledPixmap));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rounded.rect());

        auto *user_item = new UserItem(
            rounded, user.login(), user.user_id(), manager,
            [this, user](int userId) { onUserFromListClicked(user.user_id()); }
        );
        user_item->setData(Qt::UserRole, user.user_id());
        user_item->setPos(start_users, 0);
        user_item->setCursor(Qt::PointingHandCursor);
        scene_users->addItem(user_item);
        start_users += 200;
    }

    int start_films = 0;
    for (const auto &film : recommended_films) {
        if (manager->new_client.is_movie_watched(manager->ID, film.film_id())) {
            continue;
        }
        QPixmap pixmap;
        QNetworkAccessManager *nam = new QNetworkAccessManager(this);
        auto full_page = manager->new_client.get_film_full_page(film.film_id());
        QUrl url;
        bool is_full_page = false;
        if (full_page == nullptr) {
            url = film.preview_cover_url().c_str();
        } else {
            is_full_page = true;
            url = full_page->full_page_cover_url().c_str();
        }
        QNetworkReply *reply = nam->get(QNetworkRequest(url));

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        QImage image;
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            image.loadFromData(data);
        } else {
            image = QImage(":/img/file.PNG");
        }
        pixmap = QPixmap::fromImage(image);
        pixmap = pixmap.scaledToHeight(view_films->height() - 100);

        FilmItem *item =
            new FilmItem(pixmap, film.title(), film.film_id(), manager);
        int32_t movieId = film.film_id();
        item->setData(Qt::UserRole, movieId);
        item->setPos(start_films, 0);
        scene_films->addItem(item);
        reply->deleteLater();
        start_films += 200;
        if (is_full_page) {
            item->setCursor(Qt::PointingHandCursor);
        }
    }
}

void Recommendations::onUserFromListClicked(int id) {
    emit userFromListClicked(id);
}

QStringList Recommendations::searchUsers(const QString &input) {
    QStringList users;

    auto response =
        manager->new_client.search_user_by_credentials(input.toStdString());

    for (int i = 0; i < response->users_size(); i++) {
        if (response->users(i).user_id() == manager->ID) {
            continue;
        }
        users.append(QString::fromStdString(response->users(i).login()));
        users_id[QString::fromStdString(response->users(i).login())] =
            response->users(i).user_id();
    }
    return users;
}

void Recommendations::updateUserList(const QString &input) {
    model->setStringList(searchUsers(input));
}

void Recommendations::onUserClicked(const QString &username) {
    int id = users_id[username];
    emit userClicked(id);
}