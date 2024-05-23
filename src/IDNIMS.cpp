#include "IDNIMS.h"
#include "ui_IDNIMS.h"

IDNIMS::IDNIMS(QWidget *parent) : QMainWindow(parent), ui(new Ui::IDNIMS), drawer(new QtMaterialDrawer)
{
    messageHandler = new GuestMessageHandler();

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
    for (auto it = labels.begin(); it != labels.end(); ++it) { // Attend the buttons for the drawer
        drawerButtons[iLabels] = new QPushButton(*it);
        drawerButtons[iLabels]->setStyleSheet("\
            QPushButton {\
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
                color: rgb(50, 50, 50);\
        }");
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
    for (int i = 0; i < 7; i++) {
        connect(drawerButtons[i], &QPushButton::clicked, this, [=](){changePage(i);});
    }
    // Connect
    /*
    File
    The following contains are about the local files.
    */
    QFile configFile("config.ini");
    if (!configFile.exists())
        if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&configFile);
            out << "[Database]\n";
            out << "Name=\n";
            out << "User=\n";
            out << "Password=\n";
            out << "HostName=127.0.0.1\n";
            out << "Port=3306\n\n";
            out << "[Settings]\n";
            out << "DataBackup=true\n";
            out << "HomePage=0\n";
            configFile.close();
        }
    QFile logFile("log.idnims");
    if (!logFile.exists())
        if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&logFile);
            out << "This is the IDNIMS Log\n";
            out << "You can record what you want to write here.\n";
            out << "Press ctrl+s to save it.\n";
            out << "Each time you open the log, it will remind you of what you need to do.\n";
            out << "Enjoy!\n";
            logFile.close();
        }
    // File
}
IDNIMS::~IDNIMS() {delete ui;}
void IDNIMS::clearLayout(QLayout *layout)
{ // Clear the layout
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    delete messageHandler;
}
void IDNIMS::fade(auto *control, const int &duration, const int &startValue, const int &endValue)
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
bool IDNIMS::checkPasswordStrength(const QString &password)
{ // Check whether powerful
    QRegExp passwordExpr("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^\\da-zA-Z]).{8,}$");
    return passwordExpr.exactMatch(password);
}
void IDNIMS::enter(void)
{ // When enter the main window
    login.hide();
    this->show();
    this->showMaximized();
    initialHomePage();
    initialSearchPage();
    initialLogPage();
    initialSettingPage();
    if (!administratorRights)
        user = "Guest";
    ui->userLabel->setText("Welcome! " + user);
    QSettings settings("config.ini", QSettings::IniFormat);
    changePage(settings.value("Settings/HomePage").toInt());
    fade(ui->nameSortButton, 2000, 0, 1);
    fade(ui->typeSortButton, 2000, 0, 1);
    fade(ui->levelSortButton, 2000, 0, 1);
    fade(ui->sortRadio, 2000, 0, 1);
    messageHandler->showMessage();
}
void IDNIMS::changePage(const int &page)
{ // The small function aims to change the pages
    ui->stackedWidget->setCurrentIndex(page);
    if (page != 2 && page != 3 && page != 4)
        fade(pagePtr[page], 2000, 0, 1);
}
void IDNIMS::keyPressEvent(QKeyEvent *event)
{ // Ctrl+S to save the log
    if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier) { // Catch the event
        QString content = ui->textEdit->toPlainText();
        ofstream file(".\\log.idnims");
        if (file.is_open()) { // Enter the file
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
    QSettings settings("config.ini", QSettings::IniFormat);
    ui->stackedWidget->setCurrentIndex(settings.value("Settings/HomePage").toInt());
}
void IDNIMS::initialSearchPage(void)
{ // This function aims to set the searching page
    ui->searchRadio->setChecked(true);
}
void IDNIMS::initialLogPage(void)
{ // This function aims to set the log page
    ifstream file(".\\log.idnims");
    if (file.is_open()) { // Enter the file
        QString content;
        QTextStream stream(&content);
        string line;
        while (getline(file, line))
            stream << QString::fromStdString(line) << endl;
        file.close();
        ui->textEdit->insertPlainText(content);
    }
    else
        QMessageBox::information(this, "Error", "You may miss some important file, try to download again!");
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
    for (int i = 0; i < 3; i++) { // Add eyes icon to the password line edits
        eye[i]->setIcon(QIcon(":/images/password-invisible.png"));
        connect(pwdLineEdit[i], &QtMaterialAutoComplete::textEdited, this, [=]{
            if(first[i]) {
                pwdLineEdit[i]->addAction(eye[i], QLineEdit::TrailingPosition);
                first[i] = false;
            }
        });
        connect(eye[i], &QAction::triggered, this, [=]() mutable{
            if (!state[i]) {
                eye[i]->setIcon(QIcon(":/images/password-visible.png"));
                pwdLineEdit[i]->addAction(eye[i], QLineEdit::TrailingPosition);
                state[i] = true;
                pwdLineEdit[i]->setEchoMode(QLineEdit::Normal);
            }
            else {
                eye[i]->setIcon(QIcon(":/images/password-invisible.png"));
                pwdLineEdit[i]->addAction(eye[i], QLineEdit::TrailingPosition);
                state[i] = false;
                pwdLineEdit[i]->setEchoMode(QLineEdit::Password);
            }
        });
    }
    ui->tipsL->setFixedHeight(20);
    ui->tipsR->setFixedHeight(20);
    QSettings settings("config.ini", QSettings::IniFormat);
    if (settings.value("Settings/DataBackup").toBool())
        ui->saveButton->setChecked(true);
    else
        ui->saveButton->setChecked(false);
}
void IDNIMS::loadDomainData(const int &level)
{ // Show the domain data
    QSqlQuery query(sql.db);
    QString queryString = level == -1 ? "SELECT * FROM domain" : QString("SELECT * FROM domain WHERE DomainLevel = %1").arg(level);
    if (!query.exec(queryString)) {
        QMessageBox::warning(this, "Error!", "Cannot get data from database!");
        return;
    }
    int columnCount = query.record().count();
    int rowCount = 0;
    ui->tableWidget->setColumnCount(columnCount);
    ui->tableWidget->setRowCount(0);
    QStringList headerLabels;
    for (int i = 0; i < columnCount; ++i)
        headerLabels << query.record().fieldName(i);
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    while (query.next()) {
        ui->tableWidget->insertRow(rowCount);
        for (int j = 0; j < columnCount; ++j)
            ui->tableWidget->setItem(rowCount, j, new QTableWidgetItem(query.value(j).toString()));
        ++rowCount;
    }
    ui->numberLabel->setText(". . . Now the number of domain names: " + QString::number(rowCount));
}
void IDNIMS::displaySortedData(const QString &queryString)
{
    QSqlQuery query(sql.db);
    if (!query.exec(queryString)) {
        QMessageBox::warning(this, "Error!", "Cannot get data from database!");
        return;
    }

    int columnCount = query.record().count();
    int rowCount = 0;
    ui->tableWidget->setColumnCount(columnCount);
    ui->tableWidget->setRowCount(0);
    QStringList headerLabels;
    for (int i = 0; i < columnCount; ++i)
        headerLabels << query.record().fieldName(i);
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    while (query.next()) {
        ui->tableWidget->insertRow(rowCount);
        for (int j = 0; j < columnCount; ++j) {
            ui->tableWidget->setItem(rowCount, j, new QTableWidgetItem(query.value(j).toString()));
        }
        ++rowCount;
    }

    ui->numberLabel->setText(". . . Now the number of domain names: " + QString::number(rowCount));
}

// The button on page 1
void IDNIMS::on_loadAll_clicked()
{
    loadDomainData(-1);
}
void IDNIMS::on_clearAll_clicked()
{
    ui->tableWidget->clearContents();
    ui->numberLabel->setText(". . . Now the number of domain names: 0");
}
void IDNIMS::on_deriveAll_clicked()
{
    int level = ui->spinBox->value();
    QString filters = "CSV Files (*.csv);;JSON Files (*.json)";
    QString defaultFilter = "CSV Files (*.csv)";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", filters, &defaultFilter);
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open the file for writing!");
        return;
    }
    QTextStream out(&file);
    QSqlQuery query(sql.db);
    QString queryString = level == -1 ? "SELECT * FROM domain" : QString("SELECT * FROM domain WHERE DomainLevel = %1").arg(level);
    if (!query.exec(queryString)) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        file.close();
        return;
    }
    if (fileName.endsWith(".csv")) {
        QStringList columnTitles = {
            "id",
            "DomainName",
            "DomainType",
            "DomainLevel",
            "WebName", 
            "SponsorName",
            "Status",
            "Register",
            "ContactInformation", 
            "Creator",
            "CreateDate",
            "Memo",
            "UpdatedDate",
            "ExpiredDate"
        };
        out << columnTitles.join(",") << "\n";
        while(query.next()) {
            QStringList row;
            for (int i = 0; i < columnTitles.size(); ++i) {
                row.append("\"" + query.value(i).toString() + "\"");
            }
            out << row.join(",") << "\n";
        }
    } else if (fileName.endsWith(".json")) {
        QJsonArray jsonArray;
        while (query.next()) {
            QJsonObject jsonObj;
            for (int i = 0; i < query.record().count(); ++i) {
                jsonObj.insert(query.record().fieldName(i), QJsonValue::fromVariant(query.value(i)));
            }
            jsonArray.append(jsonObj);
        }
        QJsonDocument jsonDoc(jsonArray);
        out << jsonDoc.toJson();
    }
    file.close();
    QMessageBox::information(this, "Success", "All domains were successfully exported to " + fileName);
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
{ // Only shows the targeted level domain name
    loadDomainData(ui->spinBox->value());
}
// The button on page 3
void IDNIMS::on_searchDomain_clicked()
{ // Search the target domain and show it
    QString targetDomain = ui->searchLineEdit->text().trimmed();
    QString targetType = ui->typeLineEdit->text().trimmed().isEmpty() ? "DomainName" : ui->typeLineEdit->text().trimmed();
    QString lengthFilter = ui->lenLineEdit->text().trimmed();
    QString lengthSql;
    QRegExp regex("^(-?\\d*)-?(\\d*)$");

    if (regex.exactMatch(lengthFilter)) {
        int minLength = regex.cap(1).toInt();
        int maxLength = regex.cap(2).toInt();
        if (minLength > 0 && maxLength > 0)
            lengthSql = QString(" AND LENGTH(%1) BETWEEN %2 AND %3").arg(targetType).arg(minLength).arg(maxLength);
        else if (minLength > 0)
            lengthSql = QString(" AND LENGTH(%1) >= %2").arg(targetType).arg(minLength);
        else if (maxLength > 0)
            lengthSql = QString(" AND LENGTH(%1) <= %2").arg(targetType).arg(maxLength);
    }

    QSqlQuery query(sql.db);
    ui->searchWidget->clearContents();
    ui->searchWidget->setRowCount(0);

    QString queryString;
    if (ui->searchRadio->isChecked()) {
        QString command = "SELECT * FROM domain WHERE " + targetType + " LIKE '%" + targetDomain + "%'" + lengthSql;
        queryString = command;
    }
    else
        queryString = QString("SELECT * FROM domain WHERE %1 LIKE '%2' %3").arg(targetType).arg(targetDomain).arg(lengthSql);
    auto startTime = std::chrono::high_resolution_clock::now();

    if (query.exec(queryString)) {
        int columnCount = query.record().count();
        int rowCount = 0;
        ui->searchWidget->setColumnCount(columnCount);
        QStringList headerLabels;
        for (int i = 0; i < columnCount; ++i) {
            headerLabels << query.record().fieldName(i);
        }
        ui->searchWidget->setHorizontalHeaderLabels(headerLabels);
        while (query.next()) {
            ui->searchWidget->insertRow(rowCount);
            for (int j = 0; j < columnCount; ++j) {
                ui->searchWidget->setItem(rowCount, j, new QTableWidgetItem(query.value(j).toString()));
            }
            ++rowCount;
        }
        if (rowCount == 0)
            QMessageBox::information(nullptr, "Search Result", "No results found.");
    } else
        QMessageBox::warning(nullptr, "Error", "Load the database failed, please check your database!");

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    ui->timeLabel->setText("Search Time: " + QString::number(duration / 1000, 'f', 4) + "s");
}
// The button on page 4
void IDNIMS::on_calculateName_clicked()
{
    ui->statisticWidget->setCurrentIndex(0);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->showWidgetName->layout());
    if (layout != nullptr)
        clearLayout(layout);
    else
        layout = new QVBoxLayout(ui->showWidgetName);
    QSqlQuery query;
    if (!query.exec("SELECT DomainName FROM domain")) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMap<QString, int> unitStatistics;
    while (query.next()) {
        QString domainName = query.value(0).toString();
        QStringList units = domainName.split('.');
        for (const QString &unit : units)
            unitStatistics[unit]++;
    }
    QChart *chart = new QChart();
    chart->setTitle("Domain Unit Distribution");
    QBarSet *set = new QBarSet("Count");
    QBarSeries *series = new QBarSeries();
    QStringList categories;
    for (auto it = unitStatistics.begin(); it != unitStatistics.end(); ++it) {
        categories << it.key();
        *set << it.value();
    }
    series->append(set);
    chart->addSeries(series);
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setInteractive(true);
    layout->addWidget(chartView);
}
void IDNIMS::on_calculateType_clicked()
{
    ui->statisticWidget->setCurrentIndex(1);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->showWidgetType->layout());
    if (layout != nullptr)
        clearLayout(layout);
    else
        layout = new QVBoxLayout(ui->showWidgetType);
    QSqlQuery query;
    if (!query.exec("SELECT DomainType, COUNT(*) FROM domain GROUP BY DomainType")) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    QMap<QString, int> typeStatistics;
    while (query.next()) {
        QString type = query.value(0).toString();
        if (type.isEmpty())
            type = "Unknown";
        int count = query.value(1).toInt();
        typeStatistics[type] = count;
    }
    QChart *chart = new QChart();
    chart->setTitle("Domain Type Distribution");
    QBarSet *set = new QBarSet("Count");
    QBarSeries *series = new QBarSeries();
    QStringList categories;
    for (auto it = typeStatistics.begin(); it != typeStatistics.end(); ++it) {
        categories << it.key();
        *set << it.value();
    }
    series->append(set);
    chart->addSeries(series);
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setInteractive(true);
    layout->addWidget(chartView);
}
void IDNIMS::on_calculateLevel_clicked()
{
    ui->statisticWidget->setCurrentIndex(2);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->showWidgetLevel->layout());
    if (layout != nullptr)
        clearLayout(layout);
    else
        layout = new QVBoxLayout(ui->showWidgetLevel);
    QSqlQuery query;
    if (!query.exec("SELECT DomainLevel FROM domain")) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }
    int statisticLevel[5] = {0, 0, 0, 0, 0};
    while (query.next()) {
        int a = query.value("DomainLevel").toInt();
        if (a >= 0 && a < 5)
            statisticLevel[a]++;
    }
    QChart *chart = new QChart();
    chart->setTitle("Domain Name Level Distribution");
    QBarSet *set = new QBarSet("Level");
    for (int i = 0; i < 5; i++) {
        *set << statisticLevel[i];
    }
    QStringList categories = {"Level 0", "Level 1", "Level 2", "Level 3", "Level 4"};
    QBarSeries *series = new QBarSeries();
    series->append(set);
    QBarCategoryAxis *axisX = new QBarCategoryAxis;
    axisX->setTitleText("Level");
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Domain Number");
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->setInteractive(true);
    layout->addWidget(chartView);
}
void IDNIMS::on_pushButton_clicked()
{
    int currentIndex = ui->statisticWidget->currentIndex();
    QChartView *currentChartView = nullptr;
    switch (currentIndex) {
        case 0:
            currentChartView = qobject_cast<QChartView*>(ui->showWidgetName->layout()->itemAt(0)->widget());
            break;
        case 1:
            currentChartView = qobject_cast<QChartView*>(ui->showWidgetType->layout()->itemAt(0)->widget());
            break;
        case 2:
            currentChartView = qobject_cast<QChartView*>(ui->showWidgetLevel->layout()->itemAt(0)->widget());
            break;
        default:
            QMessageBox::warning(this, "Error", "No chart is currently being displayed.");
            return;
    }
    if (!currentChartView) {
        QMessageBox::warning(this, "Error", "Could not find the chart view.");
        return;
    }
    QString filters = "PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)";
    QString defaultFilter = "PNG Files (*.png)";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Chart As Image"), "", filters, &defaultFilter);
    if (fileName.isEmpty())
        return;
    QPixmap pixmap = currentChartView->grab();
    if (!pixmap.save(fileName))
        QMessageBox::critical(this, "Error", "Failed to save the chart as an image.");
    else
        QMessageBox::information(this, "Success", "Chart saved successfully to " + fileName);
}
// The button on page 6
void IDNIMS::on_loadHost_clicked()
{ // Get the string on the three line edit and load into the local
    QSettings settings("config.ini", QSettings::IniFormat);
    QString oldName = settings.value("Database/Name").toString();
    QString oldUser = settings.value("Database/User").toString();
    QString oldPwd = settings.value("Database/Password").toString();
    settings.setValue("Database/Name", ui->sqlDbNameLineEdit->text());
    settings.setValue("Database/User", ui->sqlUsernameLineEdit->text());
    settings.setValue("Database/Password", sql.XOREncode(ui->sqlPwdLineEdit->text()));
    if (!sql.connectDataBase()) {
        settings.setValue("Database/Name", oldName);
        settings.setValue("Database/User", oldUser);
        settings.setValue("Database/Password", oldPwd);
        ui->tipsL->textLabel->setText("Connect Failed!");
        ui->tipsL->animationStart();
    }
}
void IDNIMS::on_loadChange_clicked()
{ // Allows user change their password
    if (administratorRights) {
        QString oldPassword = sql.Sha256Encode(ui->oldPwdLineEdit->text());
        QString password = sql.Sha256Encode(ui->changePwdLineEdit->text());
        QString passwordConfirm = sql.Sha256Encode(ui->changeConfirmLineEdit->text());
        if (oldPassword.isEmpty() || password.isEmpty() || passwordConfirm.isEmpty()) { // The case when all are empty
            ui->tipsR->textLabel->setText("Please enter the pwd!");
            ui->tipsR->animationStart();
        }
        else if (!checkPasswordStrength(password))
            QMessageBox::warning(this, "Warning", "Password is too weak! It must contain upper and lower case letters, numbers, and symbols, and be at least 8 characters long.");
        else if (password != passwordConfirm) { // The case when the both password are not the same
            ui->tipsR->textLabel->setText("The password is not the same!");
            ui->tipsR->animationStart();
        }
        else { // Successfully enter
            QSqlQuery query;
            QString qs = QString("SELECT * FROM user WHERE user_name = '%1' and password='%2'").arg(user).arg(oldPassword);
            if (!query.exec(qs)) {
                ui->tipsR->textLabel->setText("Change Failed! Check your password.");
                ui->tipsR->animationStart();
                return;
            }
            if (query.next()) { // Get the change
                QString updateQuery = QString("UPDATE user SET password = '%1' WHERE user_name = '%2'").arg(password).arg(user);
                if (query.exec(updateQuery)) {
                    ui->tipsR->textLabel->setText("Password updated successfully!");
                    ui->tipsR->animationStart();
                }
                else {
                    ui->tipsR->textLabel->setText("Failed to update password. Check your database connection.");
                    ui->tipsR->animationStart();
                }
            }
            else { // Change failed
                ui->tipsR->textLabel->setText("Change Failed! Check your password.");
                ui->tipsR->animationStart();
                return;
            }
        }
    }
    else
        QMessageBox::information(this, "Guest", "You are Guest!");
}
void IDNIMS::on_saveButton_clicked(bool checked)
{
    QSettings settings("config.ini", QSettings::IniFormat);
    if (checked)
        settings.setValue("Settings/DataBackup", true);
    else
        settings.setValue("Settings/DataBackup", false);
}
void IDNIMS::on_loadHostName_clicked()
{
    QString hostName = ui->sqlDbHostName->text();
    QRegExp ipRegex("^(\\d{1,3}\\.){3}\\d{1,3}$");
    if (!ipRegex.exactMatch(hostName)) { // Check whether follow the ruler
        QMessageBox::warning(this, "Invalid", "Please enter a valid IP address.");
        return;
    }
    QStringList parts = hostName.split(".");
    bool valid = true;
    for (const QString &part : parts) { // Check the number range
        bool ok;
        int ip = part.toInt(&ok);
        if (!ok || ip < 0 || ip > 255) {
            valid = false;
            break;
        }
    }
    if (!valid) { // Check whether follow the ruler
        QMessageBox::warning(this, "Invalid", "Please enter a valid IP address.");
        return;
    }
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue("Database/HostName", hostName);
    sql.connectDataBase();
}
void IDNIMS::on_loadPort_clicked()
{
    QString portStr = ui->sqlDbPort->text();
    bool ok;
    int port = portStr.toInt(&ok);
    if (!ok || port < 0 || port > 65535) { // Check whether is a integer and follow the range
        QMessageBox::warning(this, "Invalid", "Please enter a valid Port.");
        return;
    }
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue("Database/Port", ui->sqlDbPort->text());
    sql.connectDataBase();
}
void IDNIMS::on_rewriteTable_clicked()
{
    if (administratorRights) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            "Confirm Backup Restoration",
            "This will overwrite existing data with backup data. Are you sure you want to proceed?",
            QMessageBox::Yes|QMessageBox::No
            );
        if (reply == QMessageBox::No)
            return;
        QSqlQuery query(sql.db);
        if (!query.exec("SHOW TABLES LIKE 'domain_backup'")) {
            QMessageBox::critical(this, "Error", "Failed to check for backup table existence.");
            return;
        }
        if (!query.next()) {
            QMessageBox::information(this, "No Backup", "No backup data found.");
            return;
        }
        if (!query.exec("DROP TABLE IF EXISTS domain")) {
            QMessageBox::critical(this, "Error", "Failed to drop existing domain table.");
            return;
        }
        if (!query.exec("CREATE TABLE domain AS SELECT * FROM domain_backup")) {
            QMessageBox::critical(this, "Error", "Failed to create domain table from backup.");
            return;
        }
        QMessageBox::information(this, "Success", "Data restoration from backup completed successfully.");
    }
    else
        QMessageBox::information(this, "Guest", "You are Guest!");
}
void IDNIMS::on_setHomeButton_clicked()
{
    int pageNumber = ui->spinBoxPage->value();
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("Settings");
    settings.setValue("HomePage", pageNumber);
    settings.endGroup();
    QMessageBox::information(this, "Settings Updated", "Default home page set to " + QString::number(pageNumber));
}
void IDNIMS::on_nameSortButton_clicked()
{
    QString order = ui->sortRadio->isChecked() ? "DESC" : "ASC";
    QString queryString = QString("SELECT * FROM domain ORDER BY DomainName %1").arg(order);
    displaySortedData(queryString);
}
void IDNIMS::on_typeSortButton_clicked()
{
    QString order = ui->sortRadio->isChecked() ? "DESC" : "ASC";
    QString queryString = QString("SELECT * FROM domain ORDER BY DomainType %1").arg(order);
    displaySortedData(queryString);
}
void IDNIMS::on_levelSortButton_clicked()
{
    QString order = ui->sortRadio->isChecked() ? "DESC" : "ASC";
    QString queryString = QString("SELECT * FROM domain ORDER BY DomainLevel %1").arg(order);
    displaySortedData(queryString);
}
