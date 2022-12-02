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

        //Default constructor
        GeoPoint() {};

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

    // conversion from earth coordinates to on screen coordinates
    QPair<float, float> CoordPerPixel(int widthInPixels, int heightInPixels);
    QPair<int,int> LatLonToScreenCoord(float x, float y);


    // constructor
    DataBridge(QString fileName, MapRenderer *renderer);

    QPair<float, float> getBoundBoxLats;

    //Getter functions
    QVector<GeoPoint>* GetTrackCoordinatesVector();
    QVector<GeoPoint>* GetConeCoordinatesVector();
    QVector<GeoPoint>* GetWarningsCoordinatesVector();
    GeoPoint GetBoundBoxLeft();
    GeoPoint GetBoundBoxRight();
    MapRenderer* GetMapRendererPtr();

private:
    //Functions
    void ReadTrackFile(QString fileName);
    void ReadConeFile(QString fileName);
    void ReadWarningsFile(QString fileName);

    //Variables
    QVector<GeoPoint> trackCoordinates;
    QString coneString;
    QVector<GeoPoint> coneCoordinatesVector;
    QVector<GeoPoint> warningsCoordinatesVector;        //might only need this and not warnings data but lets see, this stores all coordinates
    QVector<WarningsPlacemark> warningsData;
    GeoPoint boundBoxLeft;  //y is lattitudes, x is longs
    GeoPoint boundBoxRight;
    int widthInPixels;
    int heightInPixels;
    MapRenderer *mapRendererPtr;
};

#endif // DATABRIDGE_H
