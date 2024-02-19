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
    // emit send_request();
    // domainNode *insertNode = new domainNode;
    // insertNode->domainName     = ui->domainNameLineEdit->text();
    // insertNode->domainNameType = ui->domainTypeLineEdit->text();
    // insertNode->webName        = ui->webNameLineEdit->text();
    // insertNode->sponsorName    = ui->sponsorNameLineEdit->text();
    // insertNode->status         = ui->statusLineEdit->text();
    // insertNode->domainRegister = ui->registerLineEdit->text();
    // insertNode->contactInfo    = ui->contactInfoLineEdit->text();
    // insertNode->memo           = ui->memoLineEdit->text();
    // insertNode->createDate     = ui->createDateLineEdit->text();
    // insertNode->updateDate     = ui->updateDateLineEdit->text();
    // insertNode->expirationDate = ui->expirationDateLineEdit->text();
    // insertNode->creator        = user;
    // insertNode->domainLevel    = insertNode->domainName.count('.') + 1;
    // insertNode->parent = nullptr;
    // insertNode->firstChild = nullptr;
    // insertNode->nextSibling = nullptr;
    // insertNode->id = -1;
    // insertOp->insert(insertNode);
}

