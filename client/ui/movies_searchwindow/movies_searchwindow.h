#ifndef MOVIES_SEARCHWINDOW_H
#define MOVIES_SEARCHWINDOW_H

#include <QAbstractItemView>
#include <QCompleter>
#include <QInputDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>
#include "manager.h"

namespace Ui {
class Movies_SearchWindow;
}

class Movies_SearchWindow : public QWidget {
    Q_OBJECT

public:
    explicit Movies_SearchWindow(
        Manager *managerPtr,
        QWidget *parent = nullptr
    );
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
    QMap<QString, int> movies_map;
};

#endif  // MOVIES_SEARCHWINDOW_H
