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
    auto start = std::chrono::high_resolution_clock::now();
    emit send_request();
    QString target = ui->searchLineEdit->text();
    if (target.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please input the search target!");
        return;
    }
    if (target == "root") {
        QMessageBox::warning(this, "Warning", "Cannot remove root domain!");
        return;
    }
    bool deleteSuccess = false;
    if (removeOp->hasChildren(target)) {
        QMessageBox::StandardButton confirmDeleteChildren = QMessageBox::question(this, "Confirm Delete", "The domain '" + target + "' has child domains. Do you want to delete this domain and all its children?", QMessageBox::Yes | QMessageBox::No);
        if (confirmDeleteChildren == QMessageBox::Yes)
            deleteSuccess = removeOp->removeWithChildren(target);
        else
            QMessageBox::information(this, "Cancel", "Deletion canceled.");
    } else {
        QMessageBox::StandardButton confirmDelete = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete the domain: " + target, QMessageBox::Yes | QMessageBox::No);
        if (confirmDelete == QMessageBox::Yes)
            deleteSuccess = removeOp->remove(target);
        else
            QMessageBox::information(this, "Cancel", "Deletion canceled.");
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    if (deleteSuccess)
        QMessageBox::information(this, "Success", "Operation successful! Time elapsed: " + QString::number(elapsed.count()) + " ms");
    else
        QMessageBox::warning(this, "Warning", "Operation failed!");
}
