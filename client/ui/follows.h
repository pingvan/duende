#ifndef FOLLOWS_H
#define FOLLOWS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QStringListModel>
#include <QCompleter>
#include <QSpacerItem>

namespace Ui {
class Follows;
}

class Follows : public QWidget
{
    Q_OBJECT

public:
    explicit Follows(QWidget *parent = nullptr);
    ~Follows();

private slots:

private:
    Ui::Follows *ui;
    QLineEdit *search_line;
    QStringListModel *model;
    QCompleter *completer;
};

#endif // FOLLOWS_H
