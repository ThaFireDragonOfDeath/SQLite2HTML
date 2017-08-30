#include "memory.h"

Memory::Memory() //Class Memory: Saves string-data into RAM
{
    sKeyStore = new QList<QString>;
    sValueStore = new QList<QString>;
}

Memory::~Memory()
{
    delete sKeyStore;
    delete sValueStore;
}

bool Memory::checkKeyExist(QString sKeyString)
{
    bool bElementExist = sKeyStore->contains(sKeyString);
    return bElementExist;
}

bool Memory::checkValueExist(QString sValueString)
{
    bool bElementExist = sValueStore->contains(sValueString);
    return bElementExist;
}

void Memory::clearRow(int iRow) //Deletes one row from the list
{
    sKeyStore->removeAt(iRow);
    sValueStore->removeAt(iRow);
}

void Memory::clearAll() //Deletes both lists
{
    sValueStore->clear();
    sKeyStore->clear();
}

int Memory::findKey(QString sKey)
{
    int iKeyPos = sKeyStore->indexOf(sKey);
    return iKeyPos;
}

int Memory::getEntryCount()
{
    int iCount = sKeyStore->count();
    return iCount;
}

QString Memory::getValue(QString sKeyString)
{
    int iFoundRow = sKeyStore->indexOf(sKeyString);

    if(iFoundRow == -1)
    {
        return "";
    }
    else
    {
        QString sFoundString = sValueStore->at(iFoundRow);
        return sFoundString;
    }
}

QString Memory::getKeyAt(int iRow)
{
    QString sFileString = sKeyStore->at(iRow);
    return sFileString;
}

QString Memory::getValueAt(int iRow)
{
    QString sCompareString = sValueStore->at(iRow);
    return sCompareString;
}

void Memory::setKeyValueEntry(QString sKeyString, QString sValueString)
{
    if(sKeyString == "") //Skip, if the key is empty
    {
        return;
    }

    int iOldKeyPosition = findKey(sKeyString);
    if(iOldKeyPosition == -1)
    {
        sKeyStore->append(sKeyString);
        sValueStore->append(sValueString);
    }
    else
    {
        sValueStore->replace(iOldKeyPosition, sValueString);
    }
}

void Memory::setKeyValueTable(QString *sKeyStrings, QString *sValueStrings, int iCount) //Saves Arrays in memory
{
    sValueStore->clear();
    sKeyStore->clear();

    int i1;
    for(i1=0; i1<iCount; i1++)
    {
        sKeyStore->insert(i1, sKeyStrings[i1]);
    }

    int i2;
    for(i2=0; i2<iCount; i2++)
    {
        sValueStore->insert(i2, sValueStrings[i2]);
    }
}
