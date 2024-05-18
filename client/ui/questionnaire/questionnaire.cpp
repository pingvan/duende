#include "questionnaire.h"
#include "ui_questionnaire.h"

Questionnaire::Questionnaire(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::Questionnaire), manager(managerPtr) {
    ui->setupUi(this);
    qDebug() << "Questionnaire page start\n";
    this->setStyleSheet(
        "QLineEdit { font-size: 16px; color: #8023a8; min-height: 40px; }"
        "QPushButton { min-width: 100px; min-height: 40px; font-size: 20px; "
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
        "#b24ee7, stop: 1 #982ecb); "
        "color: white; border-radius: 5px;}"
        "QPushButton:pressed, QPushButton:checked { background: "
        "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #8023A8, stop: 1 "
        "#691e8a); }"
    );
    connect(
        this, &Questionnaire::questionnaireFilled, this,
        &Questionnaire::onQuestionnaireFilled
    );

    QGridLayout *gridLayout = new QGridLayout;
    QFormLayout *formLayout = new QFormLayout;
    QVBoxLayout *actorsLayout = new QVBoxLayout;
    QVBoxLayout *moviesLayout = new QVBoxLayout;
    QVBoxLayout *genresLayout = new QVBoxLayout;
    QHBoxLayout *actorsInputLayout = new QHBoxLayout;
    QHBoxLayout *moviesInputLayout = new QHBoxLayout;
    QHBoxLayout *genresInputLayout = new QHBoxLayout;
    QVBoxLayout *actorsSelectedLayout = new QVBoxLayout;
    QVBoxLayout *moviesSelectedLayout = new QVBoxLayout;
    QVBoxLayout *genresSelectedLayout = new QVBoxLayout;

    lineEditActors = new QLineEdit;
    lineEditMovies = new QLineEdit;
    lineEditGenres = new QLineEdit;

    lineEditActors->setPlaceholderText("Search actors...");
    lineEditMovies->setPlaceholderText("Search movies...");
    lineEditGenres->setPlaceholderText("Search genres...");

    QLabel *selectedActorsPrefixLabel = new QLabel("You selected:");
    QLabel *selectedMoviesPrefixLabel = new QLabel("You selected:");
    QLabel *selectedGenresPrefixLabel = new QLabel("You selected:");

    selectedActorsLabel = new QLabel;
    selectedMoviesLabel = new QLabel;
    selectedGenresLabel = new QLabel;

    selectedActorsLabel->setWordWrap(true);
    selectedMoviesLabel->setWordWrap(true);
    selectedGenresLabel->setWordWrap(true);

    actorsSelectedLayout->addWidget(selectedActorsPrefixLabel);
    actorsSelectedLayout->addWidget(selectedActorsLabel);
    moviesSelectedLayout->addWidget(selectedMoviesPrefixLabel);
    moviesSelectedLayout->addWidget(selectedMoviesLabel);
    genresSelectedLayout->addWidget(selectedGenresPrefixLabel);
    genresSelectedLayout->addWidget(selectedGenresLabel);

    QPushButton *addActorButton = new QPushButton(tr("Add"));
    QPushButton *addMovieButton = new QPushButton(tr("Add"));
    QPushButton *addGenreButton = new QPushButton(tr("Add"));

    actorsInputLayout->addWidget(lineEditActors);
    actorsInputLayout->addWidget(addActorButton);
    moviesInputLayout->addWidget(lineEditMovies);
    moviesInputLayout->addWidget(addMovieButton);
    genresInputLayout->addWidget(lineEditGenres);
    genresInputLayout->addWidget(addGenreButton);

    actorsLayout->addLayout(actorsInputLayout);
    actorsLayout->addLayout(actorsSelectedLayout);
    moviesLayout->addLayout(moviesInputLayout);
    moviesLayout->addLayout(moviesSelectedLayout);
    genresLayout->addLayout(genresInputLayout);
    genresLayout->addLayout(genresSelectedLayout);

    formLayout->addRow(tr("Favorite Actors:"), actorsLayout);
    formLayout->addRow(tr("Favorite Movies:"), moviesLayout);
    formLayout->addRow(tr("Favorite Genres:"), genresLayout);

    actors_model = new QStringListModel(searchActors(""), lineEditActors);
    movies_model = new QStringListModel(searchMovies(""), lineEditMovies);
    genres_model = new QStringListModel(searchGenres(""), lineEditGenres);

    QCompleter *actorsCompleter = new QCompleter(actors_model);
    QCompleter *moviesCompleter = new QCompleter(movies_model);
    QCompleter *genresCompleter = new QCompleter(genres_model);

    actorsCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    moviesCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    genresCompleter->setCaseSensitivity(Qt::CaseInsensitive);

    lineEditActors->setCompleter(actorsCompleter);
    lineEditMovies->setCompleter(moviesCompleter);
    lineEditGenres->setCompleter(genresCompleter);

    QPushButton *saveButton = new QPushButton(tr("Save"));
    formLayout->addWidget(saveButton);

    connect(
        saveButton, &QPushButton::clicked, this,
        &Questionnaire::on_pushButton_save_questionnaire_clicked
    );

    gridLayout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding),
        0, 0, 1, 1
    );
    gridLayout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding),
        0, 1, 1, 1
    );
    gridLayout->addLayout(formLayout, 1, 1);
    gridLayout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding),
        0, 2, 1, 1
    );
    gridLayout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding),
        2, 1, 1, 1
    );

    this->setLayout(gridLayout);

    connect(
        lineEditActors, &QLineEdit::textChanged, this,
        &Questionnaire::updateActorsList
    );
    connect(
        lineEditMovies, &QLineEdit::textChanged, this,
        &Questionnaire::updateMoviesList
    );
    connect(
        lineEditGenres, &QLineEdit::textChanged, this,
        &Questionnaire::updateGenresList
    );

    connect(
        addActorButton, &QPushButton::clicked, this,
        &Questionnaire::onAddActorClicked
    );
    connect(
        addMovieButton, &QPushButton::clicked, this,
        &Questionnaire::onAddMovieClicked
    );
    connect(
        addGenreButton, &QPushButton::clicked, this,
        &Questionnaire::onAddGenreClicked
    );

    qDebug() << "Questionnaire page end\n";
}

