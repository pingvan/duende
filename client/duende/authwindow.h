    #ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>

namespace Ui {
class authwindow;
}

class authwindow : public QWidget
{
    Q_OBJECT

public:
    explicit authwindow(QWidget *parent = nullptr);
    ~authwindow();

private:
    Ui::authwindow *ui;
};

#endif // AUTHWINDOW_H
