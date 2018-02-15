#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlError>
#include <QStringList>
#include <QMainWindow>

#include "sqlconnection.h"
#include "searchform.h"
#include "ui_searchform.h"
#include "userdialog.h"



SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);

    QSqlDatabase db = SqlConnection::mainConnection();
    if (!db.isOpen())
    {
        return;
    }

    searchModel = new QSqlQueryModel(this);
    searchModel->setQuery(getQuery(QString("")), db);

    ui->searchView->setModel(searchModel);
    ui->searchView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->searchView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->searchView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->searchView->setColumnHidden(indexId, true);

    QSqlRecord r = searchModel->record(0);
    indexId         = r.indexOf("id");
    indexLname      = r.indexOf("LastName");
    indexFname      = r.indexOf("FirstName");
    indexMname      = r.indexOf("MiddleName");
    indexBirthday   = r.indexOf("Birthday");
    indexRegisterDate = r.indexOf("RegisterDate");
    indexUserStatus = r.indexOf("UserStatus");

    ui->statusCombo->addItems(UserDialog::getUserStatusList());


    searchModel->setHeaderData(indexId, Qt::Horizontal, tr("Card number"));
    searchModel->setHeaderData(indexFname, Qt::Horizontal, tr("First name"));
    searchModel->setHeaderData(indexLname, Qt::Horizontal, tr("Last name"));
    searchModel->setHeaderData(indexMname, Qt::Horizontal, tr("Middle name"));
    searchModel->setHeaderData(indexBirthday, Qt::Horizontal, tr("Birthday"));
    searchModel->setHeaderData(indexRegisterDate, Qt::Horizontal, tr("Register date"));
    searchModel->setHeaderData(indexUserStatus, Qt::Horizontal, tr("User status"));

    ui->searchView->resizeColumnsToContents();


    connect(ui->searchView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(editUser(const QModelIndex &)));

    connect(ui->newUserButton, SIGNAL(clicked(bool)),
            this, SLOT(newUser()));

    connect(ui->searchButton, SIGNAL(clicked(bool)), this, SLOT(findUsers()));
    connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(clearFields()));
}



SearchForm::~SearchForm()
{
    delete ui;
}



QString SearchForm::getQuery(QString where)
{
    QString select("SELECT id, LastName, FirstName, MiddleName, Birthday, RegisterDate, UserStatus");

    QString from(" FROM `user_main` m");

    QString order(" ORDER BY LastName, FirstName, MiddleName, Birthday");

    if (where.length())
    {
        return select + from + QString(" WHERE ") + where + order;
    }
    return select + from + order;
}



void SearchForm::openUserForm(uint userID)
{
    UserDialog dialog(this, userID);
    dialog.setWindowFlag(Qt::WindowMinMaxButtonsHint, true);

    int res = dialog.exec();

    if (res == QDialog::Accepted)
    {
        searchModel->query().exec();
        ui->searchView->resizeColumnsToContents();
    }
}



void SearchForm::editUser(const QModelIndex &index)
{
    int row = index.row();
    QSqlRecord rec = searchModel->record(row);
    uint userID = rec.value(indexId).toUInt();

    openUserForm(userID);
}



void SearchForm::newUser()
{
    openUserForm(0);
}



void SearchForm::findUsers()
{
    QStringList list;

    ui->infoLabel->clear();

    if (ui->birthdayCheck->isChecked())
    {
        list.push_front(QString("Birthday=\"%1\"").arg(ui->birthday->date().toString("yyyy-MM-dd")));
    }
    if (ui->statusCheck->isChecked() && ui->statusCombo->currentText().compare("") != 0)
    {
        list.push_front(QString("UserStatus=\"%1\"").arg(ui->statusCombo->currentText()));
    }

    QString fname = ui->firstName->text().trimmed();
    if (fname.length())
    {
        list.push_front(QString("FirstName like \"%1%\"").arg(fname));
    }
    QString lname = ui->lastName->text().trimmed();
    if (lname.length())
    {
        list.push_front(QString("LastName like \"%1%\"").arg(lname));
    }
    QString mname = ui->middleName->text().trimmed();
    if (mname.length())
    {
        list.push_front(QString("MiddleName like \"%1%\"").arg(mname));
    }

    QSqlDatabase db = SqlConnection::mainConnection();
    if (!db.isOpen())
    {
        return;
    }

    searchModel->setQuery(getQuery(list.join(" AND ")), db);
    ui->searchView->resizeColumnsToContents();
    QSqlError error = searchModel->lastError();

    if (error.isValid())
    {
        ui->infoLabel->setText(error.text());
    }
    else
    {
        ui->infoLabel->setText(tr("Found: %1").arg(searchModel->rowCount()));
    }
}



void SearchForm::clearFields()
{
    ui->firstName->clear();
    ui->lastName->clear();
    ui->middleName->clear();
    ui->birthdayCheck->setChecked(false);
    ui->statusCheck->setChecked(false);

    findUsers();
}
