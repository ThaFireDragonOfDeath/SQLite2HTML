#ifndef DATABASEBACKEND_H
#define DATABASEBACKEND_H

#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QString>
#include <QtSql>
#include "exceptions.h"

class DatabaseBackend : public QObject
{
public:
    DatabaseBackend();
    QSqlQuery* execReadSql(QString sSqlQuery); //Returns 0x0 in case of error
    QSqlError execWriteSql(QString sSqlQuery);
    QSqlError initDbConnection();
    void setDbName(QString sNewDbName);
    void setDriver(QString sNewDbDriver);
    bool testDriver(); //Checks for driver support
    void writeError(QSqlError objNewSqlError);

protected:
    QSqlDatabase objDb;
    QString sDbDriver;
    QString sDbName;
};

#endif // DATABASEBACKEND_H
