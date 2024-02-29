#include "establishdialog.h"
#include "ui_establishdialog.h"
establishdialog::establishdialog(QWidget *parent) : QDialog(parent), ui(new Ui::establishdialog)
{
    ui->setupUi(this);
    ui->tips->setFixedHeight(20);
}
establishdialog::~establishdialog()
{
    delete ui;
}

QString establishdialog::randomString(int length)
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
void establishdialog::establish(QString domain)
{ // Give a whole domain and create it
    if (domain.isEmpty())
        return;
    int level = establishOp->getLevel(domain);
    QStack<QString> domainStack;
    for (int i = 0; i < level; ++i) {
        domainStack.push(domain);
        domain = establishOp->removeFirstSegment(domain);
    }
    QSqlQuery query(establishOp->db);
    bool isSuccess = true;
    for (int level = 1; !domainStack.isEmpty(); ++level) {
        QString currentDomain = domainStack.pop();
        query.prepare("SELECT COUNT(*) FROM domain WHERE DomainName = :DomainName");
        query.bindValue(":DomainName", currentDomain);
        if (!query.exec()) {
            QMessageBox::critical(this, "Error", "Failed to establish domain!");
            return;
        }
        if (query.next() && query.value(0).toInt() == 0)
            isSuccess = isSuccess && establishOp->insert(currentDomain, level, user);
    }
    if (isSuccess) {
        ui->tips->textLabel->setText("Domain established!");
        ui->tips->animationStart();
    }
    else {
        ui->tips->textLabel->setText("Failed to establish domain!");
        ui->tips->animationStart();
    }
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open the file!");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        establish(line);
    }
    ui->tips->textLabel->setText("Success!");
    ui->tips->animationStart();
    file.close();
}
void establishdialog::on_peopleButton_clicked()
{
    emit send_request();
    establish(ui->searchLineEdit->text());
}
