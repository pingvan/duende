#include <QApplication>
#include <QDebug>
#include <QDockWidget>
#include <QMainWindow>
#include <QSplashScreen>
#include <QStackedWidget>
#include <QTimer>
#include "../client.hpp"
#include "mainwindow/mainwindow.h"
#include "manager.h"
#include "questionnaire/questionnaire.h"
#include "registrationpage/registrationpage.h"

std::string server_address("0.0.0.0:50051");

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFont font = a.font();
    font.setPointSize(font.pointSize() + 1);
    a.setFont(font);

    a.setStyleSheet(
        "QWidget { background-color: #e9e9e9; }"
        "QPushButton { min-width: 100px; min-height: 30px; font-size: 16px; "
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #b24ee7, stop: 1 #982ecb); "
        "color: white; border-radius: 5px;}"
        "QPushButton:pressed, QPushButton:checked { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #8023A8, stop: 1 #691e8a); }" 
        "QLineEdit { font-size: 14px; color: #8023a8; min-height: 25px;}"
        "QPushButton#myButton { background-color: #e9e9e9; }"
    );

    // Splash screen
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/img/logo.jpg"));
    splash->show();
    QTimer::singleShot(3000, splash, SLOT(close()));

    // Manager
    Manager manager(
        grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials())
    );

    // Main window
    QMainWindow mainWindow;
    QStackedWidget *stackedWidget = new QStackedWidget;

    // Registration page
    RegistrationPage *registrationPage = new RegistrationPage(&manager);
    stackedWidget->addWidget(registrationPage);

    // Registration case
    QObject::connect(
        registrationPage, &RegistrationPage::registrationSuccessful,
        [&]() {
            manager.new_client.init_chat_stream(manager.ID);
            manager.new_client.init_invite_stream(manager.ID);

            // Questionnaire page
            Questionnaire *questionnairePage = new Questionnaire(&manager);
            stackedWidget->addWidget(questionnairePage);
            stackedWidget->setCurrentWidget(questionnairePage);
            QObject::connect(
                questionnairePage, &Questionnaire::questionnaireFilled,
                [&]() {
                    // Main page
                    MainWindow *mainPage = new MainWindow(&manager);
                    stackedWidget->addWidget(mainPage);
                    stackedWidget->setCurrentWidget(mainPage);
                }
            );
        }
    );

    // Login case
    QObject::connect(
        registrationPage, &RegistrationPage::loginSuccessful,
        [&]() {
            manager.new_client.init_chat_stream(manager.ID);
            manager.new_client.init_invite_stream(manager.ID);
            // Main page
            MainWindow *mainPage = new MainWindow(&manager);
            stackedWidget->addWidget(mainPage);
            stackedWidget->setCurrentWidget(mainPage);
        }
    );

    mainWindow.setCentralWidget(stackedWidget);
    QTimer::singleShot(2500, [&]() { mainWindow.showFullScreen(); });

    return a.exec();
}
