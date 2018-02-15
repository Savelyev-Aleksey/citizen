#include <QtSql>
#include <QSettings>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userdialog.h"
#include "searchform.h"
#include "userqueueform.h"
#include "sqlconnection.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!this->initConnection())
    {
        exit(1);
    }

    SearchForm *search = new SearchForm();

    queueForm = new UserQueueForm();

    ui->mainTabWidget->addTab(search, tr("Search"));
    ui->mainTabWidget->addTab(queueForm, tr("Queue"));

    connect(ui->mainTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}


/**
 * @brief MainWindow::initConnection
 * init MySQL connection
 * @return true on success or false on error.
 */
bool MainWindow::initConnection() const
{
    return SqlConnection::initMainConnection();
}



MainWindow::~MainWindow()
{
    SqlConnection::closeMainConnection();
    delete ui;
}



void MainWindow::tabChanged(int tabIndex)
{
    if (tabIndex == 1)
    {
        queueForm->updateQueue();
    }
}
