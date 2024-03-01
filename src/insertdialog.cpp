#include "insertdialog.h"
#include "ui_insertdialog.h"

insertdialog::insertdialog(QWidget *parent) : QDialog(parent), ui(new Ui::insertdialog)
{
    ui->setupUi(this);
}

insertdialog::~insertdialog()
{
    delete ui;
}

void insertdialog::on_confirmButton_clicked()
{
    emit send_request();
    QString domainName = ui->domainNameLineEdit->text();
    QString fatherDomain = insertOp->removeFirstSegment(domainName);
    if (fatherDomain.isEmpty() || !insertOp->domainExists(fatherDomain)) {
        QMessageBox::warning(this, "Error", "Father domain does not exist!");
        return;
    }
    insertOp->insert(domainName, domainName.count('.') + 1, user);
    QSqlQuery query(insertOp->db);
    query.prepare("UPDATE domain SET DomainNameType = :DomainNameType, WebName = :WebName, SponsorName = :SponsorName, Status = :Status, DomainRegister = :DomainRegister, ContactInfo = :ContactInfo, Memo = :Memo, UpdateDate = :UpdateDate, ExpirationDate = :ExpirationDate WHERE DomainName = :DomainName");
    query.bindValue(":DomainName", domainName);
    query.bindValue(":DomainNameType", ui->domainTypeLineEdit->text().trimmed());
    query.bindValue(":WebName", ui->webNameLineEdit->text().trimmed());
    query.bindValue(":SponsorName", ui->sponsorNameLineEdit->text().trimmed());
    query.bindValue(":Status", ui->statusLineEdit->text().trimmed());
    query.bindValue(":DomainRegister", ui->registerLineEdit->text().trimmed());
    query.bindValue(":ContactInfo", ui->contactInfoLineEdit->text().trimmed());
    query.bindValue(":Memo", ui->memoLineEdit->text().trimmed());
    query.bindValue(":UpdateDate", ui->updateDateLineEdit->text().trimmed());
    query.bindValue(":ExpirationDate", ui->expirationDateLineEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    if (query.numRowsAffected() > 0)
        QMessageBox::information(this, "Success", "Domain and its information inserted successfully!");
    else
        QMessageBox::warning(this, "Warning", "No information was updated.");
}

