#ifndef DOMAINSQL_H
#define DOMAINSQL_H

#include "header.h"

    // typedef struct domainNode
    // {
    //     int id;
    //     int domainLevel;
    //     QString domainName;
    //     QString domainNameType;
    //     QString webName;
    //     QString sponsorName;
    //     QString status;
    //     QString domainRegister;
    //     QString contactInfo;
    //     QString creator;
    //     QString createDate;
    //     QString memo;
    //     QString updateDate;
    //     QString expirationDate;
    //     struct domainNode *parent;      // Parent node
    //     struct domainNode *firstChild;  // First child node
    //     struct domainNode *nextSibling; // Next sibling node
    //     // int parentId;                    // Parent node ID
    //     // int firstChildId;                // First child node ID
    //     // int nextSiblingId;               // Next sibling node ID
    // } domainNode;

class domainsql : public QObject
{
    Q_OBJECT
public:
    explicit domainsql(QObject *parent = nullptr);
    QSqlDatabase db;
    QString host[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    bool connecting(void);
    void loginConnect(void);
    bool connectDataBase(void);
        // domainNode *root = nullptr;
    QString XOREncode(QString originalPwd);
    QString Sha256Encode(QString originalPwd);
    QString removeFirstSegment(QString &domain);
        // domainNode *findNodeById(domainNode *rootNode, int targetId);
    int getIdFromDomain(QString &domainName);
        // void createDataTree(void);
        // void upload(domainNode* node);
        // vector<domainNode* > searchVector;
        // void howToSearch(QString targetDomain);
        // void DFS(domainNode *domain, QString target);
        // void BFS(domainNode *root, QString target);
        // void insert(domainNode *node);
    bool remove(QString target);

signals:
private:
};

#endif // DOMAINSQL_H
