#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QString>
#include <QVariant>
#include <QDate>
#include <QDir>
#include <ActiveQt/QAxObject>

#include "sqlconnection.h"
#include "userdialog.h"
#include "userqueueform.h"
#include "ui_userqueueform.h"

UserQueueForm::UserQueueForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserQueueForm)
{
    ui->setupUi(this);

    QSqlDatabase db = SqlConnection::mainConnection();
    if (!db.isOpen())
    {
        return;
    }

    queueModel = new QSqlQueryModel();
    queueModel->setQuery(getQuery(), db);

    ui->queueView->setModel(queueModel);
    ui->queueView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->queueView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->queueView->setColumnHidden(1, true);

    queueModel->setHeaderData(0, Qt::Horizontal, tr("Queue"));
    queueModel->setHeaderData(2, Qt::Horizontal, tr("Last name"));
    queueModel->setHeaderData(3, Qt::Horizontal, tr("First name"));
    queueModel->setHeaderData(4, Qt::Horizontal, tr("Middle name"));
    queueModel->setHeaderData(5, Qt::Horizontal, tr("Birthday"));
    queueModel->setHeaderData(6, Qt::Horizontal, tr("Register date"));
    queueModel->setHeaderData(7, Qt::Horizontal, tr("User status"));

    connect(ui->queueView, SIGNAL(clicked(QModelIndex)), this, SLOT(queueSelected(QModelIndex)));

    connect(ui->queueView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(editUser(const QModelIndex &)));

    connect(ui->moveUp, SIGNAL(clicked(bool)), this, SLOT(moveQueueUp()));
    connect(ui->moveDown, SIGNAL(clicked(bool)), this, SLOT(moveQueueDown()));
    connect(ui->makeFirst, SIGNAL(clicked(bool)), this, SLOT(moveQueueFirst()));
    connect(ui->print, SIGNAL(clicked(bool)), this, SLOT(printQueue()));
}



UserQueueForm::~UserQueueForm()
{
    delete queueModel;
    delete ui;
}



QString UserQueueForm::getQuery() const
{
    return QString("SELECT q.queue_id, u.id, u.LastName, u.FirstName, u.MiddleName, u.Birthday, "
                   " u.RegisterDate, u.UserStatus"
                   " FROM user_main as u INNER JOIN user_queue as q on u.id=q.user_id"
                   " ORDER BY q.queue_id");
}



void UserQueueForm::queueSelected(const QModelIndex &index)
{
    ui->makeFirst->setEnabled(index.row() > 0);
    ui->moveUp->setEnabled(index.row() > 0);
    ui->moveDown->setEnabled(index.row() < queueModel->rowCount() - 1);
}



void UserQueueForm::updateQueue()
{
    QSqlDatabase db = SqlConnection::mainConnection();
    if (!db.isOpen())
    {
        return;
    }
    queueModel->setQuery(getQuery(), db);
}



void UserQueueForm::moveQueueUp()
{
    QSqlDatabase db = SqlConnection::mainConnection();
    if (!db.isOpen())
    {
        return;
    }

    int row = ui->queueView->currentIndex().row();
    QModelIndex index = queueModel->index(row, 1);
    QModelIndex index_prev = queueModel->index(row - 1, 1);

    uint user_id = queueModel->data(index).toUInt();
    uint prev_user_id = queueModel->data(index_prev).toUInt();

    db.transaction();
    QSqlQuery q(db);
    q.prepare("UPDATE user_queue SET queue_id = queue_id - 1 WHERE user_id = :id");
    q.bindValue(":id", user_id);
    q.exec();
    q.prepare("UPDATE user_queue SET queue_id = queue_id + 1 WHERE user_id = :id");
    q.bindValue(":id", prev_user_id);
    q.exec();
    db.commit();
    if (q.lastError().isValid())
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("exec error: %1").arg(q.lastError().text()));
    }
    queueModel->setQuery(getQuery(), db);
    ui->queueView->setCurrentIndex(index_prev);
    queueSelected(index_prev);
}



void UserQueueForm::moveQueueDown()
{
    QSqlDatabase db = SqlConnection::mainConnection();
    if (!db.isOpen())
    {
        return;
    }

    int row = ui->queueView->currentIndex().row();
    QModelIndex index = queueModel->index(row, 1);
    QModelIndex index_next = queueModel->index(row + 1, 1);

    uint user_id = queueModel->data(index).toUInt();
    uint next_user_id = queueModel->data(index_next).toUInt();

    db.transaction();
    QSqlQuery q(db);
    q.prepare("UPDATE user_queue SET queue_id = queue_id + 1 WHERE user_id = :id");
    q.bindValue(":id", user_id);
    q.exec();
    q.prepare("UPDATE user_queue SET queue_id = queue_id - 1 WHERE user_id = :id");
    q.bindValue(":id", next_user_id);
    q.exec();
    db.commit();
    if (q.lastError().isValid())
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("exec error: %1").arg(q.lastError().text()));
    }
    queueModel->setQuery(getQuery(), db);
    ui->queueView->setCurrentIndex(index_next);
    queueSelected(index_next);
}



