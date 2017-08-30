#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>

struct DataBaseException
{
    QString sErrorMessage;
};

struct FileException
{
    QString sErrorMessage;
};

#endif // EXCEPTIONS_H
