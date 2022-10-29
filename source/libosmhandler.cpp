#include "headers/libosmhandler.h"
#include <QString>
#include <QLibrary>

LibOsmHandler::LibOsmHandler(QString mlp, QString qcp, QString qmp)
    :
      mainLibPath(mlp),
      qtClientPath(qcp),
      qtMapPath(qmp)
{}

