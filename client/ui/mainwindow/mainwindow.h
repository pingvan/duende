#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "registrationpage/registrationpage.h"
#include "recommendations/recommendations.h"
#include "profile/profile.h"
#include "chats/chats.h"
#include "follows/follows.h"
#include "otherpage/otherpage.h"

#include <QMainWindow>
#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QToolButton>
#include <QToolBar>
#include <QPixmap>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Manager* managerPtr, QWidget *parent = nullptr);
    ~MainWindow();
    QIcon createIcon(const QString &path1, const QString &path2);

private slots:
    void openProfilePage();
    void openOtherPage(int id);

private:
    Ui::MainWindow *ui;
    Manager *manager;
    QStackedWidget *pages;
    RegistrationPage *registrationPage;
    Profile *profile;
    Chats *chats;
    OtherPage *otherPage;
};
#endif // MAINWINDOW_H
