#include "databasebackend_thread.h"
#include "mainwindow.h"

DatabaseBackend_Thread::DatabaseBackend_Thread(Memory* objNewParametersMemory, MainWindow* objNewSignalReceiver)
{
    objParametersMemory = objNewParametersMemory;
    objSignalReceiver = objNewSignalReceiver;
}

DatabaseBackend_Thread::~DatabaseBackend_Thread()
{
    delete objDbVendor;
    delete objParametersMemory;
}

void DatabaseBackend_Thread::run()
{
    objDbVendor = new DatabaseBackend_Vendor(objParametersMemory);

    //Connect signals and slots
    connect(this, &DatabaseBackend_Thread::sendThreadFinished, objSignalReceiver, &MainWindow::conversionThreadFinished);
    connect(this, &DatabaseBackend_Thread::sendErrorMessage, objSignalReceiver, &MainWindow::showErrorMessage);
    connect(this, &DatabaseBackend_Thread::sendStatusMessage, objSignalReceiver, &MainWindow::setStatus);
    connect(objDbVendor, &DatabaseBackend_Vendor::sendStatusMessage, objSignalReceiver, &MainWindow::setStatus);

    try
    {
        objDbVendor->parseDatabasetoHtml();
    }
    catch(DataBaseException exDb) //In case of problems with the database
    {
        QString sFinalErrorMessage = "Es trat ein Fehler in der Datenbank auf: " + exDb.sErrorMessage;
        emit sendErrorMessage(sFinalErrorMessage);
        emit sendStatusMessage("Umwandlung fehlgeschlagen");
        emit sendThreadFinished();
        return;
    }
    catch(FileException exFile) //In case of problems with the filesystem
    {
        QString sFinalErrorMessage = "Es trat ein Fehler im Dateisystem auf: " + exFile.sErrorMessage;
        emit sendErrorMessage(sFinalErrorMessage);
        emit sendStatusMessage("Umwandlung fehlgeschlagen");
        emit sendThreadFinished();
        return;
    }

    emit sendStatusMessage("Umwandlung abgeschlossen"); //Sends success message
    emit sendThreadFinished(); //Reactivates the convert button
}
