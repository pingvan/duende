#include "follows.h"
#include "ui_follows.h"

Follows::Follows(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Follows)
    , manager(managerPtr)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search users...");

    model = new QStringListModel(searchUsers(""), search_line);

    completer = new QCompleter(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    search_line->setCompleter(completer);

    layout->addWidget(search_line);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(search_line, &QLineEdit::textChanged, this, &Follows::updateUserList);
}

Follows::~Follows()
{
    delete ui;
}

QStringList Follows::searchUsers(const QString &input)
{
    QStringList users;

    auto response = manager->new_client.search_user_by_credentials(input.toStdString());

    qDebug() << "response->users_size() == " << response->users_size();

    for (int i = 0; i < response->users_size(); i++) {
        users.append(QString::fromStdString(response->users(i).login()));
    }
    return users;
}

void Follows::updateUserList(const QString &input)
{
    model->setStringList(searchUsers(input));
}