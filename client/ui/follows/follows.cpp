#include "follows.h"
#include "ui_follows.h"

Follows::Follows(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::Follows), manager(managerPtr) {
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

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
}

Follows::~Follows() {
    delete ui;
}
