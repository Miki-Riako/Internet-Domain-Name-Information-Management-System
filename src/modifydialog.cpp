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
    // emit send_request();
    // QString target = ui->domainNameLineEdit->text();
    // modifyOp->howToSearch(target);
    // if (modifyOp->searchVector.size() == 0) { // The input is wrong
    //     QMessageBox::information(nullptr, "Error", "The input is wrong!");
    //     return;
    // }
    // for (auto i = 0; i < modifyOp->searchVector.size(); i++)
    //     if (modifyOp->searchVector[i]->domainName == target) { // Find the target
    //         domainNode *modifyNode = modifyOp->searchVector[i];
    //         modifyNode->domainNameType = ui->domainTypeLineEdit->text();
    //         modifyNode->webName        = ui->webNameLineEdit->text();
    //         modifyNode->sponsorName    = ui->sponsorNameLineEdit->text();
    //         modifyNode->status         = ui->statusLineEdit->text();
    //         modifyNode->domainRegister = ui->registerLineEdit->text();
    //         modifyNode->contactInfo    = ui->contactInfoLineEdit->text();
    //         modifyNode->memo           = ui->memoLineEdit->text();
    //         modifyNode->createDate     = ui->createDateLineEdit->text();
    //         modifyNode->updateDate     = ui->updateDateLineEdit->text();
    //         modifyNode->expirationDate = ui->expirationDateLineEdit->text();
    //         modifyNode->creator        = user;
    //         QMessageBox::information(this, "Success", "Modify Successfully!");
    //     }
}
