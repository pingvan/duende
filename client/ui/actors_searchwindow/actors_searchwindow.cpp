#include "actors_searchwindow.h"
#include "ui_actors_searchwindow.h"


Actors_SearchWindow::Actors_SearchWindow(Manager* managerPtr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Actors_SearchWindow),
    manager(managerPtr)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search actors...");

    model = new QStringListModel(searchActors(""), search_line);

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

    connect(search_line, &QLineEdit::textChanged, this, &Actors_SearchWindow::updateActorsList);
    connect(saveButton, &QPushButton::clicked, this, &Actors_SearchWindow::saveChanges);
}

Actors_SearchWindow::~Actors_SearchWindow()
{
    delete ui;
}

void Actors_SearchWindow::saveChanges()
{   
    QString selectedActor = search_line->text();
    if (!selectedActor.isEmpty()) {
        manager->new_client.add_actor(manager->ID, actors_map[selectedActor]);
    }
    
    this->close();
}

QStringList Actors_SearchWindow::searchActors(const QString &input)
{
    QStringList actors;

    actors_map.clear();

    auto response = manager->new_client.search_actor_by_name(input.toStdString());

    for (int i = 0; i < response->actors_size(); i++) {
        actors.append(QString::fromStdString(response->actors(i).full_name()));
        actors_map.insert(QString::fromStdString(response->actors(i).full_name()), response->actors(i).actor_id());
    }

    return actors;
}

void Actors_SearchWindow::updateActorsList(const QString &input)
{
    model->setStringList(searchActors(input));
}
