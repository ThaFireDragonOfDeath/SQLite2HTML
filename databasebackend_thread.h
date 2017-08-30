#ifndef DATABASEBACKEND_THREAD_H
#define DATABASEBACKEND_THREAD_H

#include <QThread>
#include "databasebackend_vendor.h"

class MainWindow;

class DatabaseBackend_Thread : public QThread
{
    Q_OBJECT

public:
    DatabaseBackend_Thread(Memory* objNewParametersMemory, MainWindow* objNewSignalReceiver);
    ~DatabaseBackend_Thread();
    void run();

signals:
    void sendErrorMessage(QString sErrorMessage);
    void sendStatusMessage(QString sStatusMessage);
    void sendThreadFinished();

private:
    DatabaseBackend_Vendor* objDbVendor = 0x0; //Deleted here
    Memory* objParametersMemory = 0x0; //Deleted here
    MainWindow* objSignalReceiver; //Deleted in main
};

#endif // DATABASEBACKEND_THREAD_H
