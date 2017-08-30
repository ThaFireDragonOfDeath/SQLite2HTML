#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //Setup UI
    ui->setupUi(this);

    //Set configfile path
    sConfigFilePath = "./Config/UserSettings.conf";
    sInternalStoreFilaPath = "./Config/InternalStore.conf";

    Config objConfigManager;
    try
    {
        objConfigMemory = objConfigManager.loadConfig(sConfigFilePath);
    }
    catch(...)
    {
        objConfigMemory = new Memory;
    }

    //Puts saved inputs from last session in the fields, if activated
    QString sRememberParameters = objConfigMemory->getValue("RemenberLastParameters");
    if(sRememberParameters == "true")
    {
        try
        {
            objInternalMemory = objConfigManager.loadConfig(sInternalStoreFilaPath);
        }
        catch(...) //Use empty memory if something gone wrong
        {
            objInternalMemory = new Memory;
        }

        //Read saved inputs
        QString sLastDbpath = objInternalMemory->getValue("DatabasePath");
        QString sLastHtmlSavePath = objInternalMemory->getValue("HtmlFilePath");
        QString sLastLinkListSavePath = objInternalMemory->getValue("LinkListPath");
        QString sLastProjectTitle = objInternalMemory->getValue("ProjectTitle");

        //Set saved inputs in the line edits
        ui->leDataBase->setText(sLastDbpath);
        ui->leHtmlSavePath->setText(sLastHtmlSavePath);
        ui->leLinklistSavePath->setText(sLastLinkListSavePath);
        ui->leProjectTitle->setText(sLastProjectTitle);
    }
}

MainWindow::~MainWindow()
{
    //Save user inputs if activated
    QString sRememberParameters = objConfigMemory->getValue("RemenberLastParameters");
    if(sRememberParameters == "true")
    {
        QString sLastDbpath = ui->leDataBase->text();
        QString sLastHtmlSavePath = ui->leHtmlSavePath->text();
        QString sLastLinkListSavePath = ui->leLinklistSavePath->text();
        QString sLastProjectTitle = ui->leProjectTitle->text();

        objInternalMemory->setKeyValueEntry("DatabasePath", sLastDbpath);
        objInternalMemory->setKeyValueEntry("HtmlFilePath", sLastHtmlSavePath);
        objInternalMemory->setKeyValueEntry("LinkListPath", sLastLinkListSavePath);
        objInternalMemory->setKeyValueEntry("ProjectTitle", sLastProjectTitle);

        Config objConfigManager;
        try
        {
            objConfigManager.saveConfig(sInternalStoreFilaPath, objInternalMemory);
        }
        catch(...)
        {
            showErrorMessage("Die eingegebenen Werte konnten nicht gespeichert werden!");
        }
    }

    //Clean up
    delete objDbThread;
    delete objConfigMemory;
    delete objInternalMemory;
    delete ui;
}

void MainWindow::conversionThreadFinished()
{
    ui->btnStartConvert->setEnabled(true);
}

void MainWindow::on_btnSelectDatabase_clicked()
{
    QFileDialog objFileDialog(this); //Define a new file dialog...
    QStringList slSelectedFiles;
    objFileDialog.setAcceptMode(QFileDialog::AcceptOpen); //We want to open files
    objFileDialog.setFileMode(QFileDialog::ExistingFile); //We want to open only one existing file
    if (objFileDialog.exec()) //Open the dialog
    {
        slSelectedFiles = objFileDialog.selectedFiles(); //Put the selected files in the QStringList
        if(slSelectedFiles.size() < 1)
        {
            showErrorMessage("Unbekannter Fehler");
        }
        else
        {
            ui->leDataBase->setText(slSelectedFiles.at(0)); //Put the selected file in the line edit
        }
    }
}

void MainWindow::on_btnSelectHtmlSavePath_clicked()
{
    QFileDialog objFileDialog(this); //Define a new file dialog...
    QStringList slSelectedFiles;
    objFileDialog.setAcceptMode(QFileDialog::AcceptSave); //We want to save a file
    if (objFileDialog.exec())
    {
        slSelectedFiles = objFileDialog.selectedFiles(); //Put the selected files in the QStringList
        if(slSelectedFiles.size() < 1)
        {
            showErrorMessage("Unbekannter Fehler");
        }
        else
        {
            ui->leHtmlSavePath->setText(slSelectedFiles.at(0)); //Put the selected file in the line edit
        }
    }
}

void MainWindow::on_btnSelectLinklistSavePath_clicked()
{
    QFileDialog objFileDialog(this); //Define a new file dialog...
    QStringList slSelectedFiles;
    objFileDialog.setAcceptMode(QFileDialog::AcceptSave); //We want to save a file
    if (objFileDialog.exec())
    {
        slSelectedFiles = objFileDialog.selectedFiles(); //Put the selected files in the QStringList
        if(slSelectedFiles.size() < 1)
        {
            showErrorMessage("Unbekannter Fehler");
        }
        else
        {
            ui->leLinklistSavePath->setText(slSelectedFiles.at(0)); //Put the selected file in the line edit
        }
    }
}

void MainWindow::on_btnStartConvert_clicked()
{
    //If we have a thread object delete it and build a new one
    if(objDbThread != 0x0)
    {
        delete objDbThread;
        objDbThread = 0x0;
    }

    //Read parameters from user interface
    QString sDatabaseFile = ui->leDataBase->text();
    QString sHtmlSavePath = ui->leHtmlSavePath->text();
    QString sLinkListPath = ui->leLinklistSavePath->text();
    QString sLinkText = objConfigMemory->getValue("LinkText");
    QString sProjectTablename = objConfigMemory->getValue("ProjectTableName");
    QString sProjectTitle = ui->leProjectTitle->text();

    //Disables the convert button und resets the state view
    ui->btnStartConvert->setEnabled(false);
    ui->pteState->setPlainText("");

    //Put parameters in the Memory object
    Memory* objParameters = new Memory;
    objParameters->setKeyValueEntry("DatabasePath", sDatabaseFile);
    objParameters->setKeyValueEntry("HtmlFilePath", sHtmlSavePath);
    objParameters->setKeyValueEntry("LinkListPath", sLinkListPath);
    objParameters->setKeyValueEntry("LinkText", sLinkText);
    objParameters->setKeyValueEntry("ProjectTableName", sProjectTablename);
    objParameters->setKeyValueEntry("ProjectTitle", sProjectTitle);

    //Set up parameters and start work
    objDbThread = new DatabaseBackend_Thread(objParameters, this);
    objDbThread->start();
}

void MainWindow::setStatus(QString sStatusMessage)
{
    QString sOldText = ui->pteState->toPlainText();
    QString sNewText = "";

    if(sOldText == "") //If the statusbox is empty
    {
        sNewText = sOldText + sStatusMessage;
    }
    else
    {
        sNewText = sOldText + "\n" + sStatusMessage;
    }

    ui->pteState->setPlainText(sNewText);
}

void MainWindow::showErrorMessage(QString sErrorMessage)
{
    //Display the errormessage with the QErrorMessage object
    QErrorMessage objErrorMessage(this);
    objErrorMessage.showMessage(sErrorMessage);
    objErrorMessage.exec();
}
