#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>
#include "chats/chats.h"
#include "follows/follows.h"
#include "otherpage/otherpage.h"
#include "profile/profile.h"
#include "recommendations/recommendations.h"
#include "registrationpage/registrationpage.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Manager *managerPtr, QWidget *parent = nullptr);
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
    Follows followsInstance;
};
#endif  // MAINWINDOW_H
