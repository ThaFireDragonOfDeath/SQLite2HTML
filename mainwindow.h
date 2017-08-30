#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QErrorMessage>
#include <QFileDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include "config.h"
#include "databasebackend_thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void conversionThreadFinished();
    void setStatus(QString sStatusMessage);
    void showErrorMessage(QString sErrorMessage);

private slots:
    void on_btnSelectDatabase_clicked();
    void on_btnSelectHtmlSavePath_clicked();
    void on_btnSelectLinklistSavePath_clicked();
    void on_btnStartConvert_clicked();

private:
    DatabaseBackend_Thread *objDbThread = 0x0;
    Memory* objConfigMemory = 0x0;
    Memory* objInternalMemory = 0x0;
    Ui::MainWindow *ui;
    QString sConfigFilePath;
    QString sInternalStoreFilaPath;
};

#endif // MAINWINDOW_H
