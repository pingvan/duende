#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>

namespace Ui {
class Profile;
}

class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr);
    ~Profile();

private slots:
    void on_toolButton_edit_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::Profile *ui;
};

#endif // PROFILE_H
