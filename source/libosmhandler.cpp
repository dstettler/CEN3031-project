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
#include <osmscoutmap/MapService.h>
#include <osmscout/BasemapDatabase.h>

LibOsmHandler::LibOsmHandler(QString appPath, MapRenderer *renderer)
    :appPath(appPath),
    rendererPtr(renderer),
    osmPainterQt(nullptr),
    osmDbRef(nullptr),
    osmMapServiceRef(nullptr),
    osmStyleConfigRef(nullptr)
{
    openDatabase(appPath + "/map");
    openStyles(appPath + "/stylesheets/standard.oss");
    dbConfig(appPath + "/map");
}

LibOsmHandler::~LibOsmHandler()
{
    if (osmPainterQt)
        delete osmPainterQt;
}

void LibOsmHandler::openDatabase(QString dbPath)
{    
    osmDbRef = std::make_shared<osmscout::Database>(osmDbParam);

    if (!osmDbRef->Open(dbPath.toStdString()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to load osm database from: " + dbPath);
        _msg.exec();
    }

    osmMapServiceRef = std::make_shared<osmscout::MapService>(osmDbRef);
}

void LibOsmHandler::openStyles(QString stylePath)
{
    osmStyleConfigRef = std::make_shared<osmscout::StyleConfig>(osmDbRef->GetTypeConfig());
    
    if (!osmStyleConfigRef->Load(stylePath.toStdString()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to load osm styles from: " + stylePath);
        _msg.exec();
    }
}

void LibOsmHandler::dbConfig(QString dbPath)
{
    osmMapParameter.SetRenderSeaLand(true);
    osmMapParameter.SetRenderUnknowns(false);
    osmMapParameter.SetRenderBackground(false);
    osmMapParameter.SetRenderContourLines(false);
    osmMapParameter.SetRenderHillShading(false);

    osmMapParameter.SetLabelLineMinCharCount(15);
    osmMapParameter.SetLabelLineMaxCharCount(30);
    osmMapParameter.SetLabelLineFitToArea(true);

    osmscout::GeoCoord _center(31.071, -81.350);
    QPair<int, int> dim = rendererPtr->getOpenGLNodeSize();
    osmMapProjection.Set(_center, osmscout::Magnification(100), 96.0, dim.first, dim.second);

    osmBasemapDb = std::make_shared<osmscout::BasemapDatabase>(osmscout::BasemapDatabaseParameter{});
    if (!osmBasemapDb->Open(dbPath.toStdString()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to load basemap database from: " + dbPath);
        _msg.exec();
    }
}

void LibOsmHandler::loadData()
{
    if (osmPainterQt)
        delete osmPainterQt;

    osmTileRefList.clear();
    osmPainterQt = new osmscout::MapPainterQt(osmStyleConfigRef);

    assert(osmDbRef);
    assert(osmDbRef->IsOpen());
    assert(osmMapServiceRef);
    assert(osmStyleConfigRef);

    osmMapData.ClearDBData();

    osmMapServiceRef->LookupTiles(osmMapProjection, osmTileRefList);
    osmMapServiceRef->LoadMissingTileData(osmSearchParameter, *osmStyleConfigRef, osmTileRefList);
    osmMapServiceRef->AddTileDataToMapData(osmTileRefList, osmMapData);
    osmMapServiceRef->GetGroundTiles(osmMapProjection, osmMapData.groundTiles);

    loadBaseMapTiles(osmMapData.baseMapTiles);
}

void LibOsmHandler::loadBaseMapTiles(std::list<osmscout::GroundTile> &tiles)
{
    if (!osmBasemapDb)
    {
        QMessageBox _msg;
        _msg.setText("No basemap db to load water index from!");
        _msg.exec();
        return;
    }

    osmscout::WaterIndexRef _waterIndex = osmBasemapDb->GetWaterIndex();

    if (!_waterIndex)
    {
        QMessageBox _msg;
        _msg.setText("No water index :(");
        _msg.exec();
        return;
    }
        
    osmscout::GeoBox _boundingBox;
    osmMapProjection.GetDimensions(_boundingBox);
    if (!_waterIndex->GetRegions(_boundingBox, osmMapProjection.GetMagnification(), tiles))
    {
        QMessageBox _msg;
        _msg.setText("Unable to read water regions from base map tiles");
        _msg.exec();
    }
}

void LibOsmHandler::paintWithPainter(QPainter *painter)
{
    if (osmPainterQt->DrawMap(osmMapProjection, osmMapParameter, osmMapData, painter))
    {
        return;
    }
    else
    {
        QMessageBox _msg;
        _msg.setText("Unable to draw map.");
        _msg.exec();
    }
}

void LibOsmHandler::renderMap()
{
    QPair<int, int> _canvasSize = rendererPtr->getOpenGLNodeSize();
    QPixmap _canvas(_canvasSize.first, _canvasSize.second);
    _canvas.fill(Qt::transparent);
    QPainter *painter = new QPainter(&_canvas);

    loadData();

    paintWithPainter(painter);

    delete painter;
    
    rendererPtr->updateLayer(MapRenderer::RenderLayer::Map, _canvas);
}

void LibOsmHandler::moveUp()
{
    osmscout::GeoCoord _newCenter, _oldCenter;
    _oldCenter = osmMapProjection.GetCenter();
    _newCenter.Set(
        _oldCenter.GetLat() + LIBOSMHANDLER_MOVE_CONSTANT,
        _oldCenter.GetLon()
     );

    osmMapProjection.Set(
        _newCenter, 
        osmMapProjection.GetMagnification(), 
        osmMapProjection.GetWidth(), 
        osmMapProjection.GetHeight());
}

void LibOsmHandler::moveDown()
{
    osmscout::GeoCoord _newCenter, _oldCenter;
    _oldCenter = osmMapProjection.GetCenter();
    _newCenter.Set(
        _oldCenter.GetLat() - LIBOSMHANDLER_MOVE_CONSTANT,
        _oldCenter.GetLon()
     );

    osmMapProjection.Set(
        _newCenter, 
        osmMapProjection.GetMagnification(), 
        osmMapProjection.GetWidth(), 
        osmMapProjection.GetHeight());
}

void LibOsmHandler::moveLeft()
{
    osmscout::GeoCoord _newCenter, _oldCenter;
    _oldCenter = osmMapProjection.GetCenter();
    _newCenter.Set(
        _oldCenter.GetLat(),
        _oldCenter.GetLon() - LIBOSMHANDLER_MOVE_CONSTANT
     );

    osmMapProjection.Set(
        _newCenter, 
        osmMapProjection.GetMagnification(), 
        osmMapProjection.GetWidth(), 
        osmMapProjection.GetHeight());
}

void LibOsmHandler::moveRight()
{
    osmscout::GeoCoord _newCenter, _oldCenter;
    _oldCenter = osmMapProjection.GetCenter();
    _newCenter.Set(
        _oldCenter.GetLat(),
        _oldCenter.GetLon() + LIBOSMHANDLER_MOVE_CONSTANT
     );

    osmMapProjection.Set(
        _newCenter, 
        osmMapProjection.GetMagnification(), 
        osmMapProjection.GetWidth(), 
        osmMapProjection.GetHeight());
}

void LibOsmHandler::zoomIn()
{
    osmMapProjection.Set(
        osmMapProjection.GetCenter(),
        osmscout::Magnification(osmMapProjection.GetMagnification().GetMagnification() + LIBOSMHANDLER_ZOOM_SCALE),
        osmMapProjection.GetWidth(),
        osmMapProjection.GetHeight()
    );
}

void LibOsmHandler::zoomOut()
{
    if (osmMapProjection.GetMagnification().GetMagnification() == 0)
        return;

    osmMapProjection.Set(
        osmMapProjection.GetCenter(),
        osmscout::Magnification(osmMapProjection.GetMagnification().GetMagnification() - LIBOSMHANDLER_ZOOM_SCALE),
        osmMapProjection.GetWidth(),
        osmMapProjection.GetHeight()
    );
}

