#ifndef DATABASEBACKEND_VENDOR_H
#define DATABASEBACKEND_VENDOR_H

#include <QFile>
#include <QFileInfo>
#include <QVariant>
#include "databasebackend.h"
#include "memory.h"

class DatabaseBackend_Vendor : public DatabaseBackend //Project specific part
{
    Q_OBJECT

public:
    DatabaseBackend_Vendor(Memory* objNewParametersMemory);
    ~DatabaseBackend_Vendor();
    void parseDatabasetoHtml(); //Convert the Sqlite database to HTML

signals:
    void sendStatusMessage(QString sStatusMessage);

private:
    QString addTableHeader(QString sTargetHtml, QString sTableHeader); //Add one HTML table Head entry
    QString addTableData(QString sTargetHtml, QString sTableData); //Add one HTML entry
    QString applyTemplate(QString sTargetHtml);
    QString beginRow(QString sTargetHtml); //Begin a row in an HTML table
    QString buildLinkList(QSqlQuery* objSqlQuery);
    QString buildTableContent(QString sCurrentHtmlString, QSqlQuery* objSqlQuery);
    QString buildTableHead(QString sCurrentHtmlString); //Build the header of the Table
    bool checkFileExist(QString sFilePath);
    QString endRow(QString sTargetHtml); //End the row
    int getEntryCount(QSqlQuery* objSqlQuery);
    QString parseToLink(QString sLinkUrl, QString sLinkText); //Parse the URL to a short clickable link
    void writeFile(QString sFilePath, QString sContent);

    Memory* objParametersMemory; //Deleted in DatabaseBackend_Thread
};

#endif // DATABASEBACKEND_VENDOR_H
