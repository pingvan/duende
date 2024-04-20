#ifndef REGISTRATIONPAGE_H
#define REGISTRATIONPAGE_H

#include <QWidget>
#include "../manager.hpp"

namespace Ui {
class RegistrationPage;
}

class RegistrationPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationPage(Manager* managerPtr, QWidget *parent = nullptr);
    ~RegistrationPage();

private slots:
    void on_pushButton_login_clicked();
    void onRegistrationSuccess();
    void onLoginSuccess();

    void on_pushButton_signup_clicked();

signals:
    void registrationSuccessful();
    void loginSuccessful();

private:
    Ui::RegistrationPage *ui;
    Manager *manager;
};

#endif // REGISTRATIONPAGE_H
