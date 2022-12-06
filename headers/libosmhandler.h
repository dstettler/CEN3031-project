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
#include <QPair>

#include <osmscout/util/GeoBox.h>
#include <osmscout/util/Projection.h>
#include <osmscout/Database.h>
#include <osmscout/BasemapDatabase.h>

#include <osmscoutmap/MapService.h>
#include <osmscoutmap/MapParameter.h>

#include <osmscoutmapqt/MapPainterQt.h>

constexpr double LIBOSMHANDLER_MOVE_CONSTANT = 2.0;
constexpr double LIBOSMHANDLER_ZOOM_SCALE = 15.0;

constexpr double LIBOSMHANDLER_DEFAULT_LAT = 29.071;
constexpr double LIBOSMHANDLER_DEFAULT_LON = -81.35;
constexpr double LIBOSMHANDLER_DEFAULT_MAG = 70.0;

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

    QSharedPointer<osmscout::MapPainterQt> osmPainterQt;

    void openDatabase(QString dbPath);
    void openStyles(QString stylePath);
    void dbConfig(QString dbPath);
    void loadData();
    void loadBaseMapTiles(std::list<osmscout::GroundTile> &tiles) const;
    void paintWithPainter(QPainter *painter);

public:
    LibOsmHandler(QString appPath, MapRenderer *renderer);

    QPair<double, double> getTopLeft();
    QPair<double, double> getBottomRight();

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void zoomIn();
    void zoomOut();

    void renderMap();
};

#endif // LIBOSMHANDLER_H