void UserQueueForm::moveQueueFirst()
{
    QSqlDatabase db = SqlConnection::mainConnection();
    if (!db.isOpen())
    {
        return;
    }

    int row = ui->queueView->currentIndex().row();

    QModelIndex index_queue = queueModel->index(row, 0);
    QModelIndex index_user = queueModel->index(row, 1);

    uint queue_id = queueModel->data(index_queue).toUInt();
    uint user_id = queueModel->data(index_user).toUInt();

    db.transaction();
    QSqlQuery q(db);
    q.prepare("UPDATE user_queue SET queue_id = queue_id + 1 WHERE queue_id < :id");
    q.bindValue(":id", queue_id);
    q.exec();
    q.prepare("UPDATE user_queue SET queue_id = 1 WHERE user_id = :id");
    q.bindValue(":id", user_id);
    q.exec();
    db.commit();
    if (q.lastError().isValid())
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("exec error: %1").arg(q.lastError().text()));
    }
    queueModel->setQuery(getQuery(), db);
    QModelIndex index_first = queueModel->index(0, 0);
    ui->queueView->setCurrentIndex(index_first);
    queueSelected(index_first);
}



void UserQueueForm::printQueue()
{
    //создаем экземпляр Word
    QAxObject *word = new QAxObject("Word.Application", nullptr);
    if (word == nullptr)
    {
        return;
    }
    //получаем коллекцию документов
    QAxObject *documents = word->querySubObject("Documents");

    QVariant filename(QDir::currentPath() + "/templates/queue_list.dotx");
    QVariant confirmconversions(false);
    QVariant readonly(true);
    QVariant addtorecentfiles(false);
    QVariant passworddocument("");
    QVariant passwordtemplate("");
    QVariant revert(false);
    //добавляем свой документ в коллекцию на основе Шаблон.dot
    QAxObject *newDocument = documents->querySubObject("Open(const QVariant&, const QVariant&, "
        "const QVariant&, const QVariant&, const QVariant&, const QVariant&,const QVariant&)",
        filename, confirmconversions, readonly, addtorecentfiles, passworddocument,
        passwordtemplate, revert);

    if (newDocument == nullptr)
    {
        return;
    }
    //Получаем коллекцию закладок
    QAxObject *bookmarks = newDocument->querySubObject("Bookmarks()");
    //Конкретная закладка
    QAxObject *bookmark = bookmarks->querySubObject("Item(Qstring)", "queue");
    QAxObject *range = bookmark->querySubObject("Range");

    QAxObject *tables = range->querySubObject("Tables(1)");
    QAxObject *row = range->querySubObject("Rows()");
    QDate date;
    QAxObject *cell, *cellR;
    QString value;

    for(int i = queueModel->rowCount() - 1; i >=0 ; --i)
    {
        row->dynamicCall("Add()");
        // id
        value = queueModel->data(queueModel->index(i, 0)).toString();
        cell = tables->querySubObject("Cell(Row, Column)", 2, 1);
        cellR = cell->querySubObject("Range()");
        cellR->dynamicCall("InsertAfter(Text)", value);
        // Last name
        value = queueModel->data(queueModel->index(i, 2)).toString();
        cell = tables->querySubObject("Cell(Row, Column)", 2, 2);
        cellR = cell->querySubObject("Range()");
        cellR->dynamicCall("InsertAfter(Text)", value);
        // First Name
        value = queueModel->data(queueModel->index(i, 3)).toString();
        cell = tables->querySubObject("Cell(Row, Column)", 2, 3);
        cellR = cell->querySubObject("Range()");
        cellR->dynamicCall("InsertAfter(Text)", value);
        // Middle Name
        value = queueModel->data(queueModel->index(i, 4)).toString();
        cell = tables->querySubObject("Cell(Row, Column)", 2, 4);
        cellR = cell->querySubObject("Range()");
        cellR->dynamicCall("InsertAfter(Text)", value);
        // Birthday
        date = queueModel->data(queueModel->index(i, 5)).toDate();
        cell = tables->querySubObject("Cell(Row, Column)", 2, 5);
        cellR = cell->querySubObject("Range()");
        cellR->dynamicCall("InsertAfter(Text)", date.toString(Qt::SystemLocaleShortDate));
    }


    // Show word document
    word->dynamicCall("Visible", true);
}



void UserQueueForm::editUser(const QModelIndex &index)
{
    int row = index.row();
    QSqlRecord rec = queueModel->record(row);
    uint userID = rec.value(1).toUInt();

    UserDialog dialog(this, userID);
    dialog.setWindowFlag(Qt::WindowMinMaxButtonsHint, true);

    int res = dialog.exec();

    if (res == QDialog::Accepted)
    {
        updateQueue();
    }
}
