#ifndef FOLLOWS_H
#define FOLLOWS_H

#include <QWidget>

namespace Ui {
class Follows;
}

class Follows : public QWidget
{
    Q_OBJECT

public:
    explicit Follows(QWidget *parent = nullptr);
    ~Follows();

private:
    Ui::Follows *ui;
};

#endif // FOLLOWS_H
