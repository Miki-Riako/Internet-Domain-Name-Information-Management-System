#include "domainsql.h"

domainsql::domainsql(QObject *parent) : QObject{parent} {}

QString domainsql::removeFirstSegment(QString &domain)
{ // Find the first dot and delete it and its first string
    int dotIndex = domain.indexOf('.');
    if (dotIndex != -1)
        return domain.mid(dotIndex + 1);
    return "root";
}
bool domainsql::connecting(void)
{ // Load the Local file and connect the database
    QSettings settings("config.ini", QSettings::IniFormat);
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName(settings.value("Database/Name").toString());
    db.setUserName(settings.value("Database/User").toString());
    db.setPassword(settings.value("Database/Password").toString());
    return db.open();
}
void domainsql::loginConnect(void)
{ // When enter the login widget, try to connect the database
    if (QSqlDatabase::drivers().isEmpty()) { // When no drivers
        QMessageBox::information(nullptr, "No database drivers found", "If you are a new user, please enter as a guest and set the mysql database configuration in the settings section, then re-enter as an administrator. Otherwise, please check the database connection.");
        return;
    }
    if (!connecting())
        QMessageBox::information(nullptr, "Connection disconnected", "If you are a new user, please enter as a guest and set the mysql database configuration in the settings section, then re-enter as an administrator. Otherwise, please check the database connection.");
}
bool domainsql::connectDataBase(void)
{ // When user want to input their host to connect the database
    if (QSqlDatabase::drivers().isEmpty()) { // When no drivers
        QMessageBox::information(nullptr, "No database drivers found", "Database drivers required! Please check your SQL plugins.");
        return false;
    }
    if (!connecting()) {
        QMessageBox::information(nullptr, "Unable to open database", "Error occurred! You may input the wrong information. Or the database has already been connected.");
        return false;
    } else {
        QMessageBox::information(nullptr, "Connected to database", "Connected to database successfully.");
        return true;
    }
}
    // domainNode* domainsql::findNodeById(domainNode* rootNode, int targetId)
    // { // Using recursion to find the target id.
    //     if (!rootNode)
    //         return nullptr;
    //     if (rootNode->id == targetId)
    //         return rootNode;
    //     domainNode* foundNode = findNodeById(rootNode->firstChild, targetId);
    //     if (!foundNode)
    //         foundNode = findNodeById(rootNode->nextSibling, targetId);
    //     return foundNode;
    // }
