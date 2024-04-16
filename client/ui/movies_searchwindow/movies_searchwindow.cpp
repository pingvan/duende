#include "movies_searchwindow.h"
#include "ui_movies_searchwindow.h"


Movies_SearchWindow::Movies_SearchWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Movies_SearchWindow)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QStringList moviesList;
    moviesList << "Movie 1" << "Movie 2" << "Movie 3";

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search movies...");

    model = new QStringListModel(moviesList, search_line);

    completer = new QCompleter(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    search_line->setCompleter(completer);

    QAbstractItemView *popup = completer->popup();
    popup->setStyleSheet("background-color: lightgray;");

    layout->addWidget(search_line);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    this->setStyleSheet("background-color: lightgray;");

    connect(search_line, &QLineEdit::editingFinished, this, &Movies_SearchWindow::onEditingFinished);
}

Movies_SearchWindow::~Movies_SearchWindow()
{
    delete ui;
}

void Movies_SearchWindow::onEditingFinished()
{
    QString selectedMovie = search_line->text();
    qDebug() << "Selected movie: " << selectedMovie;

    this->close();
}
