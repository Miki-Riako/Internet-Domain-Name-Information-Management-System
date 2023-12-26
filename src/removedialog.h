#ifndef REMOVEDIALOG_H
#define REMOVEDIALOG_H

#include "domainsql.h"

namespace Ui
{
    class removedialog;
}

class removedialog : public QDialog
{
    Q_OBJECT

public:
    explicit removedialog(QWidget *parent = nullptr);
    domainsql *removeOp = nullptr;
    ~removedialog();

signals:
    void send_request();

private slots:
    void on_searchDomain_clicked();

private:
    Ui::removedialog *ui;
};

#endif // REMOVEDIALOG_H
