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
class QString;
class QStringList;

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDialog(QWidget *parent = 0, uint userID = 0);
    static QStringList getUserStatusList();
    ~UserDialog();

protected:
    QSqlRelationalTableModel *mainModel = nullptr;
    QSqlRelationalTableModel *medModel = nullptr;
    QSqlRelationalTableModel *socModel = nullptr;

    QDataWidgetMapper *mainMapper = nullptr;

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
    int indexDocName;
    int indexDocType;
    int indexDocUserID;

    QString lastStatusId;

    void showError(const QSqlError &err);
    void trimInput(QLineEdit *edit);
    void trimInput(QTextEdit *edit);
    void trimInputs();
    void setNullEmptyData();
    void addUserInQueue();
    void removeUserFromQueue();

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
