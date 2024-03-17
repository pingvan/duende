#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

namespace Ui {
class SideBar;
}

class SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = nullptr);
    ~SideBar();

signals:
    void showChatsPage();
    void showRecommendationsPage();
    void showFollowsPage();

private slots:
    void on_toolButton_chats_clicked();

    void on_toolButton_recommendations_clicked();

    void on_toolButton_follows_clicked();

private:
    Ui::SideBar *ui;
};

#endif // SIDEBAR_H
