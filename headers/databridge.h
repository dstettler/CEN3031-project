#ifndef DATABRIDGE_H
#define DATABRIDGE_H
#include <QString>

class DataBridge
{
public:
    DataBridge(QString fileName);
private:
    void ReadXMLFile(QString fileName);
};

#endif // DATABRIDGE_H
