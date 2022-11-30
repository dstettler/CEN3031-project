#include "headers/libosmhandler.h"

#include <QString>
#include <QLibrary>
#include <QThread>
#include <QPainter>
#include <QSharedPointer>
#include <QMessageBox>
#include <QFile>

#include <osmscout/Database.h>
#include <osmscoutmap/MapPainter.h>
#include <osmscoutmapqt/MapPainterQt.h>
#include <osmscoutmap/StyleConfig.h>
#include <osmscoutmap/MapParameter.h>

LibOsmHandler::LibOsmHandler(std::string appPath, MapRenderer *renderer)
    :appPath(appPath),
    rendererPtr(renderer),
    osmPainterQt(nullptr),
    osmDbRef(nullptr),
    osmMapServiceRef(nullptr),
    osmStyleConfigRef(nullptr),
    // bbox=-87.891,23.262,-70.642,31.747
    osmBoundingBox(osmscout::GeoCoord(-87.891, 19.262), osmscout::GeoCoord(-70.642, 31.747)),
    moveAmount(50)
{
    QString _appPath = QString::fromStdString(appPath);

    currentMapImage = QPixmap(0,0);

    openDatabase(_appPath + "/map-data/us-south-latest");
    openStyles(_appPath + "/stylesheets/standard.oss");

    osmscout::GeoCoord _center(30.673, -82.724);
    QPair<int, int> dim = rendererPtr->getOpenGLNodeSize();
    osmMapProjection.Set(_center, osmscout::Magnification(35), dim.first, dim.second);

    loadData();
}

LibOsmHandler::~LibOsmHandler()
{
    if (osmPainterQt)
        delete osmPainterQt;

    if (osmDbRef)
        delete osmDbRef;
    
    if (osmMapServiceRef)
        delete osmMapServiceRef;
}

void LibOsmHandler::openDatabase(QString dbPath)
{
    osmscout::DatabaseParameter _dbParam;

    if (osmDbRef)
        delete osmDbRef;
    osmDbRef = new osmscout::Database(_dbParam);
    
    if (osmMapServiceRef)
        delete osmMapServiceRef;

    osmMapServiceRef = new osmscout::MapService(osmscout::DatabaseRef(osmDbRef));

    if (!osmDbRef->Open(dbPath.toStdString()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to load osm database from: " + dbPath);
        _msg.exec();
    }

    osmBasemapDb.reset(new osmscout::BasemapDatabase(osmscout::BasemapDatabaseParameter{}));
    if (!osmBasemapDb->Open(dbPath.toStdString()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to load basemap database from: " + dbPath);
        _msg.exec();
    }
}

void LibOsmHandler::openStyles(QString stylePath)
{
    osmStyleConfigRef.reset(new osmscout::StyleConfig(osmDbRef->GetTypeConfig()));
    

    if (!osmStyleConfigRef->Load(stylePath.toStdString()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to load osm styles from: " + stylePath);
        _msg.exec();
    }
}

void LibOsmHandler::loadData()
{
    std::list<osmscout::TileRef> _tiles;
    osmMapData.ClearDBData();

    osmMapServiceRef->LookupTiles(osmMapProjection, _tiles);
    osmMapServiceRef->LoadMissingTileData(osmSearchParameter, *osmStyleConfigRef, _tiles);
    osmMapServiceRef->AddTileDataToMapData(_tiles, osmMapData);
    osmMapServiceRef->GetGroundTiles(osmMapProjection, osmMapData.groundTiles);

    osmscout::WaterIndexRef _waterIndex = osmBasemapDb->GetWaterIndex();

    osmscout::GeoBox _boundingBox;
    osmMapProjection.GetDimensions(_boundingBox);
    if (!_waterIndex->GetRegions(_boundingBox, osmMapProjection.GetMagnification(), osmMapData.baseMapTiles))
    {
        QMessageBox _msg;
        _msg.setText("Unable to read water regions from base map tiles");
        _msg.exec();
    }
}

void LibOsmHandler::paintWithPainter(QPainter *painter)
{
    if (osmPainterQt)
        delete osmPainterQt;

    osmPainterQt = new osmscout::MapPainterQt(osmStyleConfigRef);

    mapLock.lock();   

    osmscout::MapParameter osmMapParameter;

    if (!osmPainterQt->DrawMap(osmMapProjection, osmMapParameter, osmMapData, painter))
    {
        QMessageBox _msg;
        _msg.setText("Unable to draw map.");
        _msg.exec();
    }

    /*QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(5);
    painter->setPen(pen);
    painter->drawRect(10,10,500,500);*/

    mapLock.unlock();
}

void LibOsmHandler::renderMap()
{
    QPair<int, int> _canvasSize = rendererPtr->getOpenGLNodeSize();
    QPixmap _canvas(_canvasSize.first, _canvasSize.second);
    QPainter painter(&_canvas);

    paintWithPainter(&painter);

    mapImageLock.lock();

    currentMapImage = _canvas;
    rendererPtr->updateLayer(MapRenderer::RenderLayer::Map ,currentMapImage);

    mapImageLock.unlock();
}