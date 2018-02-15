#ifndef SQLCONNECTION_H
#define SQLCONNECTION_H

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QCoreApplication>
#include <QSettings>
#include <QStringList>

class SqlConnection
{
public:
    /**
     * @brief SqlConnection::mainConnection
     * return main connection instance, if connection lost try to reconnect,
     * or open message box with error and return QSqlDatabase connection.
     * @return QSqlDatabase
     */
    static QSqlDatabase& mainConnection()
    {
        static QSqlDatabase mainCon = QSqlDatabase::database("main");

        if (!mainCon.isOpen())
        {
            mainCon.open();
            if (mainCon.isOpenError())
            {
                QMessageBox::critical(nullptr,
                    QCoreApplication::translate("connection", "Connecntion error"),
                    QCoreApplication::translate("connection", "Database connection lost: %1").
                                      arg(mainCon.lastError().text()));
            }
        }
        return mainCon;
    }



    static void closeMainConnection()
    {
        QSqlDatabase mainCon = SqlConnection::mainConnection();
        mainCon.close();
    }



    /**
     * @brief SqlConnection::initMainConnection
     * init MySQL connection by readed settings from connection.ini file
     * placed near program.
     * Initialised connection named "main"
     * @return true on success or false on error.
     */
    static bool initMainConnection()
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
                QMessageBox::tr("Can't open connection with database. Check localhost connection: %1").
                                  arg(db.lastError().text()),
                QMessageBox::Cancel);
            return false;
        }
        return true;
    }

private:
    SqlConnection(){}
    SqlConnection(const SqlConnection& root) = delete;
    SqlConnection& operator=(const SqlConnection&) = delete;
};

#endif // SQLCONNECTION_H
