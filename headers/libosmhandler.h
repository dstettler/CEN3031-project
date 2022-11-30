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

class LibOsmHandler
{
    std::string appPath;
    unsigned int moveAmount;

    MapRenderer *rendererPtr;
    QPixmap currentMapImage;

    osmscout::Database *osmDbRef;
    osmscout::MapService *osmMapServiceRef;
    osmscout::StyleConfigRef osmStyleConfigRef;
    osmscout::BasemapDatabaseRef osmBasemapDb;

    osmscout::GeoBox osmBoundingBox;
    //osmscout::Magnification osmMagnificationLevel;
    osmscout::MercatorProjection osmMapProjection;
    //osmscout::MapParameter osmMapParameter;
    osmscout::MapData osmMapData;
    osmscout::AreaSearchParameter osmSearchParameter;
    
    QMutex mapLock;
    QMutex mapImageLock;

    osmscout::MapPainterQt *osmPainterQt;

    QList<osmscout::TileRef>  _tiles;
	//QVector<osmscout::GeoCoord> _lastRoute;

    void openDatabase(QString dbPath);
    void openStyles(QString stylePath);
    void loadData();
    void paintWithPainter(QPainter *painter);

public:
    LibOsmHandler(std::string appPath, MapRenderer *renderer);
    ~LibOsmHandler();

    moveLeft();
    moveRight();
    moveUp();
    moveDown();

    void renderMap();
};

#endif // LIBOSMHANDLER_H
