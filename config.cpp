#include "config.h"

Config::Config()
{

}

bool Config::checkLineValidity(QString sLineContent)
{
    QString sFirstChar = sLineContent.left(1); //Get the first character of the line

    if(sLineContent.count("=") != 1 || sFirstChar == " " || sFirstChar == "#") //Return false if this line is a comment or invalid
    {
        return false;
    }
    else
    {
        return true;
    }
}

QString Config::getLineContent(int iTargetLine, QString sTargetString)
{
    //Split the string in elements und return the wanted element
    QStringList slLines = sTargetString.split('\n', QString::SkipEmptyParts);
    QString sLineContent = slLines.at(iTargetLine);
    return sLineContent;
}

int Config::getLinesCount(QString sTargetString)
{
    //Split the string (use newline as split point) and count the elements
    QStringList slLines = sTargetString.split('\n', QString::SkipEmptyParts);
    int iLinesCount = slLines.size();
    return iLinesCount;
}

Memory* Config::loadConfig(QString sConfigFilePath)
{
    //Set config file path and open the file
    QFile objConfigFile(sConfigFilePath);
    bool bIsOpen = objConfigFile.open(QFile::ReadOnly | QFile::Text);

    if(objConfigFile.exists() != true || bIsOpen != true)
    {
        throw FileException{"Konfigurationsdatei konnte nicht gelesen werden"};
    }
    else
    {
        //Create new memory object, insert the keys and values and return the pointer of the memory object
        Memory* objConfigMemory = new Memory;
        QString sConfigContent = objConfigFile.readAll();
        int iLinesCount = getLinesCount(sConfigContent);

        for(int i = 0; i < iLinesCount; i++) //Loop over every line
        {
            QString sCurrentLine = getLineContent(i, sConfigContent); //Read one line of the config file
            parseLineToMemory(sCurrentLine, objConfigMemory); //Put the line into the memory object
        }

        return objConfigMemory;
    }
}

void Config::parseLineToMemory(QString sLineContent, Memory *objTargetMemory)
{
    bool bLineIsValid = checkLineValidity(sLineContent);
    if(bLineIsValid != true) //Skip if this Line is a comment or invalid
    {
        return;
    }
    else
    {
        QStringList slKeyValue = sLineContent.split('=', QString::SkipEmptyParts); //Split the line in key and value

        if(slKeyValue.size() != 2) //Skip if something gone wrong
        {
            return;
        }

        //Reads key and value vom the list
        QString sKey = slKeyValue.at(0);
        QString sValue = slKeyValue.at(1);
        objTargetMemory->setKeyValueEntry(sKey, sValue);
    }
}

QString Config::parseMemoryToLine(int iEntryNumber, Memory *objTargetMemory)
{
    //Read key and value
    QString sKey = objTargetMemory->getKeyAt(iEntryNumber);
    QString sValue = objTargetMemory->getValueAt(iEntryNumber);

    //Put them in one line seperated by a '=' character
    QString sBuildLine = sKey + "=" + sValue;
    bool bLineIsValid = checkLineValidity(sBuildLine);

    if(bLineIsValid != true)
    {
        return ""; //Return empty string if the memory entry is invalid/comment in the config syntax
    }
    else
    {
        return sBuildLine;
    }
}

void Config::saveConfig(QString sConfigFilePath, Memory *objMemoryToSave)
{
    QString sConfigContent = ""; //Will contain the final config text
    int iEntryCount = objMemoryToSave->getEntryCount();

    for(int i = 0; i < iEntryCount; i++) //Loop over every entry in the memory object
    {
        QString sCurrentLine = parseMemoryToLine(i, objMemoryToSave); //Convert the current entry to a config line string
        if(sCurrentLine != "") //Skip if the line is invalid or a comment
        {
            sConfigContent.append(sCurrentLine); //Append the parsed line
            sConfigContent.append("\n"); //Append a newline
        }
    }

    //Set the config filepath and opens the config file or create one
    QFile objSaveConfigFile(sConfigFilePath);
    objSaveConfigFile.open(QFile::ReadWrite | QFile::Text | QFile::Truncate);
    qint64 iBytesWritten = objSaveConfigFile.write(sConfigContent.toUtf8()); //Write out
    if(iBytesWritten == -1) //Checks for success
    {
        throw FileException{"Konfigurationsdatei konnte nicht geschrieben werden"};
    }
}
