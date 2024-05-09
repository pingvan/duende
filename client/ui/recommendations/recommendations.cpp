#include "recommendations.h"
#include "ui_recommendations.h"

Recommendations::Recommendations(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Recommendations)
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

    connect(search_line, &QLineEdit::textChanged, this, &Recommendations::updateUserList);
    connect(completer, QOverload<const QString &>::of(&QCompleter::activated), this, &Recommendations::onUserClicked);

}

Recommendations::~Recommendations()
{
    delete ui;
}

QStringList Recommendations::searchUsers(const QString &input)
{
    QStringList users;

    auto response = manager->new_client.search_user_by_credentials(input.toStdString());

    for (int i = 0; i < response->users_size(); i++) {
        if (response->users(i).user_id() == manager->ID) {
            continue;
        }
        users.append(QString::fromStdString(response->users(i).login()));
        users_id[QString::fromStdString(response->users(i).login())] = response->users(i).user_id();
    }
    return users;
}

void Recommendations::updateUserList(const QString &input)
{
    model->setStringList(searchUsers(input));
}

void Recommendations::onUserClicked(const QString &username)
{
    int id = users_id[username];
    emit userClicked(id);
}