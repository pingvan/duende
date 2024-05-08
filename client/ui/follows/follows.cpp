#include "follows.h"
#include "ui_follows.h"

Follows::Follows(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::Follows), manager(managerPtr) {
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search users...");

    model = new QStringListModel(searchUsers(""), search_line);

    completer = new QCompleter(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    search_line->setCompleter(completer);

    // layout->addWidget(search_line);

    QHBoxLayout *topLayout = new QHBoxLayout;

    subscribersList = new QListWidget;
    subscribersList = new QListWidget();
    subscribersList->setFixedWidth(500);
    subscribersList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    subscriptionsList = new QListWidget;
    subscriptionsList->setFixedWidth(500);
    subscriptionsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    subscriptionsList->hide();

    QScrollArea *scrollArea = new QScrollArea;
    QWidget *listWidget = new QWidget;
    QVBoxLayout *listLayout = new QVBoxLayout(listWidget);
    listLayout->addWidget(subscribersList);
    listLayout->addWidget(subscriptionsList);
    scrollArea->setWidget(listWidget);

    toggleButton = new QPushButton("Show Subscribers");
    toggleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    topLayout->addWidget(toggleButton);
    topLayout->addStretch();
    search_line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    topLayout->addWidget(search_line);

    topLayout->setStretchFactor(toggleButton, 4);
    topLayout->setStretchFactor(search_line, 6);

    layout->addLayout(topLayout);
    layout->addWidget(scrollArea);
    layout->addStretch();

    subscribersList->hide();
    connect(toggleButton, &QPushButton::clicked, this, &Follows::toggleLists);

    layout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding)
    );

    connect(
        search_line, &QLineEdit::textChanged, this, &Follows::updateUserList
    );
    connect(
        completer, QOverload<const QString &>::of(&QCompleter::activated), this,
        &Follows::onUserClicked
    );
}

void Follows::toggleLists() {
    if (subscribersList->isVisible()) {
        subscribersList->hide();
        subscriptionsList->show();
        toggleButton->setText("Show Subscribers");
    } else {
        subscriptionsList->hide();
        subscribersList->show();
        toggleButton->setText("Show Subscriptions");
    }
}

Follows::~Follows() {
    delete ui;
}

QStringList Follows::searchUsers(const QString &input) {
    QStringList users;

    auto response =
        manager->new_client.search_user_by_credentials(input.toStdString());

    for (int i = 0; i < response->users_size(); i++) {
        if (response->users(i).user_id() == manager->ID) {
            continue;
        }
        users.append(QString::fromStdString(response->users(i).login()));
        users_id[QString::fromStdString(response->users(i).login())] =
            response->users(i).user_id();
    }
    return users;
}

void Follows::updateUserList(const QString &input) {
    model->setStringList(searchUsers(input));
}

void Follows::onUserClicked(const QString &username) {
    int id = users_id[username];
    emit userClicked(id);
}