#ifndef GENRES_SEARCHWINDOW_H
#define GENRES_SEARCHWINDOW_H

#include "manager.h"

#include <QWidget>
#include <QLineEdit>
#include <QStringListModel>
#include <QCompleter>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QAbstractItemView>
#include <QPushButton>


namespace Ui {
class Genres_SearchWindow;
}

class Genres_SearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Genres_SearchWindow(Manager* managerPtr, QWidget *parent = nullptr);
    QStringList searchGenres(const QString &input);
    ~Genres_SearchWindow();

private slots:
    void updateGenresList(const QString &input);
    void saveChanges();

signals:
    void windowClosed();

private:
    Ui::Genres_SearchWindow *ui;
    Manager *manager;
    QLineEdit *search_line;
    QStringListModel *model;
    QCompleter *completer;
    QMap<QString, int> genres_map;
};

#endif // GENRES_SEARCHWINDOW_H
