#include "actors_searchwindow.h"
#include "ui_actors_searchwindow.h"


Actors_SearchWindow::Actors_SearchWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Actors_SearchWindow)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QStringList actorsList;
    actorsList << "Actor 1" << "Actor 2" << "Actor 3";

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search actors...");

    model = new QStringListModel(actorsList, search_line);

    completer = new QCompleter(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    search_line->setCompleter(completer);

    QAbstractItemView *popup = completer->popup();
    popup->setStyleSheet("background-color: lightgray;");

    layout->addWidget(search_line);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    this->setStyleSheet("background-color: lightgray;");

    connect(search_line, &QLineEdit::editingFinished, this, &Actors_SearchWindow::onEditingFinished);
}

Actors_SearchWindow::~Actors_SearchWindow()
{
    delete ui;
}

void Actors_SearchWindow::onEditingFinished()
{
    QString selectedActor = search_line->text();
    qDebug() << "Selected actor: " << selectedActor;

    this->close();
}
