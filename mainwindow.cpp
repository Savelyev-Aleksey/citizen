#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userdialog.h"
#include "searchform.h"

#include <QtSql>
#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!this->initConnection())
    {
        exit(1);
    }

    SearchForm *form = new SearchForm(this);
    this->setCentralWidget(form);
}


/**
 * @brief MainWindow::initConnection
 * init MySQL connection by readed settings from connection.ini file
 * placed near program.
 * Initialised connection named "main"
 * @return true on success or false on error.
 */
bool MainWindow::initConnection() const
{
    if (!QSqlDatabase::isDriverAvailable("QMYSQL"))
    {
        QStringList l = QSqlDatabase::drivers();
        QMessageBox::critical(0, QMessageBox::tr("Database"),
            QMessageBox::tr("Can't load database. Driver not available. Find: %1")
                              .arg(l.join("; ")),
            QMessageBox::Cancel);
        return false;
    }

    QSettings settings("connection.ini", QSettings::IniFormat);

    if (settings.status() != QSettings::NoError)
    {
        QMessageBox::critical(0, QMessageBox::tr("Settings"),
            QMessageBox::tr("Can't load connection.ini file"),
            QMessageBox::Cancel);
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "main");
    db.setHostName(settings.value("mysql/host").toString());
    db.setPort(settings.value("mysql/port").toInt());
    db.setUserName(settings.value("mysql/login").toString());
    db.setPassword(settings.value("mysql/password").toString());
    db.setDatabaseName(settings.value("mysql/database").toString());
    if (!db.open())
    {
        QMessageBox::critical(0, QMessageBox::tr("Database"),
            QMessageBox::tr("Can't open connection with database. Check localhost connection."),
            QMessageBox::Cancel);
        return false;
    }
    return true;
}



MainWindow::~MainWindow()
{
    delete ui;
}
