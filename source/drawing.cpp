#include "headers/drawing.h"
#include <QPixmap>
#include <QPainter>
#include <QtMath>

void Drawing::drawTrack(DataBridge* context)
{
    const QVector<DataBridge::GeoPoint>* trackCoordinates = context->GetTrackCoordinatesVector();
    DataBridge::GeoPoint leftBound = context->GetBoundBoxLeft();
    DataBridge::GeoPoint rightBound = context->GetBoundBoxRight();

    //Set up painting
    auto [canvasX, canvasY] = context->GetMapRendererPtr()->getOpenGLNodeSize();
    QPixmap _canvas(canvasX, canvasY);
    _canvas.fill(Qt::transparent);
    auto _painter = new QPainter(&_canvas);

    //------------------------Dots----------------------------------//
    for (DataBridge::GeoPoint point : *trackCoordinates)
    {
        //Check if in bounds and draw if it is
        if (point.x < rightBound.x && point.x > leftBound.x && point.y > rightBound.y && point.y < leftBound.y)
        {
            //convert
            auto [convertedX, convertedY] = context->LatLonToScreenCoord(point.x, point.y);

            //draw
            int xCoord = (int) qFabs(convertedX);
            int yCoord = (int) qFabs(convertedY);

            _painter->setBrush(Qt::blue);
            _painter->drawEllipse(QPointF(xCoord, yCoord), 5, 5);

        }
    }

    //------------------------Line----------------------------------//

    //Save previous point variable
    QPair<int, int> previousPoint;
    bool previousPointInBounds = false;

    for (DataBridge::GeoPoint point : *trackCoordinates)
    {

            //Check in bounds, if within bounds, then draw
            if (point.x < rightBound.x && point.x > leftBound.x && point.y > rightBound.y && point.y < leftBound.y)
            {

                //Convert coords
                QPair<int, int> currentPoint = context->LatLonToScreenCoord(point.x,point.y);
                currentPoint.first = qFabs(currentPoint.first);
                currentPoint.second = qFabs(currentPoint.second);

                if (previousPointInBounds)
                {
                    //Draw it
                    _painter->setPen(Qt::blue);
                    _painter->drawLine(previousPoint.first, previousPoint.second, currentPoint.first, currentPoint.second);

                }

                //Set the current point to previous point and previousPointsInBounds
                previousPoint = currentPoint;
                previousPointInBounds = true;
            }
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
    auto[canvasX, canvasY] = context->GetMapRendererPtr()->getOpenGLNodeSize();
    QPixmap _canvas(canvasX, canvasY);
    _canvas.fill(Qt::transparent);
    auto _painter = new QPainter(&_canvas);

    //------------------------Line----------------------------------//
    //Save previous point variable
    QPair<int, int> previousPoint;
    //Boolean (switch thing) to see if the previous point was out of bounds (Default: False, point is out of bound) (False: Out of bounds, True: In Bounds)
    bool previousPointInBounds = false;


    for (DataBridge::GeoPoint point : *coneCoordinates)
    {

        //Check in bounds, if within bounds, then draw
        if (point.x < rightBound.x && point.x > leftBound.x && point.y > rightBound.y && point.y < leftBound.y)
        {
            //convert
            QPair<int,int> currentPoint = context->LatLonToScreenCoord(point.x, point.y);
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
    //QVector<DataBridge::GeoPoint>*warningsCoordinates = context->GetWarningsCoordinatesVector();
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

