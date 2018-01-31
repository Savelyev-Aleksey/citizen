#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>

namespace Ui {
class SearchForm;
}

class QSqlQueryModel;
class QSqlTableModel;
class QString;

class SearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = 0);
    ~SearchForm();

protected:
    QSqlQueryModel *searchModel = nullptr;

    int indexId;
    int indexFname;
    int indexLname;
    int indexMname;
    int indexBirthday;
    int indexSenderDepartment;
    int indexCurrentPlace;
    int indexContactUser;
    int indexPassportSeria;
    int indexPassportNumber;
    int indexPassportIssue;
    int indexPassportDepartment;
    int indexAdditionalInfo;
    int indexUserStatus;
    int indexRegisterDate;

    void openUserForm(uint userID);
    QString getQuery(QString where);

protected slots:
    void editUser(const QModelIndex &index);
    void newUser();

    void findUsers();
    void clearFields();

private:
    Ui::SearchForm *ui;
};

#endif // SEARCHFORM_H
