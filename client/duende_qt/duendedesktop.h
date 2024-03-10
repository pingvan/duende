#ifndef DUENDEDESKTOP_H
#define DUENDEDESKTOP_H

#include <QDialog>

namespace Ui {
class DuendeDesktop;
}

class DuendeDesktop : public QDialog
{
    Q_OBJECT

public:
    explicit DuendeDesktop(QWidget *parent = nullptr);
    ~DuendeDesktop();

private:
    Ui::DuendeDesktop *ui;
};

#endif // DUENDEDESKTOP_H
