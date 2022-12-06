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
    rendererPtr(renderer)
{
    openDatabase(appPath + "/map");
    openStyles(appPath + "/stylesheets/standard.oss");
    dbConfig(appPath + "/map");
}

void LibOsmHandler::openDatabase(QString dbPath)
{    
    osmDbRef.reset(new osmscout::Database(osmDbParam));

    if (!osmDbRef->Open(dbPath.toStdString()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to load osm database from: " + dbPath);
        _msg.exec();
    }

    osmMapServiceRef.reset(new osmscout::MapService(osmDbRef));
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

void LibOsmHandler::dbConfig(QString dbPath)
{
    osmMapParameter.SetRenderSeaLand(true);
    osmMapParameter.SetRenderUnknowns(false);
    osmMapParameter.SetRenderBackground(false);
    osmMapParameter.SetRenderContourLines(false);
    osmMapParameter.SetRenderHillShading(false);

    osmscout::GeoCoord _center(LIBOSMHANDLER_DEFAULT_LAT, LIBOSMHANDLER_DEFAULT_LON);
    QPair<int, int> dim = rendererPtr->getOpenGLNodeSize();
    
    // Not sure why the dpi of 96.0 works the best here but that seems to be the only way to get the map to
    // draw properly
    // DO NOT use the .Set() function without specifying dpi or you *will* face the wrath of the weird drawing bug gods
    osmMapProjection.Set(_center, osmscout::Magnification(LIBOSMHANDLER_DEFAULT_MAG), 96.0, dim.first, dim.second);

    osmBasemapDb.reset(new osmscout::BasemapDatabase(osmscout::BasemapDatabaseParameter{}));
    if (!osmBasemapDb->Open(dbPath.toStdString()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to load basemap database from: " + dbPath);
        _msg.exec();
    }
}

void LibOsmHandler::loadData()
{
    osmTileRefList.clear();
    osmPainterQt.reset(new osmscout::MapPainterQt(osmStyleConfigRef));

    osmMapData.ClearDBData();

    osmMapServiceRef->LookupTiles(osmMapProjection, osmTileRefList);
    osmMapServiceRef->LoadMissingTileData(osmSearchParameter, *osmStyleConfigRef, osmTileRefList);
    osmMapServiceRef->AddTileDataToMapData(osmTileRefList, osmMapData);
    osmMapServiceRef->GetGroundTiles(osmMapProjection, osmMapData.groundTiles);

    loadBaseMapTiles(osmMapData.baseMapTiles);
}

void LibOsmHandler::loadBaseMapTiles(std::list<osmscout::GroundTile> &tiles) const
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
    auto painter = new QPainter(&_canvas);

    // Data must be loaded here to reload the tiles within the current projection view before drawing
    loadData();

    paintWithPainter(painter);

    delete painter;
    
    rendererPtr->updateLayer(MapRenderer::RenderLayer::Map, _canvas);
}

QPair<double, double> LibOsmHandler::getTopLeft()
{
    QPair<double, double> _coords;
    
    _coords.first = osmMapProjection.GetDimensions().GetTopLeft().GetLat();
    _coords.second = osmMapProjection.GetDimensions().GetTopLeft().GetLon();

    return _coords;
}

QPair<double, double> LibOsmHandler::getBottomRight()
{
    QPair<double, double> _coords;
    
    QPair<double, double> _topLeft = getTopLeft();

    // Get difference between centerpoint and top left, and add that distance to get the bottom corner
    // (for some reason GetDimensions().GetBottomRight() seems to buffer much farther than needed)
    _coords.first = (2 * osmMapProjection.GetCenter().GetLat()) - _topLeft.first;
    _coords.second = (2 * osmMapProjection.GetCenter().GetLon()) - _topLeft.second;

    return _coords;
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