Questionnaire::~Questionnaire() {
    delete ui;
}

void Questionnaire::on_pushButton_save_questionnaire_clicked() {
    for (int actorId : selectedActorsIds) {
        manager->new_client.add_actor(manager->ID, actorId);
    }

    for (int i = 0; i < selectedMoviesIds.size(); i++) {
        manager->new_client.add_movie(
            manager->ID, selectedMoviesIds[i], selectedMoviesRatings[i]
        );
    }

    for (const QString &genre : selectedGenresLabel->text().split(", ")) {
        if (!genre.isEmpty()) {
            manager->new_client.add_genre(manager->ID, genre.toStdString());
        }
    }

    emit questionnaireFilled();
}

void Questionnaire::onQuestionnaireFilled() {
}

QStringList Questionnaire::searchActors(const QString &input) {
    QStringList actors;

    auto response =
        manager->new_client.search_actor_by_name(input.toStdString());

    for (int i = 0; i < response->actors_size(); i++) {
        actors.append(QString::fromStdString(response->actors(i).full_name()));
        actors_map.insert(
            QString::fromStdString(response->actors(i).full_name()),
            response->actors(i).actor_id()
        );
    }
    return actors;
}

QStringList Questionnaire::searchMovies(const QString &input) {
    QStringList films;

    auto response =
        manager->new_client.search_film_by_title(input.toStdString());

    for (int i = 0; i < response->films_size(); i++) {
        films.append(QString::fromStdString(response->films(i).title()));
        movies_map.insert(
            QString::fromStdString(response->films(i).title()),
            response->films(i).film_id()
        );
    }
    return films;
}

QStringList Questionnaire::searchGenres(const QString &input) {
    QStringList genres;
    genres << "action" << "adventure" << "comedy" << "drama" << "horror"
           << "sci_fi" << "fantasy" << "thriller" << "mystery" << "romance"
           << "animation" << "documentary" << "crime" << "western" << "music"
           << "history" << "war" << "biographical" << "sports" << "family"
           << "foreign" << "tv_movie";

    return genres;
}

void Questionnaire::updateActorsList(const QString &input) {
    actors_model->setStringList(searchActors(input));
}

void Questionnaire::updateMoviesList(const QString &input) {
    movies_model->setStringList(searchMovies(input));
}

void Questionnaire::updateGenresList(const QString &input) {
    genres_model->setStringList(searchGenres(input));
}

void Questionnaire::onAddActorClicked() {
    QString selectedActor = lineEditActors->text();
    if (!selectedActor.isEmpty() &&
        !selectedActorsLabel->text().contains(selectedActor)) {
        if (!selectedActorsLabel->text().isEmpty()) {
            selectedActorsLabel->setText(
                selectedActorsLabel->text() + ", " + selectedActor
            );
        } else {
            selectedActorsLabel->setText(selectedActor);
        }
        selectedActorsIds.append(actors_map.value(selectedActor));
        lineEditActors->clear();
    }
}

void Questionnaire::onAddMovieClicked() {
    QString selectedMovie = lineEditMovies->text();
    if (!selectedMovie.isEmpty() &&
        !selectedMoviesLabel->text().contains(selectedMovie)) {
        bool ok;
        int rating = QInputDialog::getInt(
            this, tr("Rate the movie"), tr("Rating:"), 5, 0, 10, 1, &ok
        );
        if (ok) {
            if (!selectedMoviesLabel->text().isEmpty()) {
                selectedMoviesLabel->setText(
                    selectedMoviesLabel->text() + ", " + selectedMovie + " (" +
                    QString::number(rating) + ")"
                );
            } else {
                selectedMoviesLabel->setText(
                    selectedMovie + " (" + QString::number(rating) + ")"
                );
            }
            selectedMoviesIds.append(movies_map.value(selectedMovie));
            selectedMoviesRatings.append(rating);
            lineEditMovies->clear();
        }
    }
}

void Questionnaire::onAddGenreClicked() {
    QString selectedGenre = lineEditGenres->text();
    if (!selectedGenre.isEmpty() &&
        !selectedGenresLabel->text().contains(selectedGenre)) {
        if (!selectedGenresLabel->text().isEmpty()) {
            selectedGenresLabel->setText(
                selectedGenresLabel->text() + ", " + selectedGenre
            );
        } else {
            selectedGenresLabel->setText(selectedGenre);
        }
        lineEditGenres->clear();
    }
}
