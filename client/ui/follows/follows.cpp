#include "follows.h"
#include "ui_follows.h"

Follows::Follows(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::Follows), manager(managerPtr) {
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    search_line = new QLineEdit;
    search_line->setPlaceholderText("Search...");

    currentList = Subscriptions;

    // model = new QStringListModel(searchUsers(""), search_line);

    // completer = new QCompleter(model);
    // completer->setCaseSensitivity(Qt::CaseInsensitive);
    // search_line->setCompleter(completer);
    search_line->setMinimumHeight(50);
    search_line->setStyleSheet(
        "font-size: 16px; color: #5a1e71; border: 1px solid #8023a8; "
        "border-radius: 5px; padding: 5px; text-color: #8023a8;"
    );

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
    listWidget->setStyleSheet("border: none;");
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
    UpdateSubscriptions();

    buttonsLayout->addWidget(subscriptionsButton);
    buttonsLayout->addWidget(subscribersButton);
    buttonsLayout->addWidget(blockedButton);

    layout->addLayout(buttonsLayout);
    layout->addWidget(search_line);
    layout->addWidget(scrollArea);

    // subscribersList->setStyleSheet(
    //     "QListWidget::item { border: 1px solid #8023a8; background-color:
    //     #f4e9fe;}"
    // );
    // subscriptionsList->setStyleSheet(
    //     "QListWidget::item { border: 1px solid #8023a8; border-radius: 5px;"
    //     "margin: 2px;}"
    // );
    // blockedList->setStyleSheet(
    //     "QListWidget::item { border: 1px solid #8023a8; border-radius: 5px;"
    //     "margin: 2px;}"
    // );

    subscribersList->hide();
    blockedList->hide();
    connect(subscriptionsButton, &QPushButton::clicked, [this]() {
        UpdateSubscriptions();
        subscriptionsList->show();
        subscribersList->hide();
        blockedList->hide();
        currentList = Subscriptions;
        // updateUserList(search_line->text());
    });

    connect(subscribersButton, &QPushButton::clicked, [this]() {
        UpdateSubscribers();
        subscribersList->show();
        subscriptionsList->hide();
        blockedList->hide();
        currentList = Subscribers;
        updateUserList(search_line->text());
    });

    connect(blockedButton, &QPushButton::clicked, [this]() {
        UpdateBlocked();
        blockedList->show();
        subscribersList->hide();
        subscriptionsList->hide();
        currentList = Blocked;
        updateUserList(search_line->text());
    });

    connect(
        search_line, &QLineEdit::textChanged, this, &Follows::updateUserList
    );

    // connect(
    //     completer, QOverload<const QString &>::of(&QCompleter::activated),
    //     this, &Follows::onUserClicked
    // );

    connect(
        subscribersList, &QListWidget::itemClicked, this,
        &Follows::onUserFromListClicked
    );
    connect(
        subscriptionsList, &QListWidget::itemClicked, this,
        &Follows::onUserFromListClicked
    );
    connect(
        blockedList, &QListWidget::itemClicked, this,
        &Follows::onUserFromListClicked
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

    std::vector<uint32_t> ids;
    switch (currentList) {
        case Subscriptions:
            ids = manager->new_client.get_subscriptions(manager->ID);
            break;
        case Subscribers:
            ids = manager->new_client.get_subscribers(manager->ID);
            break;
        case Blocked:
            ids = manager->new_client.get_blocked(manager->ID);
            break;
    }

    for (const auto &id : ids) {
        common::UserFullPage *user = manager->new_client.get_user_full_page(id);
        if (user) {
            QString username = QString::fromStdString(user->preview().login());
            if (username.startsWith(input, Qt::CaseInsensitive)) {
                users.append(username);
                users_id[username] = id;
            }
            delete user;
        }
    }

    return users;
}

// void Follows::updateUserList(const QString &input) {
//     model->setStringList(searchUsers(input));
// }

void Follows::updateUserList(const QString &input) {
    QStringList users = searchUsers(input);

    switch (currentList) {
        case Subscriptions:
            subscriptionsList->clear();
            break;
        case Subscribers:
            subscribersList->clear();
            break;
        case Blocked:
            blockedList->clear();
            break;
    }
    for (const QString &user : users) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(user);
        item->setData(Qt::UserRole, users_id[user]);
        item->setSizeHint(QSize(item->sizeHint().width(), 45));
        QFont font = item->font();
        font.setPointSize(18);
        item->setFont(font);
        item->setForeground(QColor("#8023a8"));
        switch (currentList) {
            case Subscriptions:
                subscriptionsList->addItem(item);
                break;
            case Subscribers:
                subscribersList->addItem(item);
                break;
            case Blocked:
                blockedList->addItem(item);
                break;
        }
    }
}

void Follows::UpdateList(QListWidget &list, const std::vector<uint32_t> &ids) {
    list.clear();
    for (const auto &id : ids) {
        common::UserFullPage *user = manager->new_client.get_user_full_page(id);
        if (user) {
            QString username = QString::fromStdString(user->preview().login());
            QString quote = QString::fromStdString(user->quote());
            QListWidgetItem *item = new QListWidgetItem;
            if (quote.isEmpty()) {
                item->setText(username);
            } else {
                item->setText(username + ": \" " + quote + "\"");
            }
            item->setData(Qt::UserRole, id);
            item->setSizeHint(QSize(item->sizeHint().width(), 45));
            QFont font = item->font();
            font.setPointSize(18);
            item->setFont(font);
            item->setForeground(QColor("#8023a8"));
            list.addItem(item);
            delete user;
        }
    }
}

void Follows::UpdateSubscribers() {
    std::vector<uint32_t> subscribers =
        manager->new_client.get_subscribers(manager->ID);
    UpdateList(*subscribersList, subscribers);
}

void Follows::UpdateSubscriptions() {
    std::vector<uint32_t> subscriptions =
        manager->new_client.get_subscriptions(manager->ID);
    UpdateList(*subscriptionsList, subscriptions);
}

void Follows::UpdateBlocked() {
    std::vector<uint32_t> blocked =
        manager->new_client.get_blocked(manager->ID);
    UpdateList(*blockedList, blocked);
}

void Follows::onUserClicked(const QString &username) {
    int id = users_id[username];
    emit userClicked(id);
}

void Follows::onUserFromListClicked(QListWidgetItem *item) {
    int id = item->data(Qt::UserRole).toInt();
    emit userFromListClicked(id);
}
