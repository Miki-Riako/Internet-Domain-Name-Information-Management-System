#include "domainsql.h"

domainsql::domainsql(QObject *parent) : QObject{parent} {}
QString domainsql::XOREncode(const QString &originalPwd)
{ // Encode and Decode
    QString key = "a-secret-key-a-secret-key-a-secret-key-a-secret-key-a-secret-key-a-secret-key-a-secret-key-a-secret-key-a-secret-key-a-secret-key";
    QString encodedPwd;
    for(int i = 0; i < originalPwd.length(); ++i)
        encodedPwd.append(QChar(originalPwd[i].unicode() ^ key[i % key.length()].unicode()));
    return encodedPwd;
}
QString domainsql::Sha256Encode(const QString &originalPwd) {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(originalPwd.toUtf8());
    return hash.result().toHex();
}
QString domainsql::removeFirstSegment(const QString &domain)
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
    db.setHostName(settings.value("Database/HostName").toString());
    db.setPort(settings.value("Database/Port").toInt());
    db.setDatabaseName(settings.value("Database/Name").toString());
    db.setUserName(settings.value("Database/User").toString());
    db.setPassword(XOREncode(settings.value("Database/Password").toString()));
    if (db.open()) {
        QSqlQuery query(db);
        QString createUserTable = R"(
            CREATE TABLE IF NOT EXISTS user (
                id INT PRIMARY KEY AUTO_INCREMENT,
                user_name VARCHAR(255),
                password VARCHAR(255)
            )
        )";
        if (!query.exec(createUserTable)) {
            QMessageBox::critical(nullptr, "Error", "When initialized, unable to create table user!");
            return db.open();
        }
        QString createDomainTable = R"(
            CREATE TABLE IF NOT EXISTS domain (
                id INT PRIMARY KEY AUTO_INCREMENT,
                DomainName VARCHAR(255),
                DomainType VARCHAR(255),
                DomainLevel INT,
                WebName VARCHAR(255),
                SponsorName VARCHAR(255),
                Status VARCHAR(255),
                Register VARCHAR(255),
                ContactInformation VARCHAR(255),
                Creator VARCHAR(255),
                CreateDate VARCHAR(255),
                Memo VARCHAR(255),
                UpdatedDate VARCHAR(255),
                ExpiredDate VARCHAR(255)
            )
        )";
        if (!query.exec(createDomainTable)) {
            QMessageBox::critical(nullptr, "Error", "When initialized, unable to create table domain!");
            return db.open();
        }
        if (!query.exec("SELECT COUNT(*) FROM domain")) {
            QMessageBox::critical(nullptr, "Error", "When initialized, unable to query table domain!");
            return db.open();
        }
        if (query.next() && query.value(0).toInt() == 0) {
            if (!query.exec("INSERT INTO domain (DomainName, DomainType, DomainLevel) VALUES ('root', 'root', 0)")) {
                QMessageBox::critical(nullptr, "Error", "When initialized, unable to insert root node record.");
                return db.open();
            }
            QMessageBox::information(nullptr, "Success", "Database initialized!");
        }
    }
    return db.open();
}
void domainsql::backup(void)
{ // Backup the database
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Error", "Database not found!");
        return;
    }
    QSqlQuery query(db);
    if (!query.exec("SHOW TABLES LIKE 'domain_backup'")) {
        QMessageBox::critical(nullptr, "Error", "Cannot backup!");
        return;
    }
    if (query.next() && !query.exec("DROP TABLE domain_backup")) {
        QMessageBox::critical(nullptr, "Error", "Cannot backup!");
        return;
    }
    if (query.next() && !query.exec("DROP TABLE domain_backup")) {
        QMessageBox::critical(nullptr, "Error", "Cannot backup!");
        return;
    }
    if (!query.exec("CREATE TABLE domain_backup AS SELECT * FROM domain")) {
        QMessageBox::critical(nullptr, "Error", "Cannot backup!");
        return;
    }
}
void domainsql::loginConnect(void)
{ // When enter the login widget, try to connect the database and backup the database
    if (QSqlDatabase::drivers().isEmpty()) { // When no drivers
        QMessageBox::information(nullptr, "No database drivers found", "If you are a new user, please enter as a guest and set the mysql database configuration in the settings section, then re-enter as an administrator. Otherwise, please check the database connection.");
        return;
    }
    if (!connecting()) {
        QMessageBox::information(nullptr, "Connection disconnected", "If you are a new user, please enter as a guest and set the mysql database configuration in the settings section, then re-enter as an administrator. Otherwise, please check the database connection.");
        return;
    }
    if (db.open())
        backup();
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
int domainsql::getLevel(const QString &domain)
{ // Get the level of the domain
    if (domain == "root")
        return 0;
    return domain.count('.') + 1;
}
bool domainsql::domainExists(const QString &domainName)
{
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM domain WHERE DomainName = :DomainName");
    query.bindValue(":DomainName", domainName);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", query.lastError().text());
        return false;
    }
    if (query.next())
        return query.value(0).toInt() > 0;
    return false;
}
bool domainsql::hasChildren(const QString &domainName)
{
    QSqlQuery query(db);
    QString checkQuery = QString("SELECT COUNT(*) FROM domain WHERE DomainName LIKE :pattern");
    query.prepare(checkQuery);
    query.bindValue(":pattern", "%." + domainName);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", query.lastError().text());
        return false;
    }
    if (query.next() && query.value(0).toInt() > 0)
        return true;
    return false;
}
bool domainsql::insert(const QString &target, const int &level, const QString &user)
{ // Insert a node
    QRegExp regex("^[A-Za-z0-9-]+(\\.[A-Za-z0-9-]+)*$");
    if (!regex.exactMatch(target)) {
        QMessageBox::warning(nullptr, "Error", "Invalid domain name: " + target);
        return false;
    }
    if (domainExists(target)) {
        QMessageBox::warning(nullptr, "Error", "Already exits!");
        return false;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO domain (DomainName, DomainLevel, Creator, CreateDate) VALUES (:DomainName, :DomainLevel, :Creator, :CreateDate)");
    query.bindValue(":DomainName", target);
    query.bindValue(":DomainLevel", level);
    query.bindValue(":Creator", user);
    query.bindValue(":CreateDate", QDateTime::currentDateTime().toString("MM/dd/yyyy"));
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Failed to insert the domain" + target);
        return false;
    }
    return true;
}
bool domainsql::remove(const QString &target)
{ // Delete a node
    if (!domainExists(target)) {
        QMessageBox::warning(nullptr, "Error", "Not Found!");
        return false;
    }
    QSqlQuery query(db);
    query.prepare("DELETE FROM domain WHERE DomainName = :DomainName");
    query.bindValue(":DomainName", target);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Failed to remove the domain" + target);
        return false;
    }
    return true;
}
bool domainsql::removeWithChildren(const QString &domainName)
{ // Remove the node including its children
    db.transaction();
    try {
        if (!recursiveDelete(domainName)) {
            db.rollback();
            return false;
        }
        db.commit();
        return true;
    } catch (...) {
        db.rollback();
        return false;
    }
}
bool domainsql::recursiveDelete(const QString &domainName)
{ // Help function to implement a recursive operation
    if (!remove(domainName))
        return false;
    QSqlQuery query(db);
    query.prepare("SELECT DomainName FROM domain WHERE DomainName LIKE :pattern");
    query.bindValue(":pattern", "%." + domainName);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", query.lastError().text());
        return false;
    }
    while (query.next()) {
        QString childDomain = query.value(0).toString();
        if (!recursiveDelete(childDomain))
            return false;
    }
    return true;
}