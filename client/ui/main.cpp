#include "mainwindow/mainwindow.h"
#include "registrationpage/registrationpage.h"
#include "questionnaire/questionnaire.h"
#include "../client.hpp"
#include "manager.h"
#include "interceptor.hpp"

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

    grpc::ChannelArguments args;
    std::vector<std::unique_ptr<grpc::experimental::ClientInterceptorFactoryInterface>> interceptor_creators;
    interceptor_creators.push_back(std::make_unique<interceptorFactory>());
    auto channel = grpc::experimental::CreateCustomChannelWithInterceptors(server_address, grpc::InsecureChannelCredentials(), args, std::move(interceptor_creators));

    Manager manager(channel);

    QMainWindow mainWindow;
    QStackedWidget *stackedWidget = new QStackedWidget;

    RegistrationPage *registrationPage = new RegistrationPage(&manager);
    stackedWidget->addWidget(registrationPage);

    QObject::connect(registrationPage, &RegistrationPage::registrationSuccessful, [&](){
        manager.new_client.init_chat_stream(manager.ID);
        manager.new_client.start_reading_invite_stream();
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
        manager.new_client.start_reading_invite_stream();
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
