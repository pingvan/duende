#ifndef PTOFILE_H
#define PTOFILE_H

#include <QWidget>

namespace Ui {
class Ptofile;
}

class Ptofile : public QWidget
{
    Q_OBJECT

public:
    explicit Ptofile(QWidget *parent = nullptr);
    ~Ptofile();

private:
    Ui::Ptofile *ui;
};

#endif // PTOFILE_H
