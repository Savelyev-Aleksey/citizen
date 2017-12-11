#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>

namespace Ui {
class UserDialog;
}

class QSqlTableModel;
class QSqlRelationalTableModel;
class QSqlQuery;
class QDataWidgetMapper;
class QSqlError;
class QTextEdit;
class QLineEdit;

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDialog(QWidget *parent = 0, uint userID = 0);
    ~UserDialog();

protected:
    QSqlRelationalTableModel *mainModel = NULL;
    QSqlRelationalTableModel *medModel = NULL;
    QSqlRelationalTableModel *socModel = NULL;

    QDataWidgetMapper *mainMapper = NULL;

    uint userID = 0;

    // main model indexes
    int indexUserID;
    int indexBurthday;
    int indexRegister;
    int indexOccupation;
    int indexLeave;
    int indexPassportSeria;
    int indexPassportNumber;
    int indexUserStatus;
    int indexProtocolDate;
    int indexExclusionDate;
    int indexSelfAbility;
    int indexDisability;
    int indexWorkCompetence;

    // med soc folder indexes
    int indexDocCategory;
    int indexDocType;
    int indexDocUserID;

    void showError(const QSqlError &err);
    void trimInput(QLineEdit *edit);
    void trimInput(QTextEdit *edit);
    void trimInputs();
    void setNullEmptyData();

    bool saveMedDocuments();

protected slots:
    void saveUser();
    void removeUser();
    void initMedFolder(int folderIndex);
    void initSocFolder(int folderIndex);

    void addMedRow();
    void addSocRow();
    void enableMedRemoveButton();
    void removeMedDocument();

private:
    Ui::UserDialog *ui;
};

#endif // USERDIALOG_H
