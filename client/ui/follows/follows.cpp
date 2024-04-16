#include "follows.h"
#include "ui_follows.h"


Follows::Follows(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Follows)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QStringList users;
    users << "John" << "Alice" << "Bob" << "Carol" << "David" << "Eva";

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search users...");

    model = new QStringListModel(users, search_line);

    completer = new QCompleter(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    search_line->setCompleter(completer);

    layout->addWidget(search_line);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

Follows::~Follows()
{
    delete ui;
}