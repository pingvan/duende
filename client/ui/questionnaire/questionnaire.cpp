#include "questionnaire.h"
#include "ui_questionnaire.h"


Questionnaire::Questionnaire(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Questionnaire)
    , manager(managerPtr)
{
    ui->setupUi(this);
    qDebug() << "Questionnaire page start\n";
    connect(this, &Questionnaire::questionnaireFilled, this, &Questionnaire::onQuestionnaireFilled);

    QGridLayout *gridLayout = new QGridLayout;

    QFormLayout *formLayout = new QFormLayout;

    lineEditActors = new QLineEdit;
    lineEditMovies = new QLineEdit;
    lineEditGenres = new QLineEdit;

    lineEditActors->setPlaceholderText("Search actors...");
    lineEditMovies->setPlaceholderText("Search movies...");
    lineEditGenres->setPlaceholderText("Search genres...");

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

    formLayout->addRow(tr("Favorite Actors:"), lineEditActors);
    formLayout->addRow(tr("Favorite Movies:"), lineEditMovies);
    formLayout->addRow(tr("Favorite Genres:"), lineEditGenres);

    QPushButton *saveButton = new QPushButton(tr("Save"));
    formLayout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, &Questionnaire::on_pushButton_save_questionnaire_clicked);

    gridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 0, 1, 1);
    gridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 1, 1, 1);
    gridLayout->addLayout(formLayout, 1, 1);
    gridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 2, 1, 1);
    gridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 1, 1, 1);

    this->setLayout(gridLayout);

    connect(lineEditActors, &QLineEdit::textChanged, this, &Questionnaire::updateActorsList);
    connect(lineEditMovies, &QLineEdit::textChanged, this, &Questionnaire::updateMoviesList);
    connect(lineEditGenres, &QLineEdit::textChanged, this, &Questionnaire::updateGenresList);

    connect(actorsCompleter, QOverload<const QString &>::of(&QCompleter::activated), this, &Questionnaire::onActorSelected);
    connect(moviesCompleter, QOverload<const QString &>::of(&QCompleter::activated), this, &Questionnaire::onMovieSelected);
    connect(genresCompleter, QOverload<const QString &>::of(&QCompleter::activated), this, &Questionnaire::onGenreSelected);

    qDebug() << "Questionnaire page end\n";
}


Questionnaire::~Questionnaire()
{
    delete ui;
}

void Questionnaire::on_pushButton_save_questionnaire_clicked()
{
    QString favorite_actors = lineEditActors->text();
    QString favorite_films = lineEditMovies->text();
    QString favorite_genres = lineEditGenres->text();

    // TODO: actor and film id by name and rating
    manager->new_client.add_actor(manager->ID, 0);
    manager->new_client.add_movie(manager->ID, 0, 0);
    manager->new_client.add_genre(manager->ID, favorite_genres.toStdString());

    emit questionnaireFilled();
}

void Questionnaire::onQuestionnaireFilled() {
}

QStringList Questionnaire::searchActors(const QString &input)
{
    QStringList actors;

    auto response = manager->new_client.search_actor_by_name(input.toStdString());

    for (int i = 0; i < response->actors_size(); i++) {
        actors.append(QString::fromStdString(response->actors(i).full_name()));
    }
    return actors;
}

QStringList Questionnaire::searchMovies(const QString &input)
{
    QStringList films;

    auto response = manager->new_client.search_film_by_title(input.toStdString());

    for (int i = 0; i < response->films_size(); i++) {
        films.append(QString::fromStdString(response->films(i).title()));
    }
    return films;
}

QStringList Questionnaire::searchGenres(const QString &input)
{
    QStringList genres;
    genres << "drama" << "comedy" << "thriller";

    return genres;
}

void Questionnaire::updateActorsList(const QString &input)
{
    actors_model->setStringList(searchActors(input));
}

void Questionnaire::updateMoviesList(const QString &input)
{
    movies_model->setStringList(searchMovies(input));
}

void Questionnaire::updateGenresList(const QString &input)
{
    genres_model->setStringList(searchGenres(input));
}

void Questionnaire::onActorSelected(const QString &actor)
{
    qDebug() << "Actor selected: " << actor;
    // Добавьте здесь код для обработки выбора актера
}

void Questionnaire::onMovieSelected(const QString &movie)
{
    qDebug() << "Movie selected: " << movie;
    // Добавьте здесь код для обработки выбора фильма
}

void Questionnaire::onGenreSelected(const QString &genre)
{
    qDebug() << "Genre selected: " << genre;
    // Добавьте здесь код для обработки выбора жанра
}
