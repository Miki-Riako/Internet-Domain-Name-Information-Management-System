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

class IDNIMS : public QMainWindow
{
    Q_OBJECT

public:
    IDNIMS(QWidget *parent = nullptr);
    ~IDNIMS();
    QPushButton *drawerButtons[7];
    QWidget *pagePtr[7];
    QString user = "Guest";
    domainsql sql;
    bool staticsAllowed = true;
    bool administratorRights = false;
    bool inSSL = false;
    bool state[3] = {false, false, false};
    bool first[3] = {true, true, true};
    void fade(auto *control, int duration, int startValue, int endValue);
    void enter(void);
    void changePage(int page);
    void initialHomePage(void);
    void initialLogPage(void);
    void initialSettingPage(void);
    void keyPressEvent(QKeyEvent *event);
    QString removeFirstSegment(QString& input);
    QtMaterialAutoComplete *pwdLineEdit[3];

private slots:
    void on_loadAll_clicked();
    void on_clearAll_clicked();
    void on_uploadAll_clicked();
    void on_establishButton_clicked();
    void on_categorizeButton_clicked();
    void on_insertButton_clicked();
    void on_removeButton_clicked();
    void on_modifyButton_clicked();
    void on_domainButton_clicked();
    void on_searchDomain_clicked();
    void on_loadHost_clicked();
    void on_loadChange_clicked();
    void on_nodeButton_clicked(bool checked);
    void on_comboBox_currentIndexChanged(int index);
    void on_saveButton_clicked(bool checked);
    void on_calculateLevel_clicked();

private:
    Ui::IDNIMS *ui;
    Login login;
    establishdialog establishWindow;
    insertdialog insertWindow;
    categorizedialog categorizeWindow;
    removedialog removeWindow;
    modifydialog modifyWindow;
    QtMaterialDrawer *const drawer;
};
#endif // IDNIMS_H
