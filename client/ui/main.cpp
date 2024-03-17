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

std::string server_address("0.0.0.0:50051");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Manager manager(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    QMainWindow mainWindow;
    QStackedWidget *stackedWidget = new QStackedWidget;

    RegistrationPage *registrationPage = new RegistrationPage(&manager);
    stackedWidget->addWidget(registrationPage);

    Questionnaire *questionnairePage = new Questionnaire();
    stackedWidget->addWidget(questionnairePage);

    MainWindow mainPage;
    stackedWidget->addWidget(&mainPage);

    QObject::connect(registrationPage, &RegistrationPage::registrationSuccessful, [&](){
        stackedWidget->setCurrentWidget(questionnairePage);
    });
    QObject::connect(registrationPage, &RegistrationPage::loginSuccessful, [&](){
        stackedWidget->setCurrentWidget(&mainPage);
    });
    QObject::connect(questionnairePage, &Questionnaire::questionnaireFilled, [&](){
        stackedWidget->setCurrentWidget(&mainPage);
    });

    mainWindow.setCentralWidget(stackedWidget);
    mainWindow.showFullScreen();

    return a.exec();
}
