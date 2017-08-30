#ifndef MEMORY_H
#define MEMORY_H
#include <QString>
#include <QList>

class Memory
{
public:
    Memory();
    ~Memory();
    bool checkKeyExist(QString sKeyString);
    bool checkValueExist(QString sValueString);
    void clearAll();
    void clearRow(int iRow);
    int findKey(QString sKey);
    int getEntryCount();
    QString getKeyAt(int iRow);
    QString getValue(QString sKeyString);
    QString getValueAt(int iRow);
    void setKeyValueEntry(QString sKeyString, QString sValueString);
    void setKeyValueTable(QString *sKeyStrings, QString *sValueStrings, int iCount);

private:
    QList<QString>* sKeyStore;
    QList<QString>* sValueStore;
};

#endif // MEMORY_H
