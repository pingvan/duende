#include "registrationpage.h"
#include "ui_registrationpage.h"

RegistrationPage::RegistrationPage(Manager *managerPtr, QWidget *parent)
    : QWidget(parent), ui(new Ui::RegistrationPage), manager(managerPtr) {
    ui->setupUi(this);

    build();

    connect_slots();
}

void RegistrationPage::loginButton_clicked() {
    QString login = login_username->text();
    QString password = login_password->text();

    authservice::LoginResponse *response =
        manager->new_client.login(login.toStdString(), password.toStdString());
    manager->ID = response->id();
    manager->username =
        manager->new_client.get_user_preview(manager->ID)->login();

    emit loginSuccessful();
}

void RegistrationPage::signupButton_clicked() {
    QString username = signup_username->text();
    QString email = signup_email->text();
    QString password = signup_password->text();
    QString password_confirmation = signup_confirm->text();

    authservice::SignupResponse *response = manager->new_client.registration(
        username.toStdString(), email.toStdString(), password.toStdString(),
        password_confirmation.toStdString()
    );
    manager->ID = response->id();
    manager->username = username.toStdString();

    emit registrationSuccessful();
}

void RegistrationPage::build() {
    this->setStyleSheet(
        "QLineEdit { font-size: 16px; color: #8023a8; min-height: 40px; }"
        "QPushButton { min-width: 100px; min-height: 40px; font-size: 20px; "
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
        "#b24ee7, stop: 1 #982ecb); "
        "color: white; border-radius: 5px;}"
        "QPushButton:pressed, QPushButton:checked { background: "
        "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #8023A8, stop: 1 "
        "#691e8a); }"
    );

    QTabWidget *tabWidget = new QTabWidget(this);

    login_username = new QLineEdit(this);
    login_password = new QLineEdit(this);
    signup_username = new QLineEdit(this);
    signup_email = new QLineEdit(this);
    signup_password = new QLineEdit(this);
    signup_confirm = new QLineEdit(this);

    login_password->setEchoMode(QLineEdit::Password);
    signup_password->setEchoMode(QLineEdit::Password);
    signup_confirm->setEchoMode(QLineEdit::Password);

    signup_username->setPlaceholderText("Enter username");
    signup_email->setPlaceholderText("Enter email");
    signup_password->setPlaceholderText("Enter password");
    signup_confirm->setPlaceholderText("Enter password confirmation");

    login_username->setPlaceholderText("Enter login");
    login_password->setPlaceholderText("Enter password");

    QWidget *tab1 = new QWidget;
    QWidget *tab2 = new QWidget;
    tabWidget->addTab(tab1, "Signup");
    tabWidget->addTab(tab2, "Login");

    signupButton = new QPushButton("Sign up", tab1);
    signupButton->setCursor(Qt::PointingHandCursor);

    QWidget *formWidget1 = new QWidget(tab1);
    QFormLayout *formLayout1 = new QFormLayout(formWidget1);
    formLayout1->addRow(signup_username);
    formLayout1->addRow(signup_email);
    formLayout1->addRow(signup_password);
    formLayout1->addRow(signup_confirm);

    loginButton = new QPushButton("Log in", tab2);
    loginButton->setCursor(Qt::PointingHandCursor);

    QWidget *formWidget2 = new QWidget(tab2);
    QFormLayout *formLayout2 = new QFormLayout(formWidget2);
    formLayout2->addRow(login_username);
    formLayout2->addRow(login_password);

    QVBoxLayout *vLayout1 = new QVBoxLayout(tab1);
    vLayout1->addStretch(1);
    vLayout1->addWidget(formWidget1);
    vLayout1->addStretch(1);
    vLayout1->addWidget(signupButton, 0, Qt::AlignCenter);

    QVBoxLayout *vLayout2 = new QVBoxLayout(tab2);
    vLayout2->addStretch(1);
    vLayout2->addWidget(formWidget2);
    vLayout2->addStretch(1);
    vLayout2->addWidget(loginButton, 0, Qt::AlignCenter);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding),
        0, 0
    );
    mainLayout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding),
        0, 2
    );
    mainLayout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding),
        2, 0
    );
    mainLayout->addItem(
        new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding),
        2, 2
    );
    mainLayout->addWidget(tabWidget, 1, 1);
}

void RegistrationPage::connect_slots() {
    connect(
        signupButton, &QPushButton::clicked, this,
        &RegistrationPage::signupButton_clicked
    );
    connect(
        loginButton, &QPushButton::clicked, this,
        &RegistrationPage::loginButton_clicked
    );
}

RegistrationPage::~RegistrationPage() {
    delete ui;
}
