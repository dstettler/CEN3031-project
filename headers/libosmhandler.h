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
#include <osmscout/Database.h>
#include <osmscout/BasemapDatabase.h>

#include <osmscoutmap/MapService.h>
#include <osmscoutmap/MapParameter.h>

#include <osmscoutmapqt/MapPainterQt.h>

#define LIBOSMHANDLER_MOVE_CONSTANT 2.0
#define LIBOSMHANDLER_ZOOM_SCALE 15.0

class LibOsmHandler
{
    QString appPath;

    MapRenderer *rendererPtr;

    osmscout::DatabaseRef osmDbRef;
    osmscout::MapServiceRef osmMapServiceRef;
    osmscout::StyleConfigRef osmStyleConfigRef;
    osmscout::BasemapDatabaseRef osmBasemapDb;

    osmscout::MapParameter osmMapParameter;
    osmscout::DatabaseParameter osmDbParam;
    osmscout::AreaSearchParameter osmSearchParameter;

    std::list<osmscout::TileRef> osmTileRefList;

    osmscout::MercatorProjection osmMapProjection;
    osmscout::MapData osmMapData;

    osmscout::MapPainterQt *osmPainterQt;

    void openDatabase(QString dbPath);
    void openStyles(QString stylePath);
    void dbConfig(QString dbPath);
    void loadData();
    void loadBaseMapTiles(std::list<osmscout::GroundTile> &tiles);
    void paintWithPainter(QPainter *painter);

public:
    LibOsmHandler(QString appPath, MapRenderer *renderer);
    ~LibOsmHandler();

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void zoomIn();
    void zoomOut();

    void renderMap();
};

#endif // LIBOSMHANDLER_H
