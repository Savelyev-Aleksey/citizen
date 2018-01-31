#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QMessageBox>

#include "userqueueform.h"
#include "ui_userqueueform.h"

UserQueueForm::UserQueueForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserQueueForm)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen())
    {
        QMessageBox::critical(this, tr("Connecntion error"), tr("Database connection lost"));
        return;
    }

    queueModel = new QSqlQueryModel();
    queueModel->setQuery("SELECT u.LastName, u.FirstName, u.MiddleName, u.Birthday, u.RegisterDate"
                         " FROM user_main as u INNER JOIN user_queue as q on u.id=q.user_id"
                         " ORDER BY q.queue_id", db);

    ui->userQueue->setModel(queueModel);
}

UserQueueForm::~UserQueueForm()
{
    delete queueModel;
    delete ui;
}
