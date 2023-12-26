#ifndef INSERTDIALOG_H
#define INSERTDIALOG_H

#include "domainsql.h"

namespace Ui
{
    class insertdialog;
}

class insertdialog : public QDialog
{
    Q_OBJECT

public:
    explicit insertdialog(QWidget *parent = nullptr);
    QString user = "Unknown";
    domainsql *insertOp = nullptr;
    ~insertdialog();

signals:
    void send_request();

private slots:
    void on_confirmButton_clicked();

private:
    Ui::insertdialog *ui;
};

#endif // INSERTDIALOG_H
