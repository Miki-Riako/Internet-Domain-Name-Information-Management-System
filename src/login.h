#ifndef LOGIN_H
#define LOGIN_H

#include "domainsql.h"

namespace Ui
{
    class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    bool state[3] = {false, false, false};
    bool first[3] = {true, true, true};
    QString name = "Guest";
    QtMaterialAutoComplete *pwdLineEdit[3];

signals:
    void enter();
    void guest_enter();

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_gotoRegister_clicked();
    void on_gotoLogin_clicked();
    void on_guestButton_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
