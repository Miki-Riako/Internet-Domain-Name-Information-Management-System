#include "removedialog.h"
#include "ui_removedialog.h"

removedialog::removedialog(QWidget *parent) : QDialog(parent), ui(new Ui::removedialog)
{
    ui->setupUi(this);
}

removedialog::~removedialog()
{
    delete ui;
}

void removedialog::on_searchDomain_clicked()
{
    emit send_request();
    QString target = ui->searchLineEdit->text();
    if (target.isEmpty()) { // No text
        QMessageBox::warning(this, "Warning", "Please input the search target!");
        return;
    }
    if (target == "root") { // Root
        QMessageBox::warning(this, "Warning", "Cannot remove root domain!");
        return;
    }
    if (removeOp->hasChildren(target)) {
        QMessageBox::StandardButton confirmDeleteChildren = QMessageBox::question(this, "Confirm Delete", "The domain '" + target + "' has child domains. Do you want to delete this domain and all its children?", QMessageBox::Yes | QMessageBox::No);
        if (confirmDeleteChildren == QMessageBox::Yes) {
            if (removeOp->removeWithChildren(target))
                QMessageBox::information(this, "Success", "Successfully deleted the domain '" + target + "' and all its children!");
            else
                QMessageBox::warning(this, "Warning", "Failed to delete the domain '" + target + "' and all its children!");
        } else
            QMessageBox::information(this, "Cancel", "Deletion canceled.");
    } else {
        QMessageBox::StandardButton confirmDelete = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete the domain: " + target, QMessageBox::Yes | QMessageBox::No);
        if (confirmDelete == QMessageBox::Yes) {
            if (removeOp->remove(target))
                QMessageBox::information(this, "Success", "Successfully deleted the domain '" + target + "'!");
            else
                QMessageBox::warning(this, "Warning", "Failed to delete the domain '" + target + "'!");
        } else
            QMessageBox::information(this, "Cancel", "Deletion canceled.");
    }
}

