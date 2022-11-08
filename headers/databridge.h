#ifndef DATABRIDGE_H
#define DATABRIDGE_H
#include <QString>
#include <QVector>

class DataBridge
{
public:
    DataBridge(QString fileName);

private:
    void ReadXMLFile(QString fileName);
    QString trackCoordinates;
};

#endif // DATABRIDGE_H
