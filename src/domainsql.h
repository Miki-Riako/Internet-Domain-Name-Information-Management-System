#ifndef DOMAINSQL_H
#define DOMAINSQL_H

#include "header.h"


class domainsql : public QObject
{
    Q_OBJECT
public:
    explicit domainsql(QObject *parent = nullptr);
    QSqlDatabase db;
    bool connecting(void);
    void loginConnect(void);
    bool connectDataBase(void);
    QString XOREncode(QString originalPwd);
    QString Sha256Encode(QString originalPwd);
    QString removeFirstSegment(QString &domain);
    int getIdFromDomain(QString &domainName);
    bool remove(QString target);

signals:
private:
};

#endif // DOMAINSQL_H
