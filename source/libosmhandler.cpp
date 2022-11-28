#include "headers/libosmhandler.h"
#include <QString>
#include <QLibrary>

#include <osmscout/Database.h>
#include <osmscoutmap/MapPainter.h>
#include <osmscoutmapqt/MapPainterQt.h>

LibOsmHandler::LibOsmHandler(QString appPath, QSharedPointer<MapRenderer> renderer)
    :
      appPath(appPath),
      renderer(renderer)
{}

