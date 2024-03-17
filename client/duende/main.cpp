#include "mainwindow.h"
#include "sidebar.h"
#include "registrationpage.h"
#include "questionnaire.h"
#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QDockWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWindow;
    QStackedWidget *stackedWidget = new QStackedWidget;

    RegistrationPage *registrationPage = new RegistrationPage();
    stackedWidget->addWidget(registrationPage);

    Questionnaire *questionnairePage = new Questionnaire();
    stackedWidget->addWidget(questionnairePage);

    MainWindow mainPage;
    stackedWidget->addWidget(&mainPage);

    QObject::connect(registrationPage, &RegistrationPage::registrationSuccessful, [&](){
        stackedWidget->setCurrentWidget(questionnairePage);
    });
    QObject::connect(questionnairePage, &Questionnaire::questionnaireFilled, [&](){
        stackedWidget->setCurrentWidget(&mainPage);
    });

    mainWindow.setCentralWidget(stackedWidget);
    mainWindow.showFullScreen();

    return a.exec();
}
