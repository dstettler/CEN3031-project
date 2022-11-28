#include "headers/drawing.h"
#include <QPixmap>
#include <QPainter>

void Drawing::drawTrack(QSharedPointer<DataBridge>context)
{
    QSharedPointer<QVector<DataBridge::GeoPoint>> trackCoordinates = context->GetTrackCoordinatesVector();

     //Save previous point variable

     //Boolean (switch thing) to see if the previous point was out of bounds (Default: False, point is out of bound) (False: Out of bounds, True: In Bounds)

    for (int i = 0; i < trackCoordinates->size(); i++)
    {

        //Check in bounds, if within bounds, then draw

        //Convert coords

        //Draw it

        //Set the current point to previous point

    }

}

void Drawing::drawCone(QSharedPointer<DataBridge>context)
{

}

void Drawing::drawWarnings(QSharedPointer<DataBridge>context)
{

}
