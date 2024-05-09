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

    QHBoxLayout *topLayout = new QHBoxLayout;

    subscribersList = new QListWidget;
    subscribersList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    subscriptionsList = new QListWidget;
    subscriptionsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true); 
    scrollArea->setFrameShape(QFrame::NoFrame); 

    QWidget *listWidget = new QWidget;
    QVBoxLayout *listLayout = new QVBoxLayout(listWidget);
    listLayout->addWidget(subscribersList);
    listLayout->addWidget(subscriptionsList);
    scrollArea->setWidget(listWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    QPushButton *subscribersButton = new QPushButton("Subscribers");
    QPushButton *subscriptionsButton = new QPushButton("Subscriptions");

    QButtonGroup *buttonGroup = new QButtonGroup(this);

    buttonGroup->addButton(subscriptionsButton);
    buttonGroup->addButton(subscribersButton);

    subscribersButton->setCheckable(true);
    subscriptionsButton->setCheckable(true);

    subscriptionsButton->setChecked(true);

    buttonsLayout->addWidget(subscriptionsButton);
    buttonsLayout->addWidget(subscribersButton);

    layout->addWidget(search_line);
    layout->addLayout(buttonsLayout);
    layout->addWidget(scrollArea);
    layout->addStretch(1);
    // layout->addStretch();

    subscribersList->hide();
    connect(subscriptionsButton, &QPushButton::clicked, [this]() {
        subscriptionsList->show();
        subscribersList->hide();
    });

    connect(subscribersButton, &QPushButton::clicked, [this]() {
        subscribersList->show();
        subscriptionsList->hide();
    });

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