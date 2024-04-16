#ifndef MOVIES_SEARCHWINDOW_H
#define MOVIES_SEARCHWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QCompleter>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStringListModel>
#include <QAbstractItemView>

namespace Ui {
class Movies_SearchWindow;
}

class Movies_SearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Movies_SearchWindow(QWidget *parent = nullptr);
    ~Movies_SearchWindow();

private slots:
    void onEditingFinished();

private:
    Ui::Movies_SearchWindow *ui;
    QLineEdit *search_line;
    QStringListModel *model;
    QCompleter *completer;
};

#endif // MOVIES_SEARCHWINDOW_H
