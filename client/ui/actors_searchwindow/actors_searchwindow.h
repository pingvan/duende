#ifndef ACTORS_SEARCHWINDOW_H
#define ACTORS_SEARCHWINDOW_H

#include "manager.hpp"

#include <QWidget>
#include <QLineEdit>
#include <QStringListModel>
#include <QCompleter>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QAbstractItemView>
#include <QPushButton>


namespace Ui {
class Actors_SearchWindow;
}

class Actors_SearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Actors_SearchWindow(Manager* managerPtr, QWidget *parent = nullptr);
    QStringList searchActors(const QString &input);
    ~Actors_SearchWindow();

private slots:
    void updateActorsList(const QString &input);
    void saveChanges();

private:
    Ui::Actors_SearchWindow *ui;
    Manager *manager;
    QLineEdit *search_line;
    QStringListModel *model;
    QCompleter *completer;
};

#endif // ACTORS_SEARCHWINDOW_H
