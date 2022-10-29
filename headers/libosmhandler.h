#ifndef LIBOSMHANDLER_H
#define LIBOSMHANDLER_H

#include <QString>

class LibOsmHandler
{
    QString mainLibPath;
    QString qtClientPath;
    QString qtMapPath;


public:
    LibOsmHandler(QString mlp, QString qcp, QString qmp);
};

#endif // LIBOSMHANDLER_H
