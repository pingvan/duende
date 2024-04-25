#include "mainwindow/mainwindow.h"
#include "registrationpage/registrationpage.h"
#include "questionnaire/questionnaire.h"
#include "../client.hpp"
#include "manager.h"

#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QDockWidget>
#include <QSplashScreen>
#include <QTimer>
#include <QDebug>

std::string server_address("0.0.0.0:50051");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyleSheet("QWidget { background-color: #e9e9e9; }");

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/img/logo.jpg"));
    splash->show();
    QTimer::singleShot(3000, splash, SLOT(close()));

    Manager manager(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    QMainWindow mainWindow;
    QStackedWidget *stackedWidget = new QStackedWidget;

    RegistrationPage *registrationPage = new RegistrationPage(&manager);
    stackedWidget->addWidget(registrationPage);

    QObject::connect(registrationPage, &RegistrationPage::registrationSuccessful, [&](){
        manager.new_client.init_chat_stream(manager.ID);
        Questionnaire *questionnairePage = new Questionnaire(&manager);
        stackedWidget->addWidget(questionnairePage);
        stackedWidget->setCurrentWidget(questionnairePage);
        QObject::connect(questionnairePage, &Questionnaire::questionnaireFilled, [&](){
            MainWindow *mainPage = new MainWindow(&manager);
            stackedWidget->addWidget(mainPage);
            stackedWidget->setCurrentWidget(mainPage);
        });
    });
    QObject::connect(registrationPage, &RegistrationPage::loginSuccessful, [&](){
        manager.new_client.init_chat_stream(manager.ID);
        MainWindow *mainPage = new MainWindow(&manager);
        stackedWidget->addWidget(mainPage);
        stackedWidget->setCurrentWidget(mainPage);
    });

    mainWindow.setCentralWidget(stackedWidget);
    QTimer::singleShot(2500, [&]() {
        mainWindow.showFullScreen();
    });
    
    return a.exec();
}
