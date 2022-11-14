#include "headers/databridge.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QtXml>

void DataBridge::ReadConeFile(QString fileName)
{
    //The QDomDocument class represents an XML document.
    QDomDocument xmlBOM;

    // Load xml file as raw data
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly))
    {
        // Set data into the QDomDocument before processing
        xmlBOM.setContent(&file);

        QDomElement root=xmlBOM.documentElement();

        QDomElement Component=root.firstChild().firstChild().toElement();

        while (!Component.isNull())
        {
            if (Component.tagName() == "Placemark")
            {
                // Get the first child of the component
                QDomElement Child = Component.firstChild().toElement();

                // Read each child of the component node
                while (!Child.isNull())
                {
                    if (Child.tagName() == "Polygon")
                    {
                        QDomElement Gchild = Child.firstChild().toElement();

                        while (!Gchild.isNull())
                        {
                            if (Gchild.tagName() == "outerBoundaryIs")
                            {
                                //Stores the coordinates in a string, will parse later on for each set of coord.
                                coneString = Gchild.firstChild().toElement().text();

                                //Varirables (for loop)
                                int _commaCount = 0;
                                QString xCoord = "";
                                QString yCoord = "";
                                QString zCoord = "";

                                //Parses the string to get individual set of coordinates
                                for (int i = 0; i < coneString.length() - 1; i++)
                                {
                                    if (coneString[i] == ' ')
                                    {
                                        continue;
                                    }

                                    if (coneString[i] == ',')
                                    {
                                        _commaCount++;
                                    }

                                    if (_commaCount == 0 && coneString[i].isDigit() || _commaCount == 0 && coneString[i] != ' ' && coneString[i] != ',')
                                    {
                                        if (coneString[i] != '\n')
                                        {
                                            xCoord += coneString[i];
                                        }
                                    }

                                    if (_commaCount == 1 && coneString[i].isDigit() || _commaCount == 1 && coneString[i] != ' ' && coneString[i] != ',')
                                    {
                                        yCoord += coneString[i];
                                    }

                                    if (_commaCount == 2 && coneString[i].isDigit() || _commaCount == 2 && coneString != ' ' && coneString[i] != ',')
                                    {
                                        zCoord += coneString[i];

                                        if (coneString[i + 1] == ' ')
                                        {
                                            _commaCount = 3;
                                        }
                                    }

                                    if (_commaCount == 3)
                                    {
                                        coneCoordinatesVector.push_back(ConePoint(xCoord.toFloat(), yCoord.toFloat(), zCoord.toFloat()));

                                        //Reset coordinates
                                        xCoord = "";
                                        yCoord = "";
                                        zCoord = "";

                                        //Reset _commacount
                                        _commaCount = 0;
                                    }

                                }
                            }
                          Gchild = Gchild.nextSibling().toElement();
                        }
                        break;
                    }
                    Child = Child.nextSibling().toElement();
                }
            }
          Component = Component.nextSibling().toElement();
        }
    }
}

