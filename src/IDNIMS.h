#ifndef IDNIMS_H
#define IDNIMS_H

#include "login.h"
#include "establishdialog.h"
#include "categorizedialog.h"
#include "insertdialog.h"
#include "removedialog.h"
#include "modifydialog.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class IDNIMS;
}
QT_END_NAMESPACE

//************
class MessageHandler {
public:
    virtual void showMessage() = 0;
    virtual ~MessageHandler() {}
};
class GuestMessageHandler : public MessageHandler {
public:
    void showMessage() override { QMessageBox::information(nullptr, "Guest Access", "You have guest access with limited functionality."); }
};
class AdminMessageHandler : public MessageHandler {
public:
    void showMessage() override { QMessageBox::information(nullptr, "Admin Access", "You have administrator access with full functionality."); }
};
//************

class IDNIMS : public QMainWindow
{
    Q_OBJECT

public:
    void setUserType(bool isAdmin) {
        delete messageHandler;
        if (isAdmin) messageHandler = new AdminMessageHandler();
        else messageHandler = new GuestMessageHandler();
    }
    void displayMessage() { if (messageHandler) messageHandler->showMessage(); }

    IDNIMS(QWidget *parent = nullptr);
    ~IDNIMS();
    QPushButton *drawerButtons[7];
    QWidget *pagePtr[7];
    QString user = "Guest";
    domainsql sql;
    bool staticsAllowed[3] = {true, true, true};
    bool administratorRights = false;
    bool state[3] = {false, false, false};
    bool first[3] = {true, true, true};
    void clearLayout(QLayout *layout);
    void fade(auto *control, const int &duration, const int &startValue, const int &endValue);
    bool checkPasswordStrength(const QString &password);
    void enter(void);
    void changePage(const int &page);
    void initialHomePage(void);
    void initialSearchPage(void);
    void initialLogPage(void);
    void initialSettingPage(void);
    void loadDomainData(const int &level);
    void keyPressEvent(QKeyEvent *event);
    void displaySortedData(const QString &queryString);
    QtMaterialAutoComplete *pwdLineEdit[3];

private slots:
    void on_loadAll_clicked();
    void on_clearAll_clicked();
    void on_deriveAll_clicked();
    void on_establishButton_clicked();
    void on_categorizeButton_clicked();
    void on_insertButton_clicked();
    void on_removeButton_clicked();
    void on_modifyButton_clicked();
    void on_domainButton_clicked();
    void on_searchDomain_clicked();
    void on_loadHost_clicked();
    void on_loadChange_clicked();
    void on_saveButton_clicked(bool checked);
    void on_calculateName_clicked();
    void on_calculateType_clicked();
    void on_calculateLevel_clicked();
    void on_pushButton_clicked();
    void on_loadHostName_clicked();
    void on_loadPort_clicked();
    void on_rewriteTable_clicked();
    void on_setHomeButton_clicked();
    void on_nameSortButton_clicked();
    void on_typeSortButton_clicked();
    void on_levelSortButton_clicked();

private:
    Ui::IDNIMS *ui;
    Login login;
    establishdialog establishWindow;
    insertdialog insertWindow;
    categorizedialog categorizeWindow;
    removedialog removeWindow;
    modifydialog modifyWindow;
    QtMaterialDrawer *const drawer;

    MessageHandler* messageHandler;
};
#endif // IDNIMS_H
