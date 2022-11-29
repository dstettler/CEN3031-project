#ifndef LIBOSMHANDLER_H
#define LIBOSMHANDLER_H

#include "maprenderer.h"

#include <QString>
#include <QSharedPointer>
#include <QPixmap>
#include <QList>
#include <QVector>
#include <QMutex>
#include <QThread>

#include <osmscout/util/GeoBox.h>
#include <osmscout/util/Projection.h>
#include <osmscoutmap/MapParameter.h>
#include <osmscout/Database.h>

#include <osmscoutmap/MapService.h>

#include <osmscoutmapqt/MapPainterQt.h>

class LibOsmHandler
{
    QString appPath;

    QSharedPointer<QThread> renderingThread;
    QSharedPointer<MapRenderer> rendererPtr;
    QPixmap currentMapImage;

    osmscout::DatabaseRef osmDbRef;
    osmscout::MapServiceRef osmMapServiceRef;
    osmscout::StyleConfigRef osmStyleConfigRef;

    osmscout::GeoBox osmBoundingBox;
    osmscout::Magnification osmMagnificationLevel;
    osmscout::MercatorProjection osmProjection;
    osmscout::MapParameter osmMapParameter;
    osmscout::MapData osmMapData;
    
    QMutex mapLock;
    QMutex mapImageLock;

    QSharedPointer<osmscout::MapPainterQt> osmPainterQt;

    QList<osmscout::TileRef>  _tiles;
	QVector<osmscout::GeoCoord> _lastRoute;

    void openDatabase(QString dbPath);
    void openStyles(QString stylePath);
    void paintWithPainter(QSharedPointer<QPainter> painter);

public:
    LibOsmHandler(QString appPath, QSharedPointer<MapRenderer> renderer);

    void renderMap();
};

#endif // LIBOSMHANDLER_H
