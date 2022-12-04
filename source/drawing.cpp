#include "headers/drawing.h"
#include <QPixmap>
#include <QPainter>
#include <QtMath>

void Drawing::drawTrack(DataBridge* context)
{
    QVector<DataBridge::GeoPoint>* trackCoordinates = context->GetTrackCoordinatesVector();
    DataBridge::GeoPoint leftBound = context->GetBoundBoxLeft();
    DataBridge::GeoPoint rightBound = context->GetBoundBoxRight();

    //Set up painting
    QPair<int,int> _canvasSize = context->GetMapRendererPtr()->getOpenGLNodeSize();
    QPixmap _canvas(_canvasSize.first, _canvasSize.second);
    _canvas.fill(Qt::transparent);
    QPainter* _painter = new QPainter(&_canvas);

    //------------------------Dots----------------------------------//
    for (int i = 0; i < trackCoordinates->size(); i++)
    {
        //Check if in bounds and draw if it is
        if (trackCoordinates->at(i).x > 0 && trackCoordinates->at(i).x < rightBound.x &&
            trackCoordinates->at(i).y > 0 && trackCoordinates->at(i).y < leftBound.y)
        {
            //convert
            QPair<int,int> convertedCoords = context->LatLonToScreenCoord(trackCoordinates->at(i).x, trackCoordinates->at(i).y);

            //draw
            int xCoord = convertedCoords.first;
            int yCoord = convertedCoords.second;

            _painter->setPen(Qt::white);
            _painter->drawPoint(xCoord, yCoord);
        }
    }



    //------------------------Line----------------------------------//
    //Save previous point variable

    //Boolean (switch thing) to see if the previous point was out of bounds (Default: False, point is out of bound) (False: Out of bounds, True: In Bounds)

    for (int i = 0; i < trackCoordinates->size(); i++)
    {

        //Check in bounds, if within bounds, then draw

        //Convert coords

        //Draw it

        //Set the current point to previous point

    }

    //Destructor
    delete _painter;

    context->GetMapRendererPtr()->updateLayer(MapRenderer::RenderLayer::Track, _canvas);
}

void Drawing::drawCone(DataBridge* context)
{
    QVector<DataBridge::GeoPoint>*coneCoordinates = context->GetConeCoordinatesVector();
    DataBridge::GeoPoint leftBound = context->GetBoundBoxLeft();
    DataBridge::GeoPoint rightBound = context->GetBoundBoxRight();

    //Set up painting
    QPair<int,int> _canvasSize = context->GetMapRendererPtr()->getOpenGLNodeSize();
    QPixmap _canvas(_canvasSize.first, _canvasSize.second);
    _canvas.fill(Qt::transparent);
    QPainter* _painter = new QPainter(&_canvas);

    //------------------------Dots----------------------------------//
    for (int i = 0; i < coneCoordinates->size(); i++)
    {
        //Check if in bounds and draw if it is
        if (coneCoordinates->at(i).x < rightBound.x && coneCoordinates->at(i).x > leftBound.x && coneCoordinates->at(i).y > rightBound.y && coneCoordinates->at(i).y < leftBound.y)
        {
            //convert
            QPair<int,int> convertedCoords = context->LatLonToScreenCoord(coneCoordinates->at(i).x, coneCoordinates->at(i).y);

            //draw
            int xCoord = convertedCoords.first;
            int yCoord = convertedCoords.second;

            xCoord = qFabs(xCoord);
            yCoord = qFabs(yCoord);

            _painter->setPen(Qt::white);
            _painter->drawPoint(xCoord, yCoord);
        }
    }

    //------------------------Line----------------------------------//
    //Save previous point variable

    //Boolean (switch thing) to see if the previous point was out of bounds (Default: False, point is out of bound) (False: Out of bounds, True: In Bounds)

    for (int i = 0; i < coneCoordinates->size(); i++)
    {

        //Check in bounds, if within bounds, then draw

        //Convert coords

        //Draw it

        //Set the current point to previous point

    }

    //Destructor
    delete _painter;

    context->GetMapRendererPtr()->updateLayer(MapRenderer::RenderLayer::Cone, _canvas);
}

void Drawing::drawWarnings(DataBridge* context)
{
    QVector<DataBridge::GeoPoint>*warningsCoordinates = context->GetWarningsCoordinatesVector();
    DataBridge::GeoPoint leftBound = context->GetBoundBoxLeft();
    DataBridge::GeoPoint rightBound = context->GetBoundBoxRight();

    //Set up painting
    QPair<int,int> _canvasSize = context->GetMapRendererPtr()->getOpenGLNodeSize();
    QPixmap _canvas(_canvasSize.first, _canvasSize.second);
    _canvas.fill(Qt::transparent);
    QPainter* _painter = new QPainter(&_canvas);

    //------------------------Dots----------------------------------//
    for (int i = 0; i < warningsCoordinates->size(); i++)
    {
        //Check if in bounds and draw if it is
        if (warningsCoordinates->at(i).x < rightBound.x && warningsCoordinates->at(i).x > leftBound.x && warningsCoordinates->at(i).y > rightBound.y && warningsCoordinates->at(i).y < leftBound.y)
        {
            //convert
            QPair<int,int> convertedCoords = context->LatLonToScreenCoord(warningsCoordinates->at(i).x, warningsCoordinates->at(i).y);

            //draw
            int xCoord = convertedCoords.first;
            int yCoord = convertedCoords.second;

            xCoord = qFabs(xCoord);
            yCoord = qFabs(yCoord);

            _painter->setPen(Qt::yellow);
            _painter->drawPoint(xCoord, yCoord);
        }
    }

    //------------------------Line----------------------------------//
    //Save previous point variable

    //Boolean (switch thing) to see if the previous point was out of bounds (Default: False, point is out of bound) (False: Out of bounds, True: In Bounds)
    //lines should probably use the warningsData vector so that each warning area (in each placemark) has separate lines
    for (int i = 0; i < warningsCoordinates->size(); i++)
    {

        //Check in bounds, if within bounds, then draw

        //Convert coords

        //Draw it

        //Set the current point to previous point

    }

    //Destructor
    delete _painter;

    context->GetMapRendererPtr()->updateLayer(MapRenderer::RenderLayer::Warnings, _canvas);
}

