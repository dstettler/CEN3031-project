#ifndef DATABRIDGE_H
#define DATABRIDGE_H
#include <QString>
#include <QVector>
#include <QPair>

class DataBridge
{
    //TrackPoint object to store information about each point on the track
    struct TrackPoint
    {
        QString coordinates;
        QString time;
        QPair<QString,QString> location;
        int maxWindMPH;
        int windGusts;
        QString motion;
        int minPressure;
    };

    //ConePoint object to store coordinates of each point for cone
    struct ConePoint
    {
        //Constructor
        ConePoint(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        float x;
        float y;
        float z;
    };

public:
    DataBridge(QString fileName);

private:
    //Functions
    void ReadTrackFile(QString fileName);
    void ReadConeFile(QString fileName);
    void ReadWarningsFile(QString fileName);

    //Variables
    QString trackCoordinates;
    QString coneString;
    QVector<ConePoint> coneCoordinatesVector;
};

#endif // DATABRIDGE_H
