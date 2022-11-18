#ifndef DATABRIDGE_H
#define DATABRIDGE_H
#include <QString>
#include <QVector>
#include <QPair>
#include "headers/maprenderer.h"

class DataBridge
{
    //Point object to store coordinates of each point for cone
    struct GeoPoint
    {
        //Constructor
        GeoPoint(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        float x;
        float y;
        float z;
    };

    struct WarningsPlacemark
    {
        QString warningName;
        QString advisoryDate;
        QVector<GeoPoint> warningsCoordinatesVector;

        //constructor
        WarningsPlacemark(QString warningName, QString advisoryDate, QVector<GeoPoint>& _warningsCoordinatesVector)
        {
            this->warningName = warningName;
            this->advisoryDate = advisoryDate;
            for (int i = 0; i < _warningsCoordinatesVector.size(); i++)
                warningsCoordinatesVector.push_back(_warningsCoordinatesVector[i]);
        }
    };

public:
    DataBridge(QString fileName, QSharedPointer<MapRenderer> renderer);

private:
    //Functions
    void ReadTrackFile(QString fileName);
    void ReadConeFile(QString fileName);
    void ReadWarningsFile(QString fileName);
    QPair<float,float> LatLonToScreenCoord(float x, float y);

    //Variables
    QVector<GeoPoint> trackCoordinates;
    QString coneString;
    QVector<GeoPoint> coneCoordinatesVector;
    QVector<WarningsPlacemark> warningsData;
    GeoPoint boundBoxLeft;
    GeoPoint boundBoxRight;
    QSharedPointer<MapRenderer> mapRendererPtr;
};

#endif // DATABRIDGE_H
