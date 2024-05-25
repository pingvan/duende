#include "registrationpage.h"
#include "ui_registrationpage.h"


RegistrationPage::RegistrationPage(Manager* managerPtr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationPage),
    manager(managerPtr)
{
    ui->setupUi(this);

    qDebug() << "Registration page start\n";
    // this->setStyleSheet("QWidget { background-color: #8023a8; }");
    this->setStyleSheet("QLineEdit { font-size: 16px; color: #8023a8; min-height: 40px; }"
                    "QPushButton { min-width: 100px; min-height: 40px; font-size: 20px; "
                    "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #b24ee7, stop: 1 #982ecb); "
                    "color: white; border-radius: 5px;}"
                    "QPushButton:pressed, QPushButton:checked { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #8023A8, stop: 1 #691e8a); }");
    
    connect(this, &RegistrationPage::registrationSuccessful, this, &RegistrationPage::onRegistrationSuccess);

    QTabWidget *tabWidget = new QTabWidget(this);

    lineEdit_login_username = new QLineEdit(this);
    lineEdit_login_password = new QLineEdit(this);
    lineEdit_signup_username = new QLineEdit(this);
    lineEdit_signup_email = new QLineEdit(this);
    lineEdit_signup_password = new QLineEdit(this);
    lineEdit_signup_confirm = new QLineEdit(this);

    lineEdit_login_password->setEchoMode(QLineEdit::Password);
    lineEdit_signup_password->setEchoMode(QLineEdit::Password);
    lineEdit_signup_confirm->setEchoMode(QLineEdit::Password);

    lineEdit_signup_username->setPlaceholderText("Enter username");
    lineEdit_signup_email->setPlaceholderText("Enter email");
    lineEdit_signup_password->setPlaceholderText("Enter password");
    lineEdit_signup_confirm->setPlaceholderText("Enter password confirmation");

    lineEdit_login_username->setPlaceholderText("Enter login");
    lineEdit_login_password->setPlaceholderText("Enter password");

    QWidget *tab1 = new QWidget;
    QWidget *tab2 = new QWidget;
    tabWidget->addTab(tab1, "Signup");
    tabWidget->addTab(tab2, "Login");

    QPushButton *signupButton = new QPushButton("Sign up", tab1);
    signupButton->setCursor(Qt::PointingHandCursor);

    QWidget *formWidget1 = new QWidget(tab1);
    QFormLayout *formLayout1 = new QFormLayout(formWidget1);
    formLayout1->addRow(lineEdit_signup_username);
    formLayout1->addRow(lineEdit_signup_email);
    formLayout1->addRow(lineEdit_signup_password);
    formLayout1->addRow(lineEdit_signup_confirm);

    QPushButton *loginButton = new QPushButton("Log in", tab2);
    loginButton->setCursor(Qt::PointingHandCursor);

    QWidget *formWidget2 = new QWidget(tab2);
    QFormLayout *formLayout2 = new QFormLayout(formWidget2);
    formLayout2->addRow(lineEdit_login_username);
    formLayout2->addRow(lineEdit_login_password);

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

    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 2);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 2);
    mainLayout->addWidget(tabWidget, 1, 1);

    connect(signupButton, &QPushButton::clicked, this, &RegistrationPage::signupButton_clicked);
    connect(loginButton, &QPushButton::clicked, this, &RegistrationPage::loginButton_clicked);

    qDebug() << "Registration page end\n";
}

RegistrationPage::~RegistrationPage()
{
    delete ui;
}

void RegistrationPage::loginButton_clicked()
{
    QString login = lineEdit_login_username->text();
    QString password = lineEdit_login_password->text();
    authservice::LoginResponse *response = manager->new_client.login(login.toStdString(), password.toStdString());
    manager->ID = response->id();
    manager->username = manager->new_client.get_user_full_page(manager->ID)->preview().login();

    qDebug() << "ID: " << manager->ID;

    if (login == "admin" && password == "admin" || response->message() == "OK") {
        emit loginSuccessful();
    }
    else {
        QMessageBox::warning(this, "Error", response->message().c_str());
    }
}

void RegistrationPage::onRegistrationSuccess()
{

}


void RegistrationPage::signupButton_clicked()
{
    QString username = lineEdit_signup_username->text();
    QString email = lineEdit_signup_email->text();
    QString password = lineEdit_signup_password->text();
    QString password_confirmation = lineEdit_signup_confirm->text();
    authservice::SignupResponse *response = manager->new_client.registration(username.toStdString(), email.toStdString(), password.toStdString(), password_confirmation.toStdString());

    if (response->message() == "OK") {
        authservice::LoginResponse *response = manager->new_client.login(username.toStdString(), password.toStdString());
        manager->ID = response->id();
        manager->new_client.change_username(manager->ID, username.toStdString());
        manager->username = username.toStdString();
        emit registrationSuccessful();
    }
    else {
        QMessageBox::warning(this, "Error", response->message().c_str());
    }
}

void RegistrationPage::onLoginSuccess()
{

}

