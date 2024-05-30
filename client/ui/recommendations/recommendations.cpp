#include "recommendations.h"
#include "ui_recommendations.h"

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

    usersGallery = new QListWidget;
    usersGallery->setWordWrap(true);
    usersGallery->setFixedHeight(300);
    usersGallery->setIconSize(QSize(200, 200));
    usersGallery->setSpacing(10);
    usersGallery->setFrameShape(QFrame::NoFrame);
    usersGallery->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    usersGallery->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    usersGallery->setFlow(QListWidget::LeftToRight);
    usersGallery->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    // usersGallery->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    // QScroller::grabGesture(usersGallery, QScroller::LeftMouseButtonGesture);
    // QScroller *scroller = QScroller::scroller(usersGallery);
    // QScrollerProperties properties = scroller->scrollerProperties();
    // properties.setScrollMetric(
    //     QScrollerProperties::MousePressEventDelay, QVariant(0.5)
    // );
    // scroller->setScrollerProperties(properties);
    filmsGallery = new QListWidget;
    filmsGallery->setFixedHeight(300);
    filmsGallery->setIconSize(QSize(200, 200));
    filmsGallery->setSpacing(10);
    filmsGallery->setFrameShape(QFrame::NoFrame);
    filmsGallery->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    filmsGallery->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    filmsGallery->setFlow(QListWidget::LeftToRight);
    filmsGallery->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    updateRecommendations();

    layout->addSpacing(20);
    layout->addWidget(users_text);
    layout->addWidget(usersGallery);
    layout->addWidget(movies_text);
    layout->addWidget(filmsGallery);

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

    connect(
        usersGallery, &QListWidget::itemClicked, this,
        &Recommendations::onUserFromListClicked
    );
}

Recommendations::~Recommendations() {
    delete ui;
}

void Recommendations::updateRecommendations() {
    // Получаем рекомендации
    auto recommended_users =
        manager->new_client.get_user_recommendations(manager->ID);
    auto recommended_films =
        manager->new_client.get_film_recommendations(manager->ID);

    usersGallery->clear();
    filmsGallery->clear();

    for (const auto &user : recommended_users) {
        QIcon userIcon;
        if (user.photo().empty()) {
            userIcon = QIcon(":/img/person-circle2.png");
        } else {
            userIcon = QIcon(QPixmap::fromImage(
                QImage::fromData(QByteArray::fromStdString(user.photo()))
            ));
        }
        auto *userItem = new QListWidgetItem(
            userIcon, QString::fromStdString(user.login().c_str())
        );
        userItem->setData(Qt::UserRole, user.user_id());
        userItem->setTextAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        QFont font = userItem->font();
        font.setPointSize(18);
        userItem->setFont(font);
        usersGallery->addItem(userItem);
    }

    for (const auto &film : recommended_films) {
        QIcon filmIcon;
        filmIcon = QIcon(":/img/titanik.jpeg");
        auto *filmItem = new QListWidgetItem(
            filmIcon, QString::fromStdString(film.title().c_str())
        );
        filmItem->setTextAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        QFont font = filmItem->font();
        font.setPointSize(18);
        filmItem->setFont(font);
        filmsGallery->addItem(filmItem);
        filmItem->setToolTip(tr(""));
    }
}

void Recommendations::onUserFromListClicked(QListWidgetItem *item) {
    int id = item->data(Qt::UserRole).toInt();
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