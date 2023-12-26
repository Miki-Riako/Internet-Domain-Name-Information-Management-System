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

void categorizedialog::on_addButton_clicked()
{
    emit send_request();
    QSqlQuery query;
    QString alterQuery = QString("ALTER TABLE domain ADD COLUMN %1 VARCHAR(255)").arg(ui->addLineEdit->text());
    if (query.exec(alterQuery))
        ui->tips->textLabel->setText("Success!");
    else
        ui->tips->textLabel->setText("Failed!");
    ui->tips->animationStart();
}
void categorizedialog::on_deleteButton_clicked()
{
    emit send_request();
    QSqlQuery query;
    QString alterQuery = QString("ALTER TABLE domain DROP COLUMN %1").arg(ui->deleteLineEdit->text());
    if (query.exec(alterQuery))
        ui->tips->textLabel->setText("Success!");
    else
        ui->tips->textLabel->setText("Failed!");
    ui->tips->animationStart();
}
void categorizedialog::on_changeButton_clicked()
{
    emit send_request();
        QSqlQuery query;
    QString updateQuery = QString("UPDATE domain SET %1 = :newValue WHERE domain_name = :domainName").arg(ui->typeLineEdit->text());
    query.prepare(updateQuery);
    query.bindValue(":newValue", ui->contentLineEdit->text());
    query.bindValue(":domainName", ui->domainLineEdit->text());
    QSqlQuery newQuery;
    newQuery.exec(QString("CREATE TABLE %1 AS SELECT * FROM %2;").arg("domain_type").arg("domain"));
    if (query.exec())
        ui->tips->textLabel->setText("Success!");
    else
        ui->tips->textLabel->setText("Failed!");
    ui->tips->animationStart();
}

