#include "headers/drawing.h"
#include <QPixmap>
#include <QPainter>

void Drawing::drawTrack(DataBridge* context)
{
    QVector<DataBridge::GeoPoint> trackCoordinates = context->GetTrackCoordinatesVector();
    DataBridge::GeoPoint leftBound = context->GetBoundBoxLeft();
    DataBridge::GeoPoint rightBound = context->GetBoundBoxRight();
    QPainter paint;


    //------------------------Dots----------------------------------//
    for (int i = 0; i < trackCoordinates.size(); i++)
    {
        //Check if in bounds and draw if it is
        if (trackCoordinates[i].x > 0 && trackCoordinates[i].x < rightBound.x &&
            trackCoordinates[i].y > 0 && trackCoordinates[i].y < leftBound.y)
        {
            //convert
            QPair<float,float> convertedCoords = context->LatLonToScreenCoord(trackCoordinates[i].x, trackCoordinates[i].y);

            //draw
            float xCoord = convertedCoords.first;
            float yCoord = convertedCoords.second;
            paint.drawPoint(xCoord, yCoord);
        }
    }



    //------------------------Line----------------------------------//
    //Save previous point variable

    //Boolean (switch thing) to see if the previous point was out of bounds (Default: False, point is out of bound) (False: Out of bounds, True: In Bounds)

    for (int i = 0; i < trackCoordinates.size(); i++)
    {

        //Check in bounds, if within bounds, then draw

        //Convert coords

        //Draw it

        //Set the current point to previous point

    }

}

void Drawing::drawCone(DataBridge* context)
{
    QSharedPointer<QVector<DataBridge::GeoPoint>> coneCoordinates = context->GetConeCoordinatesVector();

    //Previous point
    DataBridge::GeoPoint previousPoint;

    //Boolean switch to see if previous point is in bounds or out of bounds
    bool switchForBounds = true;

    for (int i = 0; i < coneCoordinates->size(); i++)
    {

    }

}

void Drawing::drawWarnings(DataBridge* context)
{

}
