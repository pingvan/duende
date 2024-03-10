#ifndef DUENDE_H
#define DUENDE_H

#include <QMainWindow>
#include "duendedesktop.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Duende;
}
QT_END_NAMESPACE

class Duende : public QMainWindow
{
    Q_OBJECT

public:
    Duende(QWidget *parent = nullptr);
    ~Duende();

private slots:

    void on_pushButton_login_clicked();

private:
    Ui::Duende *ui;
    DuendeDesktop *duendeDesktop;
};
#endif // DUENDE_H
