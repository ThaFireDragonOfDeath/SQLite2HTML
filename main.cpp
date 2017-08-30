#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QString>

bool checkConfigEntry(QString sKey, QString sDefault, Memory* objTargetMemory)
{
    //Checks if config entry is set and use default value if not
    QString sTestValue = objTargetMemory->getValue(sKey);
    if(sTestValue == "")
    {
        objTargetMemory->setKeyValueEntry(sKey, sDefault);
        return true;
    }
    return false;
}

bool checkConfigs()
{
    QString sUserSettingsFilePath = "./Config/UserSettings.conf";
    Config objConfigManager;
    Memory* objUserSettingsMemory = 0x0;

    try
    {
        objUserSettingsMemory = objConfigManager.loadConfig(sUserSettingsFilePath);
    }
    catch(...)
    {
        delete objUserSettingsMemory;
        return false;
    }

    //Check config entrys
    bool bCheckLT = checkConfigEntry("LinkText", "Link", objUserSettingsMemory);
    bool bCheckPT = checkConfigEntry("ProjectTableName", "VideoEntrys", objUserSettingsMemory);
    bool bCheckRLP = checkConfigEntry("RemenberLastParameters", "true", objUserSettingsMemory);

    //Write out if modified
    if
    (bCheckLT || bCheckPT || bCheckRLP)
    {
        try
        {
            objConfigManager.saveConfig(sUserSettingsFilePath, objUserSettingsMemory);
        }
        catch(...)
        {
            delete objUserSettingsMemory;
            return false;
        }
    }

    delete objUserSettingsMemory;
    return true;
}

bool setupDir(QString sDirName)
{
    //Checks if the target folder exist and create it, if doesn't exist
    QString sDirPath = "./" + sDirName;
    QDir objTargetDir(sDirPath);
    bool bTargetDirExist = objTargetDir.exists();
    bool bCreateTargetDirSuccess = false;

    if(bTargetDirExist != true)
    {
        objTargetDir = QDir::current();
        bCreateTargetDirSuccess = objTargetDir.mkdir(sDirName);
        return bCreateTargetDirSuccess;
    }

    return true;
}

bool setupFile(QString sDirName, QString sFileName)
{
    //Checks if the target file exist and creates the file if not
    QString sSystemFilePath;
    QString sRessourcesFilePath;

    if(sDirName == "")
    {
        sSystemFilePath = "./" + sFileName;
        sRessourcesFilePath = ":/" + sFileName;
    }
    else
    {
        sSystemFilePath = "./" + sDirName + "/" + sFileName;
        sRessourcesFilePath = ":/" + sDirName + "/" + sFileName;
    }

    QFile objTargetFile(sSystemFilePath);

    //If the file doesn't exist, copy the file from the ressources to the target dir
    if(objTargetFile.exists() != true)
    {
        QFile objDefaultTargetFile(sRessourcesFilePath);
        bool bCopySuccess = objDefaultTargetFile.copy(sSystemFilePath);
        return bCopySuccess;
    }

    return true;
}

bool setupPath()
{
    //Setup files and folders
    bool bSetupConfigDir = setupDir("Config");
    bool bSetupTemplateDir = setupDir("Template");
    bool bSetupUserSettingsFile = setupFile("Config", "UserSettings.conf");
    bool bSetupTemplateFile = setupFile("Template", "Template.html");

    //Return false if failed
    if(!bSetupConfigDir || !bSetupTemplateDir || !bSetupTemplateFile || !bSetupUserSettingsFile)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void showStartError(QString sErrorMessage)
{
    QErrorMessage objErrorMessage;
    objErrorMessage.showMessage(sErrorMessage);
    objErrorMessage.exec();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool bSetupSuccess = setupPath(); //Check and create required dirs
    if(bSetupSuccess != true)
    {
        showStartError("Programm kann nicht gestartet werden, da benötigte Dateien und Ordner nicht vorhanden sind und zudem auch nicht angelegt werden konnten!");
        return 1;
    }

    bool bConfigTest = checkConfigs();
    if(bConfigTest != true)
    {
        showStartError("Fehler beim Überprüfen der Config Dateien!");
        return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
