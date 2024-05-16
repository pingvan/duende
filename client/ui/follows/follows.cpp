#include "follows.h"
#include "ui_follows.h"

Follows::Follows(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::Follows), manager(managerPtr) {
    std::vector<uint32_t> subscribers =
        manager->new_client.get_subscribers(manager->ID);
    std::vector<uint32_t> subscriptions =
        manager->new_client.get_subscriptions(manager->ID);
    std::vector<uint32_t> blocked =
        manager->new_client.get_blocked(manager->ID);

    // connect(
    //     manager, &Manager::subscribersChanged, this, &Follows::UpdateSubscribers
    // );
    // connect(
    //     manager, &Manager::subscriptionsChanged, this, &Follows::UpdateSubscriptions
    // );
    // connect(
    //     manager, &Manager::blockedChanged, this, &Follows::UpdateBlocked
    // );

    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search users...");

    model = new QStringListModel(searchUsers(""), search_line);

    completer = new QCompleter(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    search_line->setCompleter(completer);

    subscribersList = new QListWidget;
    subscribersList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    subscriptionsList = new QListWidget;
    subscriptionsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    blockedList = new QListWidget;
    blockedList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *listWidget = new QWidget;
    listWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    QVBoxLayout *listLayout = new QVBoxLayout(listWidget);
    listLayout->addWidget(subscribersList);
    listLayout->addWidget(subscriptionsList);
    listLayout->addWidget(blockedList);
    scrollArea->setWidget(listWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    QPushButton *subscribersButton = new QPushButton("Subscribers");
    QPushButton *subscriptionsButton = new QPushButton("Subscriptions");
    QPushButton *blockedButton = new QPushButton("Blocked");

    blockedButton->setFixedWidth(this->width() * 0.1);

    QButtonGroup *buttonGroup = new QButtonGroup(this);

    buttonGroup->addButton(subscriptionsButton);
    buttonGroup->addButton(subscribersButton);
    buttonGroup->addButton(blockedButton);

    subscribersButton->setCheckable(true);
    subscriptionsButton->setCheckable(true);
    blockedButton->setCheckable(true);

    subscriptionsButton->setChecked(true);

    buttonsLayout->addWidget(subscriptionsButton);
    buttonsLayout->addWidget(subscribersButton);
    buttonsLayout->addWidget(blockedButton);

    layout->addLayout(buttonsLayout);
    layout->addWidget(search_line);
    layout->addWidget(scrollArea);

    subscribersList->hide();
    blockedList->hide();
    connect(subscriptionsButton, &QPushButton::clicked, [this]() {
        UpdateSubscriptions();
        subscriptionsList->show();
        subscribersList->hide();
        blockedList->hide();
    });

    connect(subscribersButton, &QPushButton::clicked, [this]() {
        UpdateSubscribers();
        subscribersList->show();
        subscriptionsList->hide();
        blockedList->hide();
    });

    connect(blockedButton, &QPushButton::clicked, [this]() {
        UpdateBlocked();
        blockedList->show();
        subscribersList->hide();
        subscriptionsList->hide();
    });

    connect(search_line, &QLineEdit::textChanged, this, &Follows::updateUserList);

    connect(
        completer, QOverload<const QString &>::of(&QCompleter::activated), this,
        &Follows::onUserClicked
    );

    layout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding)
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


void Follows::UpdateSubscribers() {
    subscribersList->clear();
    std::vector<uint32_t> subscribers =
        manager->new_client.get_subscribers(manager->ID);
    for (const auto &subscriber : subscribers) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::number(subscriber));
        subscribersList->addItem(item);
    }
}

void Follows::UpdateSubscriptions() {
    subscriptionsList->clear();
    std::vector<uint32_t> subscriptions =
        manager->new_client.get_subscriptions(manager->ID);
    for (const auto &subscription : subscriptions) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::number(subscription));
        subscriptionsList->addItem(item);
    }
}

void Follows::UpdateBlocked() {
    blockedList->clear();
    std::vector<uint32_t> blocked =
        manager->new_client.get_blocked(manager->ID);
    for (const auto &block : blocked) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::number(block));
        blockedList->addItem(item);
    }
}
