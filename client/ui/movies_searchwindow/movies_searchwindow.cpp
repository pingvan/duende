#include "movies_searchwindow.h"
#include "ui_movies_searchwindow.h"


Movies_SearchWindow::Movies_SearchWindow(Manager* managerPtr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Movies_SearchWindow),
    manager(managerPtr)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search movies...");

    model = new QStringListModel(searchMovies(""), search_line);

    completer = new QCompleter(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    search_line->setCompleter(completer);

    QAbstractItemView *popup = completer->popup();
    popup->setStyleSheet("background-color: lightgray;");

    layout->addWidget(search_line);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QPushButton *saveButton = new QPushButton("Save");
    layout->addWidget(saveButton);
    
    this->setStyleSheet("background-color: lightgray;");

    connect(search_line, &QLineEdit::textChanged, this, &Movies_SearchWindow::updateMoviesList);
    connect(saveButton, &QPushButton::clicked, this, &Movies_SearchWindow::saveChanges);
}

Movies_SearchWindow::~Movies_SearchWindow()
{
    delete ui;
}

void Movies_SearchWindow::saveChanges()
{
    QString selectedMovie = search_line->text();
    if (!selectedMovie.isEmpty()) {
        bool ok;
        int rating = QInputDialog::getInt(this, tr("Rate the movie"),
                                          tr("Rating:"), 5, 0, 10, 1, &ok);
        if (ok) {
            manager->new_client.add_movie(manager->ID, movies_map[selectedMovie], rating);
        }
    }
    this->close();
}

QStringList Movies_SearchWindow::searchMovies(const QString &input)
{
    QStringList films;

    movies_map.clear();

    auto response = manager->new_client.search_film_by_title(input.toStdString());

    qDebug() << "response->films_size() == " << response->films_size();

    for (int i = 0; i < response->films_size(); i++) {
        films.append(QString::fromStdString(response->films(i).title()));
        movies_map.insert(QString::fromStdString(response->films(i).title()), response->films(i).film_id());
    }

    return films;
}

void Movies_SearchWindow::updateMoviesList(const QString &input)
{
    model->setStringList(searchMovies(input));
}