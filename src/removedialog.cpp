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
    if (removeOp->remove(target)){ // Found
        ui->tips->textLabel->setText("Success!");
        ui->tips->animationStart();
    }
    else{ // Not Found
        ui->tips->textLabel->setText("Not Found!");
        ui->tips->animationStart();
    }
}

