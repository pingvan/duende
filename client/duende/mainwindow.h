#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>
#include "registrationpage.h"
#include "profile.h"
#include "chats.h"
#include "sidebar.h"

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
    void showChatsPageSlot();
    void showRecommendationsPageSlot();
    void showFollowsPageSlot();
    void openProfilePage();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    SideBar *sidebar;
    RegistrationPage *registrationPage;
    Profile *profile;
    Chats *chats;
};
#endif // MAINWINDOW_H
