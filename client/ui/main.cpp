#include <QApplication>
#include <QDebug>
#include <QDockWidget>
#include <QMainWindow>
#include <QSplashScreen>
#include <QStackedWidget>
#include <QTimer>
#include <QToolTip>
#include "../client.hpp"
#include "mainwindow/mainwindow.h"
#include "manager.h"
#include "questionnaire/questionnaire.h"
#include "registrationpage/registrationpage.h"

std::string server_address("0.0.0.0:50051");

std::string ReadFile(const std::filesystem::path &path) {
    auto cert_path = std::filesystem::current_path().parent_path() / path;
    std::ifstream file(cert_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << cert_path << std::endl;
        std::exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::shared_ptr<grpc::ChannelCredentials> GetCredentials() {
    grpc::SslCredentialsOptions ssl_opts;
    ssl_opts.pem_root_certs = ReadFile("certs/ca.crt");
    ssl_opts.pem_private_key = ReadFile("certs/client.key");
    ssl_opts.pem_cert_chain = ReadFile("certs/client.crt");
    return grpc::SslCredentials(ssl_opts);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFont font = a.font();
    font.setPointSize(16);
    a.setFont(font);

    a.setStyleSheet(
        "QWidget { background-color: #e9e9e9; }"
        "QPushButton#сhatButton { width: 30px; height: 30px; font-size: 24px; "
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
        "#b24ee7, stop: 1 #982ecb); "
        "color: white; border-radius: 5px;}"
        "QPushButton { min-width: 100px; min-height: 40px; font-size: 24px; "
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
        "#b24ee7, stop: 1 #982ecb); "
        "color: white; border-radius: 5px;}"
        "QPushButton:pressed, QPushButton:checked { background: "
        "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #8023A8, stop: 1 "
        "#691e8a); }"
        "QLineEdit { font-size: 18px; color: #8023a8; min-height: 40px;}"
        "QPushButton#myButton { background-color: #e9e9e9; }"
        "QToolTip { font-size: 16px; color: black;}"
        "QPushButton#сhatButton { width: 30px; height: 30px; font-size: 24px; "
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
        "#b24ee7, stop: 1 #982ecb); "
        "color: white; border-radius: 5px;}"
    );

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/img/logo.jpg"));
    splash->show();
    QTimer::singleShot(3000, splash, SLOT(close()));

    Manager manager(grpc::CreateChannel(server_address, GetCredentials()));

    QMainWindow mainWindow;
    QStackedWidget *stackedWidget = new QStackedWidget;

    RegistrationPage *registrationPage = new RegistrationPage(&manager);
    stackedWidget->addWidget(registrationPage);

    QObject::connect(
        registrationPage, &RegistrationPage::registrationSuccessful,
        [&]() {
            manager.new_client.init_chat_stream(manager.ID);
            manager.new_client.init_invite_stream(manager.ID);
            manager.new_client.init_invite_chat_stream(manager.ID);

            Questionnaire *questionnairePage = new Questionnaire(&manager);
            stackedWidget->addWidget(questionnairePage);
            stackedWidget->setCurrentWidget(questionnairePage);
            QObject::connect(
                questionnairePage, &Questionnaire::questionnaireFilled,
                [&]() {
                    MainWindow *mainPage = new MainWindow(&manager);
                    stackedWidget->addWidget(mainPage);
                    stackedWidget->setCurrentWidget(mainPage);
                }
            );
        }
    );

    QObject::connect(
        registrationPage, &RegistrationPage::loginSuccessful,
        [&]() {
            manager.new_client.init_chat_stream(manager.ID);
            manager.new_client.init_invite_stream(manager.ID);
            manager.new_client.init_invite_chat_stream(manager.ID);

            MainWindow *mainPage = new MainWindow(&manager);
            stackedWidget->addWidget(mainPage);
            stackedWidget->setCurrentWidget(mainPage);
        }
    );

    QObject::connect(&manager, &Manager::needLogin, [&]() {
        stackedWidget->setCurrentIndex(0);
        manager.logout();
    });

    mainWindow.setCentralWidget(stackedWidget);
    QTimer::singleShot(2500, [&]() { mainWindow.showFullScreen(); });

    return a.exec();
}