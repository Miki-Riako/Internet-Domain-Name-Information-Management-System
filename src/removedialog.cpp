#include "removedialog.h"
#include "ui_removedialog.h"

removedialog::removedialog(QWidget *parent) : QDialog(parent), ui(new Ui::removedialog)
{
    ui->setupUi(this);
    ui->tips->setFixedHeight(20);
}

removedialog::~removedialog()
{
    delete ui;
}

void removedialog::on_searchDomain_clicked()
{
    emit send_request();
    QString target = ui->searchLineEdit->text();
    if (target.isEmpty()){ // No text
        ui->tips->textLabel->setText("Please input the remove target!");
        ui->tips->animationStart();
        return;
    }
    if (target == "root"){ // Root
        ui->tips->textLabel->setText("Cannot remove root domain!");
        ui->tips->animationStart();
        return;
    }
    if (!removeOp->domainExists(target)) { // Not Found
        ui->tips->textLabel->setText("Not Found!");
        ui->tips->animationStart();
        return;
    }
    if (removeOp->hasChildren(target)) {
        QMessageBox::StandardButton confirmDeleteChildren = QMessageBox::question(this, "Confirm Delete", "The domain '" + target + "' has child domains. Do you want to delete this domain and all its children?", QMessageBox::Yes | QMessageBox::No);
        if (confirmDeleteChildren == QMessageBox::Yes) {
            if (removeOp->removeWithChildren(target))
                ui->tips->textLabel->setText("Success!");
            else
                ui->tips->textLabel->setText("Failed!");
        } else
            ui->tips->textLabel->setText("Deletion canceled.");
    } else {
        QMessageBox::StandardButton confirmDelete = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete the domain: " + target, QMessageBox::Yes | QMessageBox::No);
        if (confirmDelete == QMessageBox::Yes) {
            if (removeOp->remove(target))
                ui->tips->textLabel->setText("Success!");
            else
                ui->tips->textLabel->setText("Failed!");
        } else
            ui->tips->textLabel->setText("Deletion canceled.");
    }
    ui->tips->animationStart();
}

