#ifndef CATEGORIZEDIALOG_H
#define CATEGORIZEDIALOG_H

#include "domainsql.h"

namespace Ui
{
    class categorizedialog;
}

class categorizedialog : public QDialog
{
    Q_OBJECT

public:
    explicit categorizedialog(QWidget *parent = nullptr);
    QString user = "Unknown";
    domainsql *categorizeOp = nullptr;
    ~categorizedialog();

signals:
    void send_request();

private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_changeButton_clicked();

private:
    Ui::categorizedialog *ui;
};

#endif // CATEGORIZEDIALOG_H
