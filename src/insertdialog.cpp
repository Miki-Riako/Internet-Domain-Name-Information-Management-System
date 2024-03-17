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
    auto start = std::chrono::high_resolution_clock::now();
    QString domainName = ui->domainNameLineEdit->text();
    QString fatherDomain = insertOp->removeFirstSegment(domainName);
    if (fatherDomain.isEmpty() || !insertOp->domainExists(fatherDomain)) {
        QMessageBox::warning(this, "Error", "Father domain does not exist!");
        return;
    }
    bool exit = insertOp->insert(domainName, domainName.count('.') + 1, user);
    QSqlQuery query(insertOp->db);
    query.prepare("UPDATE domain SET DomainType = :DomainType, WebName = :WebName, SponsorName = :SponsorName, Status = :Status, Register = :Register, ContactInformation = :ContactInformation, Memo = :Memo, UpdatedDate = :UpdatedDate, ExpiredDate = :ExpiredDate WHERE DomainName = :DomainName");
    query.bindValue(":DomainName", domainName);
    query.bindValue(":DomainType", ui->domainTypeLineEdit->text().trimmed());
    query.bindValue(":WebName", ui->webNameLineEdit->text().trimmed());
    query.bindValue(":SponsorName", ui->sponsorNameLineEdit->text().trimmed());
    query.bindValue(":Status", ui->statusLineEdit->text().trimmed());
    query.bindValue(":Register", ui->registerLineEdit->text().trimmed());
    query.bindValue(":ContactInformation", ui->contactInfoLineEdit->text().trimmed());
    query.bindValue(":Memo", ui->memoLineEdit->text().trimmed());
    query.bindValue(":UpdatedDate", ui->updateDateLineEdit->text().trimmed());
    query.bindValue(":ExpiredDate", ui->expirationDateLineEdit->text().trimmed());
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    QString timeMessage = "Time elapsed: " + QString::number(elapsed.count()) + " ms";
    if (query.numRowsAffected() > 0) {
        if (exit)
            QMessageBox::information(this, "Success", "Domain and its information inserted successfully! " + timeMessage);
        else
            QMessageBox::information(this, "Failed", "Can not insert the domain. " + timeMessage);
    }
    else
        QMessageBox::warning(this, "Warning", "No information was updated. " + timeMessage);
}

