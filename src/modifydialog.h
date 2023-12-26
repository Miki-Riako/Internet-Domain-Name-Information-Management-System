#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H

#include "domainsql.h"

namespace Ui
{
    class modifydialog;
}

class modifydialog : public QDialog
{
    Q_OBJECT

public:
    explicit modifydialog(QWidget *parent = nullptr);
    QString user = "Unknown";
    domainsql *modifyOp = nullptr;
    ~modifydialog();

signals:
    void send_request();

private slots:
    void on_confirmButton_clicked();

private:
    Ui::modifydialog *ui;
};

#endif // MODIFYDIALOG_H
