#ifndef LIBOSMHANDLER_H
#define LIBOSMHANDLER_H

#include "maprenderer.h"

#include <QString>
#include <QSharedPointer>
#include <QPixmap>

#include <osmscout/util/GeoBox.h>
#include <osmscout/util/Projection.h>

class LibOsmHandler
{
    QString appPath;

    QSharedPointer<MapRenderer> renderer;
    QPixmap currentImage;

    osmscout::GeoBox osmBoundingBox;
    osmscout::MercatorProjection osmProjection;

public:
    LibOsmHandler(QString appPath, QSharedPointer<MapRenderer> renderer);
};

#endif // LIBOSMHANDLER_H