int domainsql::getIdFromDomain(QString &domainName)
{ // Input the domain name and return the ID in mysql
    QSqlQuery query;
    query.prepare("SELECT id FROM domain WHERE domain_name = :domainName");
    query.bindValue(":domainName", domainName);
    if (query.exec() && query.next())
        return query.value("id").toInt();
    else { // The case that can not find the id
        QMessageBox::warning(nullptr, "Failed", "Error while accessing the database.");
        return -1;
    }
}
    // void domainsql::createDataTree(void)
    // { // Create the database tree
    //     QSqlQuery query(db);
    //     if (query.exec("SELECT * FROM domain")) { // Load the domain database
    //         while (query.next()) { // Traverse the database and download it to the tree
    //             domainNode* newNode = new domainNode;
    //             newNode->id             = query.value("id").toInt();
    //             newNode->domainLevel    = query.value("domain_name_level").toInt();
    //             newNode->domainName     = query.value("domain_name").toString();
    //             newNode->domainNameType = query.value("domain_name_type").toString();
    //             newNode->webName        = query.value("web_name").toString();
    //             newNode->sponsorName    = query.value("sponsor_name").toString();
    //             newNode->status         = query.value("status").toString();
    //             newNode->domainRegister = query.value("register").toString();
    //             newNode->contactInfo    = query.value("contact_information").toString();
    //             newNode->creator        = query.value("creator").toString();
    //             newNode->memo           = query.value("memo").toString();
    //             newNode->createDate     = query.value("create_date").toString();
    //             newNode->updateDate     = query.value("updated_date").toString();
    //             newNode->expirationDate = query.value("expiration_date").toString();
    //             int parentId = query.value("parent").toInt();
    //             if (parentId == 0) { // No parent node, the node is root
    //                 newNode->parent = nullptr;
    //                 newNode->nextSibling = nullptr;
    //                 newNode->firstChild = nullptr;
    //                 root = newNode;
    //             }
    //             else { // Node is not the root
    //                 domainNode* parentNode = findNodeById(root, parentId);
    //                 if (parentNode) { // When the parent node is not null
    //                     newNode->parent = parentNode;
    //                     newNode->nextSibling = nullptr;
    //                     newNode->firstChild = nullptr;
    //                     if (!parentNode->firstChild)
    //                         parentNode->firstChild = newNode;
    //                     else { // The case that the parent node has already a node
    //                         domainNode* lastChild = parentNode->firstChild;
    //                         while (lastChild->nextSibling)
    //                             lastChild = lastChild->nextSibling;
    //                         lastChild->nextSibling = newNode;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //     else
    //         QMessageBox::warning(nullptr, "Error", "Load the database failed, please check your database!");
    // }
    // void domainsql::upload(domainNode* node)
    // { // Upload the database
    //     if (!node)
    //         return;
    //     QSqlQuery query;
    //     query.prepare("INSERT INTO domain_temp (id, parent, first_child, next_sibling, domain_name, domain_name_type, domain_name_level, web_name, sponsor_name, status, register, contact_information, creator, create_date, memo, updated_date, expiration_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    //     query.addBindValue(node->id);
    //     query.addBindValue(node->parent ? node->parent->id : 0);
    //     query.addBindValue(node->firstChild ? node->firstChild->id : 0);
    //     query.addBindValue(node->nextSibling ? node->nextSibling->id : 0);
    //     query.addBindValue(node->domainName);
    //     query.addBindValue(node->domainNameType);
    //     query.addBindValue(node->domainLevel);
    //     query.addBindValue(node->webName);
    //     query.addBindValue(node->sponsorName);
    //     query.addBindValue(node->status);
    //     query.addBindValue(node->domainRegister);
    //     query.addBindValue(node->contactInfo);
    //     query.addBindValue(node->creator);
    //     query.addBindValue(node->createDate);
    //     query.addBindValue(node->memo);
    //     query.addBindValue(node->updateDate);
    //     query.addBindValue(node->expirationDate);
    //     if (!query.exec())
    //         QMessageBox::warning(nullptr, "Error", "Write the database failed, please check your database!");
    //     upload(node->firstChild);
    //     upload(node->nextSibling);
    // }
    // void domainsql::howToSearch(QString targetDomain)
    // { // Using BFS or DFS
    //     searchVector.clear();
    //     if (host[4] == "BFS")
    //         BFS(root, targetDomain);
    //     else
    //         DFS(root, targetDomain);
    // }
    // void domainsql::DFS(domainNode *domain, QString target)
    // { // Depth-First-Search
    //     if (domain == nullptr)
    //         return;
    //     if (domain->domainName.contains(target))
    //         searchVector.push_back(domain);
    //     for (domainNode *child = domain->firstChild; child != nullptr; child = child->nextSibling)
    //         DFS(child, target);
    // }
    // void domainsql::BFS(domainNode *root, QString target)
    // { // Breadth-First-Search
    //     if (root == nullptr)
    //         return;
    //     domainNode *queue[MAXSIZE];
    //     int front = 0;
    //     int rear = 0;
    //     queue[rear++] = root;
    //     while (front < rear) { // Using the queue to search in BFS
    //         domainNode *cur = queue[front++];
    //         if (cur->domainName.contains(target))
    //             searchVector.push_back(cur);
    //         if (cur->firstChild !=  nullptr)
    //             for (domainNode *ptr = cur->firstChild; ptr !=  nullptr; ptr = ptr->nextSibling)
    //                 queue[rear++] = ptr;
    //     }
    // }

    // void domainsql::insert(domainNode *child)
    // { // Insert a node into the tree
    //     howToSearch(child->domainName);
    //     if (searchVector.size() != 0)
    //         return; 
    //     QString father = removeFirstSegment(child->domainName);
    //     domainNode *parent = nullptr;
    //     howToSearch(father);
    //     if (searchVector.size() == 0) { // The input is wrong
    //         QMessageBox::information(nullptr, "Error", "The input is wrong!");
    //         return;
    //     }
    //     for (auto i = 0; i < searchVector.size(); i++)
    //         if (searchVector[i]->domainName == father) { // Find the father
    //             parent = searchVector[i];
    //             break;
    //         }
    //     if (host[3] == "Yes") { // Stored as the node
    //         int flagSibling = 0;
    //         if (father == child->domainName)
    //             parent = root;
    //         if (parent == nullptr || child == nullptr)
    //             return;
    //         if (parent->firstChild == nullptr) { // The parent node has no child
    //             child->parent = parent;
    //             parent->firstChild = child;
    //         }
    //         else { // If the parent node has children, find the last sibling node and insert it
    //             domainNode *sibling = parent->firstChild;
    //             while (sibling->nextSibling != nullptr) { // Find the last sibling
    //                 if (sibling->nextSibling->nextSibling == nullptr)
    //                     flagSibling = sibling->nextSibling->id;
    //                 sibling = sibling->nextSibling;
    //             }
    //             child->parent = parent;
    //             sibling->nextSibling = child;
    //         }
    //         for (int i = 0; true; ++i) { // Give an id
    //             int range = 2;
    //             if (child->domainLevel == 4)
    //                 range = 20000;
    //             else if (child->domainLevel == 3)
    //                 range = 2000;
    //             else if (child->domainLevel == 2)
    //                 range = 200;
    //             srand(time(NULL));
    //             child->id = rand() % (range * 10 - range + 1) + range;
    //             QSqlQuery queryCheck;
    //             queryCheck.prepare("SELECT id FROM domain WHERE id = :random");
    //             queryCheck.bindValue(":random", child->id);
    //             bool conflict = false;
    //             if (queryCheck.exec() && queryCheck.next())
    //                 conflict = false;
    //             else
    //                 conflict = true;
    //             if (conflict && flagSibling < child->id)
    //                 break;
    //             if (i > 1000) {
    //                 QMessageBox::information(nullptr, "Failed", "Your database is overflowed!");
    //                 break;
    //             }
    //         }
    //         QMessageBox::information(nullptr, "Success", "Insert Successfully!");
    //     }
    //     else
    //         QMessageBox::information(nullptr, "Failed", "You have not opened the store as node model.");
    // }
bool domainsql::remove(QString target)
{ // Delete a node from the tree
    // howToSearch(target);
    // if (searchVector.size() == 0)
    //     return false;
    // for (auto i = 0; i < searchVector.size(); i++)
    //     if (searchVector[i]->domainName == target) { // Find the target
    //         domainNode *parent = searchVector[i]->parent;
    //         domainNode *prev = NULL;
    //         for (domainNode *sibling = parent->firstChild; sibling != NULL; sibling = sibling->nextSibling) {
    //             if (sibling == searchVector[i])
    //                 break;
    //             prev = sibling;
    //         }
    //         if (prev != NULL)
    //             prev->nextSibling = searchVector[i]->nextSibling;
    //         else
    //             parent->firstChild = searchVector[i]->nextSibling;
    //         delete searchVector[i];
    //         return true;
    //     }
    // return false;
}
