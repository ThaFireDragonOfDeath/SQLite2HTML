#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QString>
#include "exceptions.h"
#include "memory.h"

class Config
{
public:
    Config();
    Memory* loadConfig(QString sConfigFilePath);
    void saveConfig(QString sConfigFilePath, Memory* objMemoryToSave);

private:
    bool checkLineValidity(QString sLineContent);
    QString getLineContent(int iTargetLine, QString sTargetString);
    int getLinesCount(QString sTargetString);
    void parseLineToMemory(QString sLineContent, Memory* objTargetMemory);
    QString parseMemoryToLine(int iEntryNumber, Memory* objTargetMemory);
};

#endif // CONFIG_H
