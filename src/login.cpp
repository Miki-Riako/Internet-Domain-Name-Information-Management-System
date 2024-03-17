#include "login.h"
#include "ui_login.h"
Login::Login(QWidget *parent) : QWidget(parent), ui(new Ui::Login)
{
    ui->setupUi(this);
    /*
    Eyes Connect
    The following connect functions are about the eyes.
    Including the initial start of a eye and the mutual changes of eyes.
    */
    QAction *eye[3];
    eye[0] = new QAction(this);
    eye[1] = new QAction(this);
    eye[2] = new QAction(this);
    pwdLineEdit[0] = ui->loginPwdLineEdit;
    pwdLineEdit[1] = ui->registerPwdLineEdit;
    pwdLineEdit[2] = ui->registerConfirmLineEdit;
    for (int i = 0; i < 3; i++) { // Add eyes icon to the password line edits
        eye[i]->setIcon(QIcon(":/images/password-invisible.png"));
        connect(pwdLineEdit[i], &QtMaterialAutoComplete::textEdited, this, [=]{
            if(first[i]) {
                pwdLineEdit[i]->addAction(eye[i], QLineEdit::TrailingPosition);
                first[i] = false;
            }
        });
        connect(eye[i], &QAction::triggered, this, [=]() mutable{
            if (!state[i]) {
                eye[i]->setIcon(QIcon(":/images/password-visible.png"));
                pwdLineEdit[i]->addAction(eye[i], QLineEdit::TrailingPosition);
                state[i] = true;
                pwdLineEdit[i]->setEchoMode(QLineEdit::Normal);
            }
            else {
                eye[i]->setIcon(QIcon(":/images/password-invisible.png"));
                pwdLineEdit[i]->addAction(eye[i], QLineEdit::TrailingPosition);
                state[i] = false;
                pwdLineEdit[i]->setEchoMode(QLineEdit::Password);
            }
        });
    }
    ui->tipsR->setFixedHeight(20);
    ui->tipsL->setFixedHeight(20);
    // Connect
    on_gotoLogin_clicked();
}
Login::~Login() {delete ui;}

void Login::fade(auto *control, const int &duration, const int &startValue, const int &endValue)
{ // The function can provide the fade in and fade out animations.
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(control);
    control->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity", control);
    animation->setDuration(duration);
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->start();
}
void Login::hideLayout(auto *layout)
{ // The function aims to hide a layout
    layout->setEnabled(false);
    for (auto i = 0; i < layout->count(); i++) { // Traverse all the item
        QWidget *ptr = layout->itemAt(i)->widget();
        if (ptr != nullptr) { // And then set it invisible
            ptr->setVisible(false);
            fade(ptr, 1500, 1, 0);
        }
    }
}
void Login::showLayout(auto *layout)
{ // The function aims to show the layout hided
    layout->setEnabled(true);
    for (auto i = 0; i < layout->count(); i++) { // Traverse all the item
        QWidget *ptr = layout->itemAt(i)->widget();
        if (ptr != nullptr) { // And then set it visible
            ptr->setVisible(true);
            fade(ptr, 1500, 0, 1);
        }
    }
}
QString Login::Sha256Encode(QString originalPwd)
{ // Encode
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(originalPwd.toUtf8());
    return hash.result().toHex();
}
bool Login::checkPasswordStrength(const QString &password)
{ // Check whether powerful
    bool hasLetter = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;
    for (const QChar &ch : password) {
        if (ch.isLetter())
            hasLetter = true;
        else if (ch.isDigit())
            hasDigit = true;
        else if (!ch.isSpace())
            hasSpecialChar = true;
    }
    int validTypesCount = (hasLetter ? 1 : 0) + (hasDigit ? 1 : 0) + (hasSpecialChar ? 1 : 0);
    return password.length() >= 8 && validTypesCount >= 2;
}
void Login::on_loginButton_clicked()
{
    QString userName = ui->loginUsernameLineEdit->text();
    QString password = Sha256Encode(ui->loginPwdLineEdit->text());
    if (userName.isEmpty() || password.isEmpty()) { // The case when the username and the password are all empty
        ui->tipsR->textLabel->setText("Please enter the username and pwd!");
        ui->tipsR->animationStart();
    }
    else { // Check wether can enter the program
        QSqlQuery query;
        QString qs = QString("select * from user where user_name = '%1' and password='%2'").arg(userName).arg(password);
        if (!query.exec(qs)) { // Password is not right
            ui->tipsR->textLabel->setText("Login Failed! Check your username and password.");
            ui->tipsR->animationStart();
            return;
        }
        if (query.next()) { // Get the info
            name = userName;
            emit enter();
        }
        else { // Enter failed
            ui->tipsR->textLabel->setText("Login Failed! Check your username and password.");
            ui->tipsR->animationStart();
        }
    }
}
void Login::on_registerButton_clicked()
{
    QString userName = ui->registerUsernameLineEdit->text();
    QString password = Sha256Encode(ui->registerPwdLineEdit->text());
    QString passwordConfirm = Sha256Encode(ui->registerConfirmLineEdit->text());
    if (userName.isEmpty() || password.isEmpty() || passwordConfirm.isEmpty()) { // The case when all are empty
        ui->tipsL->textLabel->setText("Please enter the username and pwd!");
        ui->tipsL->animationStart();
    }
    else if (password != passwordConfirm) { // The case when the both password are not the same
        ui->tipsL->textLabel->setText("The password is not the same!");
        ui->tipsL->animationStart();
    }
    else if (!checkPasswordStrength(password))
        QMessageBox::warning(this, "Warning", "Password is too weak! It must contain two of the letters, numbers, and symbols, and be at least 8 characters long.");
    else { // Successfully register
        QSqlQuery queryCheck;
        queryCheck.prepare("SELECT user_name FROM user WHERE user_name = :userName");
        queryCheck.bindValue(":userName", userName);
        if (queryCheck.exec() && queryCheck.next()) { // The condition that user already exits
            ui->tipsL->textLabel->setText("User name exits!");
            ui->tipsL->animationStart();
        }
        else { // Successfully register
            QSqlQuery queryInsert;
            queryInsert.prepare("INSERT INTO user(user_name, password) VALUES(:userName, :password)");
            queryInsert.bindValue(":userName", userName);
            queryInsert.bindValue(":password", password);
            if (queryInsert.exec()) {
                ui->tipsL->textLabel->setText("Register Successfully!");
                ui->tipsL->animationStart();
            }
            else {
                ui->tipsL->textLabel->setText("Register Failed!");
                ui->tipsL->animationStart();
            }
        }
    }
}
void Login::on_gotoRegister_clicked()
{
    hideLayout(ui->loginLayout);
    hideLayout(ui->loginButtonLayout);
    hideLayout(ui->loginQLayout);
    hideLayout(ui->registerWelcome);
    showLayout(ui->registerLayout);
    showLayout(ui->registerButtonLayout);
    showLayout(ui->loginWelcome);
}
void Login::on_gotoLogin_clicked()
{
    hideLayout(ui->registerLayout);
    hideLayout(ui->registerButtonLayout);
    hideLayout(ui->loginWelcome);
    showLayout(ui->loginLayout);
    showLayout(ui->loginButtonLayout);
    showLayout(ui->loginQLayout);
    showLayout(ui->registerWelcome);
}
void Login::on_guestButton_clicked() {emit guest_enter();}

