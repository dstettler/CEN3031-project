#include "headers/libosmhandler.h"
#include <QString>
#include <QLibrary>

#include <osmscout/Database.h>

LibOsmHandler::LibOsmHandler(QString mlp, QString qcp, QString qmp)
    :
      mainLibPath(mlp),
      qtClientPath(qcp),
      qtMapPath(qmp)
{}

