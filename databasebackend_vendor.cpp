#include "databasebackend_vendor.h"

DatabaseBackend_Vendor::DatabaseBackend_Vendor(Memory* objNewParametersMemory)
{
    objParametersMemory = objNewParametersMemory;
}

DatabaseBackend_Vendor::~DatabaseBackend_Vendor()
{

}

QString DatabaseBackend_Vendor::addTableData(QString sTargetHtml, QString sTableData)
{
    QString sTableDataHtml = "<td>" + sTableData + "</td>";
    QString sReturn = sTargetHtml.append(sTableDataHtml);
    return sReturn;
}

QString DatabaseBackend_Vendor::addTableHeader(QString sTargetHtml, QString sTableHeader)
{
    QString sTableHeaderHtml = "<th>" + sTableHeader + "</th>";
    QString sReturn = sTargetHtml.append(sTableHeaderHtml);
    return sReturn;
}

QString DatabaseBackend_Vendor::beginRow(QString sTargetHtml)
{
    QString sReturn = sTargetHtml.append("<tr>");
    return sReturn;
}

QString DatabaseBackend_Vendor::applyTemplate(QString sTargetHtml)
{
    QString sEntryCount = objParametersMemory->getValue("ResultEntryCount");

    //If exist: use the template file from the local filesystem, else from the ressources, which are build in the executable
    QString sSysTemplateFilePath = "./Template/Template.html";
    QString sQrcTemplateFilePath = ":/Template/Template.html";
    bool bSysTemplateExist = checkFileExist(sSysTemplateFilePath);
    bool bQrcTemplateExist = checkFileExist(sQrcTemplateFilePath);
    QString sTemplateFilePath = "";
    QString sProjectTitle = objParametersMemory->getValue("ProjectTitle");

    if(bSysTemplateExist == true)
    {
        sTemplateFilePath = sSysTemplateFilePath;
    }
    else if(bQrcTemplateExist == true)
    {
        sTemplateFilePath = sQrcTemplateFilePath;
    }
    else
    {
        throw FileException{"Die Template Datei konnte nicht gefunden werden!"};
    }

    //Open the template file as reed only
    QFile objTemplateFile(sTemplateFilePath);
    bool bTemplateOpened = objTemplateFile.open(QFile::ReadOnly | QFile::Text);

    //Check if opened successfully and readable
    QFileInfo objTemplateFileInfo(objTemplateFile);
    bool bTemplateIsReadable = objTemplateFileInfo.isReadable();

    if(bTemplateOpened != true || bTemplateIsReadable != true)
    {
        FileException objEx{"Die Template Datei konnte nicht gelesen werden!"};
        throw objEx;
    }

    //Read the hole template file into a string and replace the keywords with the content from here
    QString sTemplate = objTemplateFile.readAll();
    objTemplateFile.close();
    sTemplate.replace(":EntryCount", sEntryCount);
    sTemplate.replace(":ProjectTitle", sProjectTitle);
    sTemplate.replace(":VideoTableContent", sTargetHtml);
    return sTemplate;
}

QString DatabaseBackend_Vendor::buildLinkList(QSqlQuery *objSqlQuery)
{
    int iResultSize = objSqlQuery->size();
    QString sVideoLinkList = "";
    objSqlQuery->seek(-1); //Reset position of the sqlquery

    //Loop over every result row
    while(objSqlQuery->next() == true)
    {
        int iCurrentRow = objSqlQuery->at();
        QString sVideoURL = objSqlQuery->value("VideoURL").toString();
        sVideoLinkList.append(sVideoURL);

        if(iCurrentRow < iResultSize) //If this is not the last entry
        {
            sVideoLinkList.append("\n"); //Add newline
        }
    }

    return sVideoLinkList;
}

QString DatabaseBackend_Vendor::buildTableContent(QString sCurrentHtmlString, QSqlQuery *objSqlQuery)
{
    objSqlQuery->seek(-1); //Reset position of the sqlquery
    int iElementsCount = 0;
    QString sLinkText = objParametersMemory->getValue("LinkText");

    //Loop over every result row
    while(objSqlQuery->next() == true)
    {
        //Read the wanted strings from the database
        QString sVideoName = objSqlQuery->value("VideoName").toString();
        QString sVideoDate = objSqlQuery->value("VideoDate").toString();
        QString sVideoQuality = objSqlQuery->value("VideoQuality").toString();
        QString sVideoURL = objSqlQuery->value("VideoURL").toString();
        QString sVideoDescURL = objSqlQuery->value("VideoDescURL").toString();

        sVideoQuality.append("p");

        //Convert the long URL in a short clickable text
        sVideoURL = parseToLink(sVideoURL, sLinkText);
        sVideoDescURL = parseToLink(sVideoDescURL, sLinkText);

        //Build the table entrys
        sCurrentHtmlString = beginRow(sCurrentHtmlString);
        sCurrentHtmlString = addTableData(sCurrentHtmlString, sVideoName);
        sCurrentHtmlString = addTableData(sCurrentHtmlString, sVideoDate);
        sCurrentHtmlString = addTableData(sCurrentHtmlString, sVideoQuality);
        sCurrentHtmlString = addTableData(sCurrentHtmlString, sVideoURL);
        sCurrentHtmlString = addTableData(sCurrentHtmlString, sVideoDescURL);
        sCurrentHtmlString = endRow(sCurrentHtmlString);

        //Increse the counter
        iElementsCount++;
    }

    //Save elements count in memory
    QString sEntryCount = QString::number(iElementsCount);
    objParametersMemory->setKeyValueEntry("ResultEntryCount", sEntryCount);

    return sCurrentHtmlString;
}

