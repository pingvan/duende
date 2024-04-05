#include "mainwindow.h"
#include "sidebar.h"
#include "registrationpage.h"
#include "questionnaire.h"
#include "../client.hpp"
#include "../manager.hpp"

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

    a.setStyleSheet("QWidget { background-color: #aa6fbf; }");

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/img/logo.jpg"));
    splash->show();
    QTimer::singleShot(2500, splash, SLOT(close()));

    Manager manager(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    QMainWindow mainWindow;
    QStackedWidget *stackedWidget = new QStackedWidget;

    RegistrationPage *registrationPage = new RegistrationPage(&manager);
    stackedWidget->addWidget(registrationPage);

    Questionnaire *questionnairePage = new Questionnaire(&manager);
    stackedWidget->addWidget(questionnairePage);

    MainWindow *mainPage = new MainWindow(&manager);
    stackedWidget->addWidget(mainPage);

    QObject::connect(registrationPage, &RegistrationPage::registrationSuccessful, [&](){
        stackedWidget->setCurrentWidget(questionnairePage);
    });
    QObject::connect(registrationPage, &RegistrationPage::loginSuccessful, [&](){
        stackedWidget->setCurrentWidget(mainPage);
    });
    QObject::connect(questionnairePage, &Questionnaire::questionnaireFilled, [&](){
        stackedWidget->setCurrentWidget(mainPage);
    });

    mainWindow.setCentralWidget(stackedWidget);
    QTimer::singleShot(2500, [&]() {
        mainWindow.showFullScreen();
    });
    
    return a.exec();
}
