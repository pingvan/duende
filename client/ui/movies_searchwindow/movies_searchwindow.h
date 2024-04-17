#ifndef MOVIES_SEARCHWINDOW_H
#define MOVIES_SEARCHWINDOW_H

#include "manager.hpp"

#include <QWidget>
#include <QLineEdit>
#include <QCompleter>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QPushButton>

namespace Ui {
class Movies_SearchWindow;
}

class Movies_SearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Movies_SearchWindow(Manager* managerPtr, QWidget *parent = nullptr);
    QStringList searchMovies(const QString &input);
    ~Movies_SearchWindow();

private slots:
    void updateMoviesList(const QString &input);
    void saveChanges();

private:
    Ui::Movies_SearchWindow *ui;
    Manager *manager;
    QLineEdit *search_line;
    QStringListModel *model;
    QCompleter *completer;
};

#endif // MOVIES_SEARCHWINDOW_H
