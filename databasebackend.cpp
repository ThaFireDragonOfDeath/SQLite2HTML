#include "databasebackend.h"

DatabaseBackend::DatabaseBackend()
{
    sDbDriver = "QSQLITE";
    sDbName = ":memory:";
}

QSqlQuery* DatabaseBackend::execReadSql(QString sSqlQuery)
{
    //Create the SQL query
    QSqlQuery* objSqlQuery = new QSqlQuery(objDb);
    bool bExecSuccess = objSqlQuery->exec(sSqlQuery);

    //Check for succes and return QSqlQuery object or nullpointer
    if(bExecSuccess == false)
    {
        return 0x0;
    }
    else
    {
        return objSqlQuery;
    }
}


QSqlError DatabaseBackend::execWriteSql(QString sSqlQuery)
{
    //Start the transaction
    bool bStartTransAction = objDb.transaction();
    if(bStartTransAction == false)
    {
        return QSqlError("transaction() failed", "", QSqlError::TransactionError);
    }

    //Do the SQL query
    QSqlQuery objSqlQuery(objDb);
    bool bExecSuccess = objSqlQuery.exec(sSqlQuery);
    if(bExecSuccess == false)
    {
        return objDb.lastError();
    }

    //Commit the changes
    bool bCommitSucces = objDb.commit();
    if(bCommitSucces == false)
    {
        return objDb.lastError();
    }
    else
    {
        return QSqlError();
    }
}

QSqlError DatabaseBackend::initDbConnection()
{
    //Checks if Qt have the required SQL driver
    if(testDriver() != true)
    {
        QSqlError objDriverSqlError("Driver not installed", "Driver not installed", QSqlError::ConnectionError);
        return objDriverSqlError;
    }

    //Init and open the database
    objDb = QSqlDatabase::addDatabase(sDbDriver);
    objDb.setDatabaseName(sDbName);
    bool bDatabaseOpened = objDb.open();

    //Checks for success
    if (bDatabaseOpened == false)
    {
        return objDb.lastError();
    }
    else
    {
        return QSqlError(); //Returns an empty QSqlError, if all was right
    }
}

void DatabaseBackend::setDbName(QString sNewDbName)
{
    sDbName = sNewDbName;
}

void DatabaseBackend::setDriver(QString sNewDbDriver)
{
    sDbDriver = sNewDbDriver;
}

bool DatabaseBackend::testDriver()
{
    //Get the list of installed SQL drivers
    QStringList sDrivers = QSqlDatabase::drivers();
    qDebug() << "Available drivers:" << sDrivers;

    if (sDrivers.contains(sDbDriver) == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DatabaseBackend::writeError(QSqlError objNewSqlError)
{
    qDebug() << "Driver Text:" << objNewSqlError.driverText();
    qDebug() << "Database Text:" << objNewSqlError.databaseText();
    qDebug() << "Database Text:" << objNewSqlError.type();
}
