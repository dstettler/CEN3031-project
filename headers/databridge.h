#ifndef DATABRIDGE_H
#define DATABRIDGE_H
#include <QString>
#include <QVector>
#include <QPair>

class DataBridge
{
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

public:
    DataBridge(QString fileName);

private:
    void ReadXMLFile(QString fileName);
    void ReadConeFile(QString fileName);
    void ReadWarningsFile(QString fileName);
    QString trackCoordinates;
};

#endif // DATABRIDGE_H
