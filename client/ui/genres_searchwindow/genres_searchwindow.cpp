#include "genres_searchwindow.h"
#include "ui_genres_searchwindow.h"

Genres_SearchWindow::Genres_SearchWindow(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::Genres_SearchWindow), manager(managerPtr) {
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search genres...");

    model = new QStringListModel(searchGenres(""), search_line);

    completer = new QCompleter(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    search_line->setCompleter(completer);

    QAbstractItemView *popup = completer->popup();
    popup->setStyleSheet("background-color: lightgray;");

    layout->addWidget(search_line);

    layout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding)
    );

    QPushButton *saveButton = new QPushButton("Save");
    layout->addWidget(saveButton);

    this->setStyleSheet("background-color: lightgray;");

    connect(
        search_line, &QLineEdit::textChanged, this,
        &Genres_SearchWindow::updateGenresList
    );
    connect(
        saveButton, &QPushButton::clicked, this,
        &Genres_SearchWindow::saveChanges
    );
}

Genres_SearchWindow::~Genres_SearchWindow() {
    delete ui;
}

void Genres_SearchWindow::saveChanges() {
    QString selectedGenre = search_line->text();
    common::UserFullPage *user =
        manager->new_client.get_user_full_page(manager->ID);
    if (!selectedGenre.isEmpty() && user != nullptr) {
        bool isGenreAlreadyFavorite = false;
        for (const auto &genre : user->favorite_genres()) {
            if (selectedGenre.toUpper() ==
                QString(common::Genre_Name(genre).c_str())) {
                isGenreAlreadyFavorite = true;
                break;
            }
        }
        if (!isGenreAlreadyFavorite) {
            manager->new_client.add_genre(
                manager->ID, selectedGenre.toStdString()
            );
            emit genreAdded();
        }
    }
    emit windowClosed();
    this->close();
}

QStringList Genres_SearchWindow::searchGenres(const QString &input) {
    QStringList genres;

    auto descriptor = common::Genre_descriptor();
    if (descriptor) {
        for (int i = 0; i < descriptor->value_count(); ++i) {
            std::string genre = descriptor->value(i)->name();
            std::transform(
                genre.begin(), genre.end(), genre.begin(), ::tolower
            );
            genres << QString::fromStdString(genre);
        }
    }

    return genres;
}

void Genres_SearchWindow::updateGenresList(const QString &input) {
    model->setStringList(searchGenres(input));
}