QString DatabaseBackend_Vendor::buildTableHead(QString sCurrentHtmlString)
{
    //Build table head
    sCurrentHtmlString = beginRow(sCurrentHtmlString);
    sCurrentHtmlString = addTableHeader(sCurrentHtmlString, "Titel");
    sCurrentHtmlString = addTableHeader(sCurrentHtmlString, "Datum");
    sCurrentHtmlString = addTableHeader(sCurrentHtmlString, "Qualität");
    sCurrentHtmlString = addTableHeader(sCurrentHtmlString, "Video URL");
    sCurrentHtmlString = addTableHeader(sCurrentHtmlString, "Beschreibung URL");
    sCurrentHtmlString = endRow(sCurrentHtmlString);
    return sCurrentHtmlString;
}

bool DatabaseBackend_Vendor::checkFileExist(QString sFilePath)
{
    QFile objFile(sFilePath);
    bool bFileExist = objFile.exists();
    return bFileExist;
}

QString DatabaseBackend_Vendor::endRow(QString sTargetHtml)
{
    QString sReturn = sTargetHtml.append("</tr>");
    return sReturn;
}

int DatabaseBackend_Vendor::getEntryCount(QSqlQuery *objSqlQuery)
{
    int iCount = objSqlQuery->size();
    return iCount;
}

void DatabaseBackend_Vendor::parseDatabasetoHtml()
{
    QString sDbPath = objParametersMemory->getValue("DatabasePath");

    //Checks if the database file exists
    QFile objDbFile(sDbPath);
    if(objDbFile.exists() != true)
    {
        throw FileException{"Die angegebene SQLite Datenbank konnte nicht gefunden werden!"};
    }

    //Set name and driver
    setDbName(sDbPath);
    setDriver("QSQLITE");

    //"Connect" to the database
    QSqlError objInitError = initDbConnection();
    if(objInitError.type() != QSqlError::NoError)
    {
        writeError(objInitError);
        throw DataBaseException{"Die SQLite Datanbank konnte nicht geladen werden!"};
    }

    emit sendStatusMessage("Datenbank wurde initialisiert");

    //Get all entrys ordered by date
    QString sSqlQueryString = "SELECT * FROM :TableName ORDER BY VideoDate DESC, VideoName ASC;";
    QString sProjectTableName = objParametersMemory->getValue("ProjectTableName");

    //Set table name in the SQL query
    sSqlQueryString.replace(":TableName", sProjectTableName);

    //Checks if the SQL query was successfull
    QSqlQuery* objSqlQuery = execReadSql(sSqlQueryString);
    if(objSqlQuery == 0x0)
    {
        throw DataBaseException{"Die SQL Abfrage ist fehlgeschlagen!"};
    }

    QString sHtmlFilePath = objParametersMemory->getValue("HtmlFilePath");
    QString sLinkListPath = objParametersMemory->getValue("LinkListPath");
    QString sHtmlTableContent = "";

    if(sHtmlFilePath != "")
    {
        sHtmlTableContent = buildTableHead(sHtmlTableContent); //Build table head
        sHtmlTableContent = buildTableContent(sHtmlTableContent, objSqlQuery); //Add the resaults to the HTML table
        emit sendStatusMessage("Abfrage erfolgreich durchgeführt");

        sHtmlTableContent = applyTemplate(sHtmlTableContent); //Use the Template to generate a the final HTML code

        //Write out the HTML File
        try
        {
            writeFile(sHtmlFilePath, sHtmlTableContent);
            emit sendStatusMessage("HTML Datei erfolgreich geschrieben");
        }
        catch(...)
        {
            emit sendStatusMessage("HTML Datei konnte nicht geschrieben werden. Wird übersprungen.");
        }
    }

    QString sLinkListContent = "";
    if(sLinkListPath != "")
    {
        sLinkListContent = buildLinkList(objSqlQuery);
        emit sendStatusMessage("Abfrage erfolgreich durchgeführt");

        //Write out the HTML File
        try
        {
            writeFile(sLinkListPath, sLinkListContent);
            emit sendStatusMessage("Linkliste erfolgreich geschrieben");
        }
        catch(...)
        {
            emit sendStatusMessage("Linkliste konnte nicht geschrieben werden. Wird übersprungen.");
        }
    }

    //Close the database connection
    objDb.close();
}

QString DatabaseBackend_Vendor::parseToLink(QString sLinkUrl, QString sLinkText)
{
    QString sLinkHtml;

    //If URL is empty just display the link text without href
    if(sLinkUrl == "")
    {
        sLinkHtml = sLinkText;
    }
    else
    {
        sLinkHtml = "<a href=\"" + sLinkUrl + "\">" + sLinkText + "</a>";
    }

    return sLinkHtml;
}

void DatabaseBackend_Vendor::writeFile(QString sFilePath, QString sContent)
{
    QString sErrorMessage = "Fehler beim Schreiben der Datei";

    //Open the target HTML file as read and write
    QFile objGeneretedHtmlFile(sFilePath);
    bool bGenHtmlFileIsOpen = objGeneretedHtmlFile.open(QFile::ReadWrite | QFile::Text | QFile::Truncate);

    QFileInfo objGenHtmlFileInfo(objGeneretedHtmlFile);
    bool bGenHtmlFileIsWriteable = objGenHtmlFileInfo.isWritable();

    if(bGenHtmlFileIsOpen != true || bGenHtmlFileIsWriteable != true)
    {
        FileException objEx{sErrorMessage};
        throw objEx;
    }

    //Write out the HTML content and check it
    qint64 iBytesWritten =  objGeneretedHtmlFile.write(sContent.toUtf8());
    objGeneretedHtmlFile.close();

    if(iBytesWritten == -1)
    {
        FileException objEx{sErrorMessage};
        throw objEx;
    }
}
