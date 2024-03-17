#include "modifydialog.h"
#include "ui_modifydialog.h"

modifydialog::modifydialog(QWidget *parent) : QDialog(parent), ui(new Ui::modifydialog)
{
    ui->setupUi(this);
}

modifydialog::~modifydialog()
{
    delete ui;
}

void modifydialog::on_confirmButton_clicked()
{
    emit send_request();
    auto start = std::chrono::high_resolution_clock::now();
    QString target = ui->domainNameLineEdit->text();
    if (target.isEmpty()){ // No text
        QMessageBox::information(this, "Failed", "Please input the remove target!");
        return;
    }
    if (target == "root"){ // Root
        QMessageBox::information(this, "Failed", "Cannot modify root domain!");
        return;
    }
    if (!modifyOp->domainExists(target)) { // Not Found
        QMessageBox::information(this, "Failed", "Not Found!");
        return;
    }
    QSqlQuery query(modifyOp->db);
    query.prepare("UPDATE domain SET DomainType = :DomainType, WebName = :WebName, SponsorName = :SponsorName, Status = :Status, Register = :Register, ContactInformation = :ContactInformation, Creator = :Creator, Memo = :Memo, CreateDate = :CreateDate, UpdatedDate = :UpdatedDate, ExpiredDate = :ExpiredDate WHERE DomainName = :DomainName");
    query.bindValue(":DomainName", target);
    query.bindValue(":DomainType", ui->domainTypeLineEdit->text().trimmed());
    query.bindValue(":WebName", ui->webNameLineEdit->text().trimmed());
    query.bindValue(":SponsorName", ui->sponsorNameLineEdit->text().trimmed());
    query.bindValue(":Status", ui->statusLineEdit->text().trimmed());
    query.bindValue(":Register", ui->registerLineEdit->text().trimmed());
    query.bindValue(":ContactInformation", ui->contactInfoLineEdit->text().trimmed());
    query.bindValue(":Creator", user);
    query.bindValue(":Memo", ui->memoLineEdit->text().trimmed());
    query.bindValue(":CreateDate", ui->createDateLineEdit->text().trimmed());
    query.bindValue(":UpdatedDate", ui->updateDateLineEdit->text().trimmed());
    query.bindValue(":ExpiredDate", ui->expirationDateLineEdit->text().trimmed());
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    QString timeMessage = "Time elapsed: " + QString::number(elapsed.count()) + " ms";
    if (query.exec())
        QMessageBox::information(this, "Success", "Modify Successfully! " + timeMessage);
    else
        QMessageBox::critical(this, "Error", "Failed to modify: " + query.lastError().text() + ". " + timeMessage);
}
