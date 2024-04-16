#include "questionnaire.h"
#include "ui_questionnaire.h"

#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QPushButton>
#include <QCompleter>

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

    QStringList actorsList;
    actorsList << "Actor 1" << "Actor 2" << "Actor 3";

    QStringList moviesList;
    moviesList << "Movie 1" << "Movie 2" << "Movie 3";

    QStringList genresList;
    genresList << "Genre 1" << "Genre 2" << "Genre 3";

    lineEditActors->setPlaceholderText("Search actors...");
    lineEditMovies->setPlaceholderText("Search movies...");
    lineEditGenres->setPlaceholderText("Search genres...");

    QStringListModel *actors_model = new QStringListModel(actorsList, lineEditActors);
    QStringListModel *movies_model = new QStringListModel(moviesList, lineEditMovies);
    QStringListModel *genres_model = new QStringListModel(genresList, lineEditGenres);

    QCompleter *actorsCompleter = new QCompleter(actorsList, this);
    QCompleter *moviesCompleter = new QCompleter(moviesList, this);
    QCompleter *genresCompleter = new QCompleter(genresList, this);

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

    qDebug() << "Questionnaire page end\n";
}


Questionnaire::~Questionnaire()
{
    delete ui;
}

void Questionnaire::on_pushButton_save_questionnaire_clicked()
{
    QString favourite_actors = lineEditActors->text();
    QString favourite_films = lineEditMovies->text();
    QString favourite_genres = lineEditGenres->text();

    // TODO: actor and film id by name and rating
    manager->new_client.add_actor(manager->ID, 0);
    manager->new_client.add_movie(manager->ID, 0, 0);
    manager->new_client.add_genre(manager->ID, favourite_genres.toStdString());

    emit questionnaireFilled();
}

void Questionnaire::onQuestionnaireFilled() {
}


