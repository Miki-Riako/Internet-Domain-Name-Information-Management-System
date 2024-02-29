#ifndef ESTABLISHDIALOG_H
#define ESTABLISHDIALOG_H

#include "domainsql.h"

namespace Ui {
class establishdialog;
}

class establishdialog : public QDialog
{
    Q_OBJECT

public:
    explicit establishdialog(QWidget *parent = nullptr);
    QString user = "Unknown";
    domainsql *establishOp = nullptr;
    QString randomString(int length);
    void establish(QString domain);
    ~establishdialog();

signals:
    void send_request();

private slots:
    void on_randomButton_clicked();
    void on_fileButton_clicked();
    void on_peopleButton_clicked();

private:
    Ui::establishdialog *ui;
};

#endif // ESTABLISHDIALOG_H
