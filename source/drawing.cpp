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
    //commented out Dots because the lines just paint over them
    /*for (int i = 0; i < coneCoordinates->size(); i++)
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
    }*/

    //------------------------Line----------------------------------//
    //Save previous point variable
    QPair<int, int> previousPoint;
    //Boolean (switch thing) to see if the previous point was out of bounds (Default: False, point is out of bound) (False: Out of bounds, True: In Bounds)
    bool previousPointInBounds = false;


    for (int i = 0; i < coneCoordinates->size(); i++)
    {

        //Check in bounds, if within bounds, then draw
        if (coneCoordinates->at(i).x < rightBound.x && coneCoordinates->at(i).x > leftBound.x && coneCoordinates->at(i).y > rightBound.y && coneCoordinates->at(i).y < leftBound.y)
        {
            //convert
            QPair<int,int> currentPoint = context->LatLonToScreenCoord(coneCoordinates->at(i).x, coneCoordinates->at(i).y);
            currentPoint.first = qFabs(currentPoint.first);
            currentPoint.second = qFabs(currentPoint.second);

            if (previousPointInBounds)
            {
                //Draw it
                _painter->setPen(QPen(Qt::white, 3)); //the number is thickness in pixels
                _painter->drawLine(previousPoint.first, previousPoint.second, currentPoint.first, currentPoint.second);
            }
            //Set the current point to previous point and previousPointsInBounds
            previousPoint = currentPoint;
            previousPointInBounds = true;
        }
        else    //this ensures that the last point before going out of bounds doesnt connect to the first point after coming back. coz we dont want that.
            previousPointInBounds = false;
    }

    //Destructor
    delete _painter;

    context->GetMapRendererPtr()->updateLayer(MapRenderer::RenderLayer::Cone, _canvas);
}

void Drawing::drawWarnings(DataBridge* context)
{
    QVector<DataBridge::GeoPoint>*warningsCoordinates = context->GetWarningsCoordinatesVector();
    QVector<DataBridge::WarningsPlacemark>* warningsData = context->GetWarningsDataVector();
    DataBridge::GeoPoint leftBound = context->GetBoundBoxLeft();
    DataBridge::GeoPoint rightBound = context->GetBoundBoxRight();

    //Set up painting
    QPair<int,int> _canvasSize = context->GetMapRendererPtr()->getOpenGLNodeSize();
    QPixmap _canvas(_canvasSize.first, _canvasSize.second);
    _canvas.fill(Qt::transparent);
    QPainter* _painter = new QPainter(&_canvas);

    //no dots needed
    //------------------------Line----------------------------------//
    //Save previous point variable

    //Boolean (switch thing) to see if the previous point was out of bounds (Default: False, point is out of bound) (False: Out of bounds, True: In Bounds)
    //lines use the warningsData vector so that each warning area (in each placemark) has separate lines. Drawing dots above is also a bit redundant since the lines go over them.
    for (int i = 0; i < warningsData->size(); i++)
    {   //warningsData stores placemarks w the warning name and a vector of coordinates. This checks if the coordinates are for a watch or warning
        if (warningsData->at(i).warningName.contains("Warning"))
            _painter->setPen(QPen(Qt::red, 3));
        else if (warningsData->at(i).warningName.contains("Watch"))
            _painter->setPen(QPen(Qt::yellow, 3));  //the number is thickness in pixels

        //Save previous point variable
        QPair<int, int> previousPoint;
        bool previousPointInBounds = false;

        //getting the coordinates vector of each placemark.
        for (int j = 0; j < warningsData->at(i).warningsCoordinatesVector.size(); j++)
        {
            //Check in bounds, if within bounds, then draw
            if (warningsData->at(i).warningsCoordinatesVector[j].x < rightBound.x && warningsData->at(i).warningsCoordinatesVector[j].x > leftBound.x && warningsData->at(i).warningsCoordinatesVector[j].y > rightBound.y && warningsData->at(i).warningsCoordinatesVector[j].y < leftBound.y)
            {
                //Convert coords
                QPair<int, int> currentPoint = context->LatLonToScreenCoord(warningsData->at(i).warningsCoordinatesVector[j].x,warningsData->at(i).warningsCoordinatesVector[j].y);
                currentPoint.first = qFabs(currentPoint.first);
                currentPoint.second = qFabs(currentPoint.second);

                if (previousPointInBounds)
                {
                    //Draw it
                    _painter->drawLine(previousPoint.first, previousPoint.second, currentPoint.first, currentPoint.second);
                }
                //Set the current point to previous point and previousPointsInBounds
                previousPoint = currentPoint;
                previousPointInBounds = true;
            }
            else
                previousPointInBounds = false;
       }
    }

    //Destructor
    delete _painter;

    context->GetMapRendererPtr()->updateLayer(MapRenderer::RenderLayer::Warnings, _canvas);
}

