#include "headers/libosmhandler.h"

#include <QString>
#include <QLibrary>
#include <QThread>
#include <QPainter>
#include <QSharedPointer>
#include <QMessageBox>

#include <osmscout/Database.h>
#include <osmscoutmap/MapPainter.h>
#include <osmscoutmapqt/MapPainterQt.h>

LibOsmHandler::LibOsmHandler(QString appPath, QSharedPointer<MapRenderer> renderer)
    :
      appPath(appPath),
      rendererPtr(renderer),
      osmPainterQt(nullptr)
{
    currentMapImage = QPixmap(0,0);

    openDatabase(appPath + "/map-data/us-south-latest");
    openStyles(appPath + "/stylesheets/standard.oss");
}

void LibOsmHandler::openDatabase(QString dbPath)
{
    osmscout::DatabaseParameter _dbParam;

    osmDbRef.reset(new osmscout::Database(_dbParam));
    osmMapServiceRef.reset(new osmscout::MapService(osmDbRef));

    if (!osmDbRef->Open(dbPath.toStdString()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to load osm database from: " + dbPath);
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

void LibOsmHandler::paintWithPainter(QSharedPointer<QPainter> painter)
{
    osmPainterQt.reset(new osmscout::MapPainterQt(osmStyleConfigRef));

    // Map style config settings
    osmMapParameter.SetFontSize(5.0);

    mapLock.lock();
    if (!osmPainterQt->DrawMap(osmProjection, osmMapParameter, osmMapData, painter.get()))
    {
        QMessageBox _msg;
        _msg.setText("Unable to draw map.");
        _msg.exec();
    }
    mapLock.unlock();
}

void LibOsmHandler::renderMap()
{
    QPair<int, int> _canvasSize = rendererPtr->getOpenGLNodeSize();
    QPixmap _canvas(_canvasSize.first, _canvasSize.second);
    QPainter painter(&_canvas);

    paintWithPainter(QSharedPointer<QPainter>(&painter));

    mapImageLock.lock();
    
    currentMapImage = _canvas;
    rendererPtr->updateLayer(MapRenderer::Map, currentMapImage);
    
    mapImageLock.unlock();
}