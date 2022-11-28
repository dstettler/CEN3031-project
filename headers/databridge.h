#ifndef DATABRIDGE_H
#define DATABRIDGE_H
#include <QString>
#include <QVector>
#include <QPair>
#include <QSharedPointer>
#include "headers/maprenderer.h"

class DataBridge
{
public:
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

    DataBridge(QString fileName, QSharedPointer<MapRenderer> renderer);
    QPair<float, float> getBoundBoxLats;

    //Getter functions
    QSharedPointer<QVector<GeoPoint>> GetTrackCoordinatesVector();
    QSharedPointer<QVector<GeoPoint>> GetConeCoordinatesVector();
    QSharedPointer<QVector<GeoPoint>> GetWarningsCoordinatesVector();
    GeoPoint GetBoundBoxLeft();
    GeoPoint GetBoundBoxRight();

private:
    //Functions
    void ReadTrackFile(QString fileName);
    void ReadConeFile(QString fileName);
    void ReadWarningsFile(QString fileName);
    float CoordPerPixel(int widthInPixels);
    QPair<float,float> LatLonToScreenCoord(float x, float y);

    //Variables
    QVector<GeoPoint> trackCoordinates;
    QString coneString;
    QVector<GeoPoint> coneCoordinatesVector;
    QVector<GeoPoint> warningsCoordinatesVector;        //might only need this and not warnings data but lets see, this stores all coordinates
    QVector<WarningsPlacemark> warningsData;
    GeoPoint boundBoxLeft;  //y is lattitudes, x is longs
    GeoPoint boundBoxRight;
    int widthInPixels;
    QSharedPointer<MapRenderer> mapRendererPtr;
};

#endif // DATABRIDGE_H
