#ifndef ACTORS_SEARCHWINDOW_H
#define ACTORS_SEARCHWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QStringListModel>
#include <QCompleter>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QAbstractItemView>

namespace Ui {
class Actors_SearchWindow;
}

class Actors_SearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Actors_SearchWindow(QWidget *parent = nullptr);
    ~Actors_SearchWindow();

private slots:
    void onEditingFinished();

private:
    Ui::Actors_SearchWindow *ui;
    QLineEdit *search_line;
    QStringListModel *model;
    QCompleter *completer;
};

#endif // ACTORS_SEARCHWINDOW_H