void DataBridge::ReadWarningsFile(QString fileName)
{
    //The QDomDocument class represents an XML document.
    QDomDocument xmlBOM;

    // Load xml file as raw data
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly))
    {
        // Set data into the QDomDocument before processing

        xmlBOM.setContent(&file);

        QDomElement root=xmlBOM.documentElement();

        QDomElement Component=root.firstChild().firstChild().toElement();
        //qDebug() << "In file";

        while (!Component.isNull())
        {
            //qDebug() << "In while loop";
            //qDebug() << Component.tagName();
            if (Component.tagName() == "Placemark")
            {   //warnings files have several placemark components w different coordinates. Data in each is stored in a separate "WarningsPlacemark" struct. The structs are all stored in a vector
                //the name is stored in case we need to differentiate b/w watches n warnings
                //advisory date coz it may be helpful
                //coordinates vector stored using the same coordinate struct used in ReadConeFile
                //the variables are declared here so that each iteration can store new data
                //qDebug() << "we here";
                QString warningName;
                QString warningsCoordinatesString;
                QVector<ConePoint> warningsCoordinatesVector;
                QString advisoryDate;

                QDomElement Child=Component.firstChild().toElement();

                while (!Child.isNull())
                {
                    if (Child.tagName() == "name")
                    {
                          warningName = Child.toElement().text();
                          //qDebug() << warningName;
                    }
                    if (Child.tagName() == "LineString")
                    {   //usd Johnny's code from readConeFile
                        warningsCoordinatesString = Child.firstChild().toElement().text();

                        //Varirables (for loop)
                        int _commaCount = 0;
                        QString xCoord = "";
                        QString yCoord = "";
                        QString zCoord = "";

                        //Parses the string to get individual set of coordinates
                        for (int i = 0; i < warningsCoordinatesString.length() - 1; i++)
                        {
                            if (warningsCoordinatesString[i] == ' ')
                            {
                                continue;
                            }

                            if (warningsCoordinatesString[i] == ',')
                            {
                                _commaCount++;
                            }

                            if (_commaCount == 0 && warningsCoordinatesString[i].isDigit() || _commaCount == 0 && warningsCoordinatesString[i] != ' ' && warningsCoordinatesString[i] != ',')
                            {
                                if (warningsCoordinatesString[i] != '\n')
                                {
                                    xCoord += warningsCoordinatesString[i];
                                }
                            }

                            if (_commaCount == 1 && warningsCoordinatesString[i].isDigit() || _commaCount == 1 && warningsCoordinatesString[i] != ' ' && warningsCoordinatesString[i] != ',')
                            {
                                yCoord += warningsCoordinatesString[i];
                            }

                            if (_commaCount == 2 && warningsCoordinatesString[i].isDigit() || _commaCount == 2 && warningsCoordinatesString != ' ' && warningsCoordinatesString[i] != ',')
                            {
                                zCoord += warningsCoordinatesString[i];

                                if (warningsCoordinatesString[i + 1] == ' ')
                                {
                                    _commaCount = 3;
                                }
                            }

                            if (_commaCount == 3)
                            {
                                warningsCoordinatesVector.push_back(ConePoint(xCoord.toFloat(), yCoord.toFloat(), zCoord.toFloat()));

                                //Reset coordinates
                                xCoord = "";
                                yCoord = "";
                                zCoord = "";

                                //Reset _commacount
                                _commaCount = 0;
                            }
                        }
                    }
                    if (Child.tagName() == "ExtendedData")
                    {   //all children of extendedData are tagnamed data
                        QDomElement Gchild = Child.firstChild().toElement();
                        advisoryDate = Gchild.nextSibling().nextSibling().toElement().text();   //third sibling is advisorydate
                        //if more data needed, add here
                    }
                    Child = Child.nextSibling().toElement();
                }
                //create struct and add to vector of structs
                warningsData.push_back(WarningsPlacemark(warningName, advisoryDate, warningsCoordinatesVector));

            }
            Component = Component.nextSibling().toElement();
        }
    }
}

void DataBridge::ReadTrackFile(QString fileName)
{
    //The QDomDocument class represents an XML document.
    QDomDocument xmlBOM;

    // Load xml file as raw data
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly))
    {
        // Set data into the QDomDocument before processing

        xmlBOM.setContent(&file);

        QDomElement root=xmlBOM.documentElement();

        QDomElement Component=root.firstChild().firstChild().toElement();
        qDebug() << "In file";

        while (!Component.isNull())
        {
            qDebug() << "In while loop";
            qDebug() << Component.tagName();
            //Check if the tag name is <name
            //track files don't have a tag name named name. haha
            if (Component.tagName() == "Folder")
            {
                qDebug() << "In if statement";

                // Get the first child of the component
                QDomElement Child=Component.firstChild().toElement();

                // Read each child of the component node
                while (!Child.isNull())
                {
                    qDebug() << Child.tagName();
                    if (Child.tagName() == "Placemark")
                    {
                        qDebug() << "in child if statement";
                        QDomElement Gchild = Child.firstChild().toElement();
                        while (!Gchild.isNull())
                        {
                            qDebug() << Gchild.tagName();
                            if (Gchild.tagName() == "LineString")
                            {
                                qDebug() << "coordinates time";

                                //Stores the coordinates in a string, will parse later on for each set of coord.
                                trackCoordinates = Gchild.firstChild().toElement().text();

                                qDebug() << trackCoordinates;

                            }
                            if (Gchild.tagName() == "ExtendedData")
                            {

                                qDebug() << "data time";
                            }
                            Gchild = Gchild.nextSibling().toElement();
                        }
                        break;
                    }
                    Child = Child.nextSibling().toElement();
                }
            }
            Component = Component.nextSibling().toElement();    //next component
        }
    }

    file.close();
}

DataBridge::DataBridge(QString fileName)
{
    //ReadTrackFile(fileName);
    //ReadConeFile(fileName);
    ReadWarningsFile(fileName);

    /*testing for wanrings
    for (int i = 0; i < warningsData.size(); i++)
    {
        qDebug() << warningsData[i].warningName;
        qDebug() << "|";
        qDebug() << warningsData[i].advisoryDate;
        qDebug() << "|";
        for (int j = 0; j < warningsData[i].warningsCoordinatesVector.size(); j++)
        {
            qDebug() << warningsData[i].warningsCoordinatesVector[j].x;
            qDebug() << "";
            qDebug() << warningsData[i].warningsCoordinatesVector[j].y;
            qDebug() << "";
            qDebug() << warningsData[i].warningsCoordinatesVector[j].z;
            qDebug() << "";
        }
    }*/
}
