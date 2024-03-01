#include "categorizedialog.h"
#include "ui_categorizedialog.h"

categorizedialog::categorizedialog(QWidget *parent) : QDialog(parent), ui(new Ui::categorizedialog)
{
    ui->setupUi(this);
}
categorizedialog::~categorizedialog()
{
    delete ui;
}
bool categorizedialog::columnExists(QString columnName)
{ // Wether the column exits
    QSqlQuery query(categorizeOp->db);
    QString checkQuery = QString("SELECT %1 FROM domain LIMIT 1").arg(columnName);
    if (query.exec(checkQuery))
        return true;
    else
        return false;
}
void categorizedialog::on_addButton_clicked()
{
    emit send_request();
    QSqlQuery query(categorizeOp->db);
    QString columnName = ui->addLineEdit->text().trimmed();
    if (columnExists(columnName)) {
        QMessageBox::information(this, "Failed", "Column already exists!");
        return;
    }
    QString alterQuery = QString("ALTER TABLE domain ADD COLUMN %1 VARCHAR(255)").arg(columnName);
    if (query.exec(alterQuery))
        QMessageBox::information(this, "Success", "Column added successfully!");
    else
        QMessageBox::warning(this, "Error", "Failed! Error: " + query.lastError().text());
}
void categorizedialog::on_deleteButton_clicked()
{
    emit send_request();
    QString columnName = ui->deleteLineEdit->text().trimmed();
    if (columnName.isEmpty()) {
        QMessageBox::information(this, "Failed", "Column name is empty!");
        return;
    }
    QSqlQuery query(categorizeOp->db);
    QString checkQuery = QString("SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'YourDatabaseName' AND TABLE_NAME = 'domain' AND COLUMN_NAME = '%1'").arg(columnName);
    if (!query.exec(checkQuery)) {
        QMessageBox::warning(this, "Error", "Failed to check the column! Error: " + query.lastError().text());
        return;
    }
    if (columnExists(columnName)) {
        QString alterQuery = QString("ALTER TABLE domain DROP COLUMN %1").arg(columnName);
        if (query.exec(alterQuery))
            QMessageBox::information(this, "Success", "Column deleted successfully!");
        else
            QMessageBox::warning(this, "Error", "Failed! Error: " + query.lastError().text());
    } else
        QMessageBox::information(this, "Failed", "The column does not exist!");
}
void categorizedialog::on_changeButton_clicked()
{
    emit send_request();
    QSqlQuery query(categorizeOp->db);
    QString domain = ui->domainLineEdit->text();
    if (!categorizeOp->domainExists(domain)) {
        QMessageBox::information(this, "Failed", "Domain not found!");
        return;
    }
    QString updateQuery = QString("UPDATE domain SET %1 = :newValue WHERE DomainName = :DomainName").arg(ui->typeLineEdit->text().trimmed());
    query.prepare(updateQuery);
    query.bindValue(":newValue", ui->contentLineEdit->text().trimmed());
    query.bindValue(":DomainName", domain);
    if (query.exec())
        QMessageBox::information(this, "Success", "Domain updated successfully!");
    else
        QMessageBox::warning(this, "Error", "Failed! Error: " + query.lastError().text());
}
