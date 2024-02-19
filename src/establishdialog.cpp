#include "establishdialog.h"
#include "ui_establishdialog.h"
QString randomString(int length)
{ // Give back a random string
    string charset = "abcdefghijklmnopqrstuvwxyz";
    mt19937 rng(static_cast<unsigned>(time(0)));
    int charsetLength = charset.length();
    string randomString;
    for (int i = 0; i < length; ++i)
    { // Give the random value
        uniform_int_distribution<> distribution(0, charsetLength - 1);
        int index = distribution(rng);
        randomString += charset[index];
    }
    return QString::fromStdString(randomString);
}
establishdialog::establishdialog(QWidget *parent) : QDialog(parent), ui(new Ui::establishdialog)
{
    ui->setupUi(this);
    ui->tips->setFixedHeight(20);
}
establishdialog::~establishdialog()
{
    delete ui;
}


void establishdialog::establish(QString domain)
{ // Give a whole domain and create it
    domainNode *establishNode = new domainNode;
    establishNode->domainName = domain;
    establishNode->parent = nullptr;
    establishNode->firstChild = nullptr;
    establishNode->nextSibling = nullptr;
    establishNode->creator = user;
    establishNode->id = -1;
    establishNode->domainLevel = establishNode->domainName.count('.') + 1;
    if (establishNode->domainLevel > 1) { // At least have father
        domainNode *fatherNode = new domainNode;
        fatherNode->domainName = establishOp->removeFirstSegment(establishNode->domainName);
        fatherNode->parent = nullptr;
        fatherNode->firstChild = nullptr;
        fatherNode->nextSibling = nullptr;
        fatherNode->creator = user;
        fatherNode->id = -1;
        fatherNode->domainLevel = establishNode->domainLevel - 1;
        if (establishNode->domainLevel > 2) { // When level is 2
            domainNode *grandFatherNode = new domainNode;
            grandFatherNode->domainName = establishOp->removeFirstSegment(fatherNode->domainName);
            grandFatherNode->creator = user;
            grandFatherNode->parent = nullptr;
            grandFatherNode->firstChild = nullptr;
            grandFatherNode->nextSibling = nullptr;
            grandFatherNode->id = -1;
            grandFatherNode->domainLevel = fatherNode->domainLevel - 1;
            if (establishNode->domainLevel > 3) { // When level is 1
                domainNode *greatGrandFatherNode = new domainNode;
                greatGrandFatherNode->domainName = establishOp->removeFirstSegment(grandFatherNode->domainName);
                greatGrandFatherNode->creator = user;
                greatGrandFatherNode->parent = nullptr;
                greatGrandFatherNode->firstChild = nullptr;
                greatGrandFatherNode->nextSibling = nullptr;
                greatGrandFatherNode->id = -1;
                greatGrandFatherNode->domainLevel = grandFatherNode->domainLevel - 1;
                establishOp->insert(greatGrandFatherNode);
            }
            establishOp->insert(grandFatherNode);
        }
        establishOp->insert(fatherNode);
    }
    establishOp->insert(establishNode);
}


void establishdialog::on_randomButton_clicked()
{
    emit send_request();
    int length = ui->lengthBox->value();
    switch (ui->spinBox->value()) { // Create the domain name with the level
    case 4:
        establish(randomString(length) + "." + randomString(length) + "." + randomString(length) + "." + randomString(length));
        break;
    case 3:
        establish(randomString(length) + "." + randomString(length) + "." + randomString(length));
        break;
    case 2:
        establish(randomString(length) + "." + randomString(length));
        break;
    default:
        establish(randomString(length));
        break;
    }
}
void establishdialog::on_fileButton_clicked()
{
    emit send_request();
    ifstream file(".\\domain.txt");
    if (file.is_open()) { // Enter the file
        string line;
        while (getline(file, line))
            establish(QString::fromStdString(line));
        ui->tips->textLabel->setText("Success!");
        ui->tips->animationStart();
        file.close();
    }
    else { // Enter failed
        ui->tips->textLabel->setText("The txt is missing!");
        ui->tips->animationStart();
        return;
    }
}
void establishdialog::on_peopleButton_clicked()
{
    emit send_request();
    establish(ui->searchLineEdit->text());
}

