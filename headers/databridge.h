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
            :x(x),
             y(y),
             z(z)
        {

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
            :warningName(warningName),
             advisoryDate(advisoryDate),
             warningsCoordinatesVector(_warningsCoordinatesVector)
        {

        }
    };

    // conversion from earth coordinates to on screen coordinates
    QPair<float, float> CoordPerPixel(int widthInPixels, int heightInPixels) const;
    QPair<int,int> LatLonToScreenCoord(float x, float y);


    // constructor
    DataBridge(QString fileName, MapRenderer *renderer);

    //Getter functions
    QVector<GeoPoint>* GetTrackCoordinatesVector();
    QVector<GeoPoint>* GetConeCoordinatesVector();
    QVector<GeoPoint>* GetWarningsCoordinatesVector();
    QVector<WarningsPlacemark>* GetWarningsDataVector();
    GeoPoint GetBoundBoxLeft() const;
    GeoPoint GetBoundBoxRight() const;
    MapRenderer* GetMapRendererPtr();

    //Setter functions
    void SetBoundBoxLeft(QPair<double, double> p);
    void SetBoundBoxRight(QPair<double, double> p);

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
    GeoPoint boundBoxLeft = GeoPoint(-89.3958f, 34.0412f, 0.0f);  //y is lattitudes, x is longs
    GeoPoint boundBoxRight =  GeoPoint(-73.3042f, 24.1008f, 0.0f);
    int widthInPixels;
    int heightInPixels;
    MapRenderer *mapRendererPtr;
};

#endif // DATABRIDGE_H
