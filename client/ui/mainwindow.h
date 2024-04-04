#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "registrationpage.h"
#include "profile.h"
#include "chats.h"
#include "sidebar.h"

#include <QMainWindow>
#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openProfilePage();

private:
    Ui::MainWindow *ui;
    QStackedWidget *pages;
    SideBar *sidebar;
    RegistrationPage *registrationPage;
    Profile *profile;
    Chats *chats;
};
#endif // MAINWINDOW_H
