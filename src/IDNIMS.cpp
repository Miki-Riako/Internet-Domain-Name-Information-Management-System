#include "IDNIMS.h"
#include "ui_IDNIMS.h"


void fade(auto *control, int duration, int startValue, int endValue)
{ // The function can provide the fade in and fade out animations.
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(control);
    control->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity", control);
    animation->setDuration(duration);
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->start();
}
QString removeFirstSegment(QString& input)
{ // A small function to cut off the part after the first dot
    int dotIndex = input.indexOf('.');
    if (dotIndex != -1)
        return input.mid(dotIndex + 1);
    return input;
}


IDNIMS::IDNIMS(QWidget *parent) : QMainWindow(parent), ui(new Ui::IDNIMS), drawer(new QtMaterialDrawer)
{
    ui->setupUi(this);
    login.show();
    sql.loginConnect();
    /*
    Drawer
    The following contains are about the drawer.
    First it will create a drawer class can then input the button we want into it.
    All the button will be changed in their style sheet.
    */
    drawer->setParent(ui->centralwidget);
    drawer->setClickOutsideToClose(true);
    drawer->setOverlayMode(true);
    QVBoxLayout *drawerLayout = new QVBoxLayout;
    drawer->setDrawerLayout(drawerLayout);
    QVector<QString> labels = {"Home", "Manage", "Searching", "Statistic", "Log", "Setting", "About"};
    int iLabels = 0;
    for (QVector<QString>::iterator it = labels.begin(); it != labels.end(); ++it)
    { // Attend the buttons for the drawer
        drawerButtons[iLabels] = new QPushButton(*it);
        drawerButtons[iLabels]->setStyleSheet("QPushButton {\
                                                border: 0px;\
                                                min-width: 80px;\
                                                font-family: \"Microsoft YaHei UI\";\
                                                font-size: 12pt;\
                                                font-weight: bold;\
                                                color: rgb(0, 0, 0);\
                                            }\
                                            QPushButton:hover {\
                                                border: 0px;\
                                                min-width: 80px;\
                                                font-family: \"Microsoft YaHei UI\";\
                                                font-size: 11pt;\
                                                font-weight: bold;\
                                                color: rgb(50, 50, 50);}");
        drawerButtons[iLabels]->setMinimumHeight(40);
        drawerLayout->addWidget(drawerButtons[iLabels++]);
    }
    drawerLayout->addStretch(3);
    drawer->setContentsMargins(10, 0, 0, 0);
    connect(ui->drawerButton, SIGNAL(pressed()), drawer, SLOT(openDrawer()));
    pagePtr[0] = ui->pageHome;
    pagePtr[1] = ui->pageManage;
    pagePtr[2] = ui->pageSearching;
    pagePtr[3] = ui->pageStatistic;
    pagePtr[4] = ui->pageLog;
    pagePtr[5] = ui->pageSetting;
    pagePtr[6] = ui->pageAbout;
    // Drawer
    
    /*
    Connect
    The following contains are about the buttons connected.
    */
    connect(&login, &Login::guest_enter, this, [&](){
        administratorRights = false;
        enter();
        });
    connect(&login, &Login::enter, this, [&](){
        administratorRights = true;
        user = login.name;
        enter();
        });
    connect(&establishWindow, &establishdialog::send_request, this, [&](){
        establishWindow.user = user;
        establishWindow.establishOp = &sql;
        });
    connect(&categorizeWindow, &categorizedialog::send_request, this, [&](){
        categorizeWindow.user = user;
        categorizeWindow.categorizeOp = &sql;
        });
    connect(&insertWindow, &insertdialog::send_request, this, [&](){
        insertWindow.user = user;
        insertWindow.insertOp = &sql;
        });
    connect(&removeWindow, &removedialog::send_request, this, [&](){removeWindow.removeOp = &sql;});
    connect(&modifyWindow, &modifydialog::send_request, this, [&](){
        modifyWindow.user = user;
        modifyWindow.modifyOp = &sql;
        });
    for (int i = 0; i < 7; i++)
        connect(drawerButtons[i], &QPushButton::clicked, this, [=](){changePage(i);});
    // Connect
}
IDNIMS::~IDNIMS()
{
    delete ui;
}
void IDNIMS::enter(void)
{ // When enter the main window
    login.hide();
    this->show();
    this->showMaximized();
    initialHomePage();
    initialLogPage();
    initialSettingPage();
    if (sql.host[3] == "Yes")
        sql.createDataTree();
    if (!administratorRights)
        user = "Guest";
    ui->userLabel->setText("Welcome! " + user);
    changePage(0);
}
void IDNIMS::changePage(int page)
{ // The small function aims to change the pages
    ui->stackedWidget->setCurrentIndex(page);
    if (inSSL)
        inSSL = false;
    if (page != 2 && page != 3 && page != 4)
        fade(pagePtr[page], 2000, 0, 1);
    else
        inSSL = true;
}
void IDNIMS::keyPressEvent(QKeyEvent *event)
{ // Ctrl+S to save the log
    if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier)
    { // Catch the event
        QString content = ui->textEdit->toPlainText();
        ofstream file(".\\log.idnims");
        if (file.is_open())
        { // Enter the file
            file << content.toStdString();
            file.close();
        }
        else
            QMessageBox::critical(this, "Error", "Could not open the file!");
        if (ui->stackedWidget->currentIndex() == 4)
            QMessageBox::information(this, "Information", "Log saved successfully!");
    }
}
void IDNIMS::initialHomePage(void)
{ // This function aims to set the home page
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    int hour = localTime->tm_hour;
    QString greeting;
    if (hour >= 5 && hour < 12)
        greeting = "Good Morning, ";
    else if (hour >= 12 && hour < 18)
        greeting = "Good Afternoon, ";
    else
        greeting = "Good Evening, ";
    ui->welcomeLabel->setText(greeting + user + "!");
    ui->stackedWidget->setCurrentIndex(0);
}
void IDNIMS::initialLogPage(void)
{ // This function aims to set the log page
    ifstream file(".\\log.idnims");
    if (file.is_open())
    { // Enter the file
        QString content;
        QTextStream stream(&content);
        string line;
        while (getline(file, line))
            stream << QString::fromStdString(line) << endl;
        file.close();
        ui->textEdit->insertPlainText(content);
    }
}
void IDNIMS::initialSettingPage(void)
{ // This function aims to set the setting page
    QAction *eye[3];
    eye[0] = new QAction(this);
    eye[1] = new QAction(this);
    eye[2] = new QAction(this);
    pwdLineEdit[0] = ui->oldPwdLineEdit;
    pwdLineEdit[1] = ui->changePwdLineEdit;
    pwdLineEdit[2] = ui->changeConfirmLineEdit;
    for (int i = 0; i < 3; i++)
    { // Add eyes icon to the password line edits
        eye[i]->setIcon(QIcon(":/images/password-invisible.png"));
        connect(pwdLineEdit[i], &QtMaterialAutoComplete::textEdited, this, [=]{
            if(first[i])
            {
                pwdLineEdit[i]->addAction(eye[i], QLineEdit::TrailingPosition);
                first[i] = false;
            }
        });
        connect(eye[i], &QAction::triggered, this, [=]() mutable{
            if (!state[i])
            {
                eye[i]->setIcon(QIcon(":/images/password-visible.png"));
                pwdLineEdit[i]->addAction(eye[i], QLineEdit::TrailingPosition);
                state[i] = true;
                pwdLineEdit[i]->setEchoMode(QLineEdit::Normal);
            }
            else
            {
                eye[i]->setIcon(QIcon(":/images/password-invisible.png"));
                pwdLineEdit[i]->addAction(eye[i], QLineEdit::TrailingPosition);
                state[i] = false;
                pwdLineEdit[i]->setEchoMode(QLineEdit::Password);
            }
        });
    }
    ui->tipsL->setFixedHeight(20);
    ui->tipsR->setFixedHeight(20);
    if (sql.host[3] == "Yes")
        ui->nodeButton->setChecked(true);
    if (sql.host[4] == "BFS")
        ui->comboBox->setCurrentIndex(1);
    if (sql.host[5] == "On")
        ui->saveButton->setChecked(true);
}
// The button on page 1
void IDNIMS::on_loadAll_clicked()
{
    QSqlQuery query(sql.db);
    query.exec("SELECT * FROM domain");
    int columnCount = query.record().count() - 4;
    int rowCount = 0;
    ui->tableWidget->setColumnCount(columnCount);
    if (query.last())
        rowCount = query.at() + 1;
    ui->tableWidget->setRowCount(rowCount);
    ui->numberLabel->setText(". . . Now the number of domain names: " + QString::number(rowCount));
    QStringList headerLabels;
    for (int i = 4; i < columnCount + 4; i++)
        headerLabels << query.record().fieldName(i);
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    query.exec("SELECT * FROM domain");
    for (int i = 0; query.next(); i++)
        for (int j = 4; j < columnCount; j++)
            ui->tableWidget->setItem(i, j - 4, new QTableWidgetItem(query.value(j).toString()));
}
void IDNIMS::on_clearAll_clicked()
{
    ui->tableWidget->clearContents();
    ui->numberLabel->setText(". . . Now the number of domain names: 0");
}
void IDNIMS::on_uploadAll_clicked()
{ // Upload the domain tree into the database
    if (sql.host[3] == "Yes")
    { // Need to save as node
        QSqlDatabase::database().transaction();
        QSqlQuery query;
        if (sql.host[5] == "On")
        { // If open the backup mode
            query.exec(QString("CREATE TABLE %1 AS SELECT * FROM %2;").arg("domain_temp").arg("domain"));
            query.exec(QString("TRUNCATE TABLE %1;").arg("domain_temp"));
            sql.upload(sql.root);
            query.exec(QString("DROP TABLE %1;").arg("domain"));
        }
        else
        { // No need to backup
            query.exec(QString("ALTER TABLE %1 RENAME TO %2;").arg("domain").arg("domain_temp"));
            query.exec(QString("TRUNCATE TABLE %1;").arg("domain_temp"));
            sql.upload(sql.root);
        }
        query.exec(QString("ALTER TABLE %1 RENAME TO %2;").arg("domain_temp").arg("domain"));
        QSqlDatabase::database().commit();
        QMessageBox::information(this, "Success", "The data has been uploaded successfully.");
    }
    else
        QMessageBox::information(this, "Error", "You have closed store data as the node model.");
}
// The button on page 2
void IDNIMS::on_establishButton_clicked()
{ // Establish a node
    if (administratorRights)
        establishWindow.exec();
    else
        QMessageBox::information(this, "Guest", "You are Guest!");
}
void IDNIMS::on_categorizeButton_clicked()
{ // Categorize a node
    if (administratorRights)
        categorizeWindow.exec();
    else
        QMessageBox::information(this, "Guest", "You are Guest!");
}
void IDNIMS::on_insertButton_clicked()
{ // Insert a node
    if (administratorRights)
        insertWindow.exec();
    else
        QMessageBox::information(this, "Guest", "You are Guest!");
}
void IDNIMS::on_removeButton_clicked()
{ // Remove a node
    if (administratorRights)
        removeWindow.exec();
    else
        QMessageBox::information(this, "Guest", "You are Guest!");
}
void IDNIMS::on_modifyButton_clicked()
{ // Modify a node
    if (administratorRights)
        modifyWindow.exec();
    else
        QMessageBox::information(this, "Guest", "You are Guest!");
}
void IDNIMS::on_domainButton_clicked()
{ // Only shows the level 4 domain name
    QSqlQuery query(sql.db);
    query.exec("SELECT * FROM domain WHERE domain_name_level = 4");
    int columnCount = query.record().count() - 4;
    int rowCount = 0;
    ui->tableWidget->setColumnCount(columnCount);
    if (query.last())
        rowCount = query.at() + 1;
    ui->tableWidget->setRowCount(rowCount);
    ui->numberLabel->setText(". . . Now the number of domain names: " + QString::number(rowCount));
    QStringList headerLabels;
    for (int i = 4; i < columnCount + 4; i++)
        headerLabels << query.record().fieldName(i);
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    query.exec("SELECT * FROM domain WHERE domain_name_level = 4");
    for (int i = 0; query.next(); i++)
        for (int j = 4; j < columnCount; j++)
            ui->tableWidget->setItem(i, j - 4, new QTableWidgetItem(query.value(j).toString()));
}
// The button on page 3
void IDNIMS::on_searchDomain_clicked()
{ // Search the target domain and show it
    QString targetDomain = ui->searchLineEdit->text();
    QString targetType = ui->typeLineEdit->text();
    QSqlQuery query;
    ui->searchWidget->clearContents();
    ui->searchWidget->setRowCount(0);
    LARGE_INTEGER startTime, endTime, frequency;
    if (sql.host[3] == "Yes")
    { // The case that data stored in the node
        QueryPerformanceCounter(&startTime);
        sql.howToSearch(targetDomain);
        if (!sql.searchVector.empty())
        { // If the search vector is not empty, paint it on the table
            int rowCount = sql.searchVector.size();
            ui->searchWidget->setRowCount(rowCount);
            ui->searchWidget->setColumnCount(13);
            QStringList headerLabels;
            headerLabels << "domain name";
            headerLabels << "domain name type";
            headerLabels << "domain name level";
            headerLabels << "web name";
            headerLabels << "sponsor name";
            headerLabels << "status";
            headerLabels << "register";
            headerLabels << "contact information";
            headerLabels << "creator";
            headerLabels << "create date";
            headerLabels << "memo";
            headerLabels << "updated date";
            headerLabels << "expiration date";
            ui->searchWidget->setHorizontalHeaderLabels(headerLabels);
            for (int i = 0; i < rowCount; i++)
            {
                ui->searchWidget->setItem(i, 0, new QTableWidgetItem(sql.searchVector[i]->domainName));
                ui->searchWidget->setItem(i, 1, new QTableWidgetItem(sql.searchVector[i]->domainNameType));
                ui->searchWidget->setItem(i, 2, new QTableWidgetItem(sql.searchVector[i]->domainLevel));
                ui->searchWidget->setItem(i, 3, new QTableWidgetItem(sql.searchVector[i]->webName));
                ui->searchWidget->setItem(i, 4, new QTableWidgetItem(sql.searchVector[i]->sponsorName));
                ui->searchWidget->setItem(i, 5, new QTableWidgetItem(sql.searchVector[i]->status));
                ui->searchWidget->setItem(i, 6, new QTableWidgetItem(sql.searchVector[i]->domainRegister));
                ui->searchWidget->setItem(i, 7, new QTableWidgetItem(sql.searchVector[i]->contactInfo));
                ui->searchWidget->setItem(i, 8, new QTableWidgetItem(sql.searchVector[i]->creator));
                ui->searchWidget->setItem(i, 9, new QTableWidgetItem(sql.searchVector[i]->createDate));
                ui->searchWidget->setItem(i, 10, new QTableWidgetItem(sql.searchVector[i]->memo));
                ui->searchWidget->setItem(i, 11, new QTableWidgetItem(sql.searchVector[i]->updateDate));
                ui->searchWidget->setItem(i, 12, new QTableWidgetItem(sql.searchVector[i]->expirationDate));
            }
        }
        else
            QMessageBox::information(nullptr, "Search Result", "No results found.");
        QueryPerformanceCounter(&endTime);
        double time = (double)(endTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
        ui->timeLabel->setText("search Time: " + QString::number(time, 'f', 4) + "s, in "+ sql.host[4]);
    }
    else
    { // The case that search in the database directly
        QueryPerformanceCounter(&startTime);
        if (targetType.isEmpty())
            targetType = "domain_name";
        QString queryString = QString("SELECT * FROM domain WHERE %1 LIKE '%%2%'").arg(targetType).arg(targetDomain);
        if (query.exec(queryString))
        { // Updated the search table
            int columnCount = query.record().count() - 4;
            int rowCount = 0;
            while (query.next())
                rowCount++;
            if (rowCount == 0)
                QMessageBox::information(nullptr, "Search Result", "No results found.");
            else
            { // Load in the search table
                QStringList headerLabels;
                ifstream file(".\\table.idnims");
                if (file.is_open())
                {
                    string line;
                    while (getline(file, line))
                        headerLabels << QString::fromStdString(line);
                    ui->searchWidget->setHorizontalHeaderLabels(headerLabels);
                    file.close();
                }
                ui->searchWidget->setColumnCount(columnCount);
                ui->searchWidget->setRowCount(rowCount);
                query.exec(queryString);
                for (int i = 0; query.next(); i++)
                    for (int j = 4; j < columnCount; j++)
                        ui->searchWidget->setItem(i, j - 4, new QTableWidgetItem(query.value(j).toString()));
            }
        }
        else
            QMessageBox::warning(nullptr, "Error", "Load the database failed, please check your database!");
        QueryPerformanceCounter(&endTime);
        double time = (double)(endTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
        ui->timeLabel->setText("search Time: " + QString::number(time, 'f', 4) + "s, in MySQL");
    }
}
// The button on page 4
void IDNIMS::on_calculateKey_clicked()
{
    if (staticsAllowed[0])
    {
        staticsAllowed[0] = false;
        QSqlQuery query;
        QVector<QString> statisticName;
        query.exec("SELECT DISTINCT domain_name FROM domain");
        while (query.next())
        { // Get the counts of domain name key types
            QString domainName = query.value("domain_name").toString();
            QString name = removeFirstSegment(domainName);
            statisticName.push_back(name);
        }
        QVector<int> nameCounts(statisticName.size(), 0);
        query.exec("SELECT domain_name FROM domain");
        while (query.next())
        { // Get the counts of every domain name
            QString domainName = query.value("domain_name").toString();
            QString name = removeFirstSegment(domainName);
            int index = statisticName.indexOf(name);
            if (index != -1)
                nameCounts[index]++;
        }
        QChart *chart = new QChart();
        chart->setTitle("Domain Name Distribution");
        QBarSet *set = new QBarSet("Name");
        for (int count : nameCounts)
            *set << count;
        QBarSeries *series = new QBarSeries();
        series->append(set);
        QBarCategoryAxis *axisX = new QBarCategoryAxis;
        axisX->setTitleText("Name");
        axisX->append(statisticName.toList());
        chart->addAxis(axisX, Qt::AlignBottom);
        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Domain Number");
        chart->addAxis(axisY, Qt::AlignLeft);
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
        QChartView *chartView = new QtCharts::QChartView(chart, this);
        QVBoxLayout *layout = new QVBoxLayout(ui->showWidget1);
        layout->addWidget(chartView);
    }
    else
        QMessageBox::information(this, "Sorry", "Already Exits! You can reboot to load a new chart.");
}
void IDNIMS::on_calculateLevel_clicked()
{
    if (staticsAllowed[1])
    {
        staticsAllowed[1] = false;
        QSqlQuery query;
        int statisticLevel[5] = {0, 0, 0, 0, 0};
        query.exec("SELECT domain_name_level FROM domain");
        while(query.next())
        { // Calculate the data
            int a = query.value("domain_name_level").toInt();
            statisticLevel[a]++;
        }
        QChart *chart = new QChart();
        chart->setTitle("Domain Name Level Distribution");
        QBarSet *set = new QBarSet("Level");
        for (int i = 0; i < 5; i++)
            *set << statisticLevel[i];
        QStringList categories;
        categories << "Level 0" << "Level 1" << "Level 2" << "Level 3" << "Level 4";
        QBarSeries *series = new QBarSeries();
        series->append(set);
        QBarCategoryAxis *axisX = new QBarCategoryAxis;
        axisX->setTitleText("Level");
        axisX->append(categories);
        chart->addAxis(axisX,Qt::AlignBottom);
        QValueAxis *axisY=new QValueAxis();
        axisY->setTitleText("Domain Number");
        chart->addAxis(axisY, Qt::AlignLeft);
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
        QChartView *chartView = new QtCharts::QChartView(chart, this);
        QVBoxLayout *layout = new QVBoxLayout(ui->showWidget2);
        layout->addWidget(chartView);
    }
    else
        QMessageBox::information(this, "Sorry", "Already Exits! You can reboot to load a new chart.");
}
void IDNIMS::on_calculateType_clicked()
{
    if (staticsAllowed[2])
    {
        staticsAllowed[2] = false;
        QSqlQuery query;
        QVector<QString> statisticType;
        query.exec("SELECT DISTINCT domain_name_type FROM domain");
        while(query.next())
        { // Calculate the data
            QString type = query.value("domain_name_type").toString();
            statisticType.push_back(type);
        }
        QVector<int> typeCounts(statisticType.size(), 0);
        query.exec("SELECT domain_name_type FROM domain");
        while (query.next())
        { // Get the number of the different types
            QString type = query.value("domain_name_type").toString();
            int index = statisticType.indexOf(type);
            if (index != -1)
                typeCounts[index]++;
        }
        QChart *chart = new QChart();
        chart->setTitle("Domain Name Type Distribution");
        QBarSet *set = new QBarSet("Type");
        for (int count : typeCounts)
            *set << count;
        QBarSeries *series = new QBarSeries();
        series->append(set);
        QBarCategoryAxis *axisX = new QBarCategoryAxis;
        axisX->setTitleText("Type");
        axisX->append(statisticType.toList());
        chart->addAxis(axisX,Qt::AlignBottom);
        QValueAxis *axisY=new QValueAxis();
        axisY->setTitleText("Domain Number");
        chart->addAxis(axisY, Qt::AlignLeft);
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
        QChartView *chartView = new QtCharts::QChartView(chart, this);
        QVBoxLayout *layout = new QVBoxLayout(ui->showWidget3);
        layout->addWidget(chartView);
    }
    else
        QMessageBox::information(this, "Sorry", "Already Exits! You can reboot to load a new chart.");
}
// The button on page 6
void IDNIMS::on_loadHost_clicked()
{ // Get the string on the three line edit and load into the local
    QString dbName = ui->sqlDbNameLineEdit->text();
    QString userName = ui->sqlUsernameLineEdit->text();
    QString pwd = ui->sqlPwdLineEdit->text();
    ofstream file(".\\config.idnims");
    if (file.is_open())
    { // Enter the file
        file << dbName.toStdString() << endl;
        file << userName.toStdString() << endl;
        file << pwd.toStdString() << endl;
        file << "No" << endl;
        file << "BFS" << endl;
        file << "On" << endl;
        file.close();
    }
    else
    { // Can not open the file
        ui->tipsL->textLabel->setText("Could not open the file!");
        ui->tipsL->animationStart();
    }
    sql.connectDataBase();
}
void IDNIMS::on_loadChange_clicked()
{ // Allows user change their password
    if (administratorRights)
    {
        QString oldPassword = ui->oldPwdLineEdit->text();
        QString password = ui->changePwdLineEdit->text();
        QString passwordConfirm = ui->changeConfirmLineEdit->text();
        if (oldPassword.isEmpty() || password.isEmpty() || passwordConfirm.isEmpty())
        { // The case when all are empty
            ui->tipsR->textLabel->setText("Please enter the pwd!");
            ui->tipsR->animationStart();
        }
        else if (password != passwordConfirm)
        { // The case when the both password are not the same
            ui->tipsR->textLabel->setText("The password is not the same!");
            ui->tipsR->animationStart();
        }
        else
        { // Successfully register
            QSqlQuery query;
            QString qs = QString("select * from user where user_name = '%1' and password='%2'").arg(user).arg(oldPassword);
            if (!query.exec(qs))
            {
                ui->tipsR->textLabel->setText("Login Failed! Check your password.");
                ui->tipsR->animationStart();
                return;
            }
            else
            {
                QString updateQuery = QString("UPDATE user SET password = '%1' WHERE user_name = '%2'").arg(password).arg(user);
                if (query.exec(updateQuery))
                {
                    ui->tipsR->textLabel->setText("Password updated successfully!");
                    ui->tipsR->animationStart();
                }
                else
                {
                    ui->tipsR->textLabel->setText("Failed to update password. Check your database connection.");
                    ui->tipsR->animationStart();
                }
            }
        }
    }
    else
        QMessageBox::information(this, "Guest", "You are Guest!");
}
void IDNIMS::on_nodeButton_clicked(bool checked)
{
    if (checked)
    { // Make the No to Yes
        ofstream file(".\\config.idnims");
        if (file.is_open())
        { // Enter the file
            file << sql.host[0].toStdString() << endl;
            file << sql.host[1].toStdString() << endl;
            file << sql.host[2].toStdString() << endl;
            file << "Yes" << endl;
            file << sql.host[4].toStdString() << endl;
            file << sql.host[5].toStdString() << endl;
            file.close();
        }
        else
            QMessageBox::information(this, "Error", "You may miss some important file, try to download again!");
    }
    else
    { // Make the Yes to No
        ofstream file(".\\config.idnims");
        if (file.is_open())
        { // Enter the file
            file << sql.host[0].toStdString() << endl;
            file << sql.host[1].toStdString() << endl;
            file << sql.host[2].toStdString() << endl;
            file << "No" << endl;
            file << sql.host[4].toStdString() << endl;
            file << sql.host[5].toStdString() << endl;
            file.close();
        }
        else
            QMessageBox::information(this, "Error", "You may miss some important file, try to download again!");
    }
}
void IDNIMS::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    { // Check the index of the combo box
        case 0:
        {
            ofstream file(".\\config.idnims");
            if (file.is_open())
            {
                file << sql.host[0].toStdString() << endl;
                file << sql.host[1].toStdString() << endl;
                file << sql.host[2].toStdString() << endl;
                file << sql.host[3].toStdString() << endl;
                file << "DFS" << endl;
                file.close();
            }
            else
                QMessageBox::information(this, "Error", "You may miss some important file, try to download again!");
            break;
        }
        case 1:
        {
            ofstream file(".\\config.idnims");
            if (file.is_open())
            {
                file << sql.host[0].toStdString() << endl;
                file << sql.host[1].toStdString() << endl;
                file << sql.host[2].toStdString() << endl;
                file << sql.host[3].toStdString() << endl;
                file << "BFS" << endl;
                file.close();
            }
            else
                QMessageBox::information(this, "Error", "You may miss some important file, try to download again!");
            break;
        }
        default:
            break;
    }
}
void IDNIMS::on_saveButton_clicked(bool checked)
{
    if (checked)
    { // Make the Off to On
        ofstream file(".\\config.idnims");
        if (file.is_open())
        { // Enter the file
            file << sql.host[0].toStdString() << endl;
            file << sql.host[1].toStdString() << endl;
            file << sql.host[2].toStdString() << endl;
            file << sql.host[3].toStdString() << endl;
            file << sql.host[4].toStdString() << endl;
            file << "On" << endl;
            file.close();
        }
        else
            QMessageBox::information(this, "Error", "You may miss some important file, try to download again!");
    }
    else
    { // Make the On to Off
        ofstream file(".\\config.idnims");
        if (file.is_open())
        { // Enter the file
            file << sql.host[0].toStdString() << endl;
            file << sql.host[1].toStdString() << endl;
            file << sql.host[2].toStdString() << endl;
            file << sql.host[3].toStdString() << endl;
            file << sql.host[4].toStdString() << endl;
            file << "Off" << endl;
            file.close();
        }
        else
            QMessageBox::information(this, "Error", "You may miss some important file, try to download again!");
    }
}
