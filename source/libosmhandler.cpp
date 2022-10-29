#include "headers/libosmhandler.h"
#include <QString>
#include <QLibrary>

LibOsmHandler::LibOsmHandler(QString mlp, QString qcp, QString qmp)
{
    this->mainLibPath = mlp;
    this->qtClientPath = qcp;
    this->qtMapPath = qmp;
}
