#include "headers/databridge.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QSharedPointer>
#include <QMessageBox>
#include <QDomDocument>
#include <QVector>
#include <QtXml>

//---------------------------------------GETTER FUNCTIONS--------------------------------------------------------//
QVector<DataBridge::GeoPoint>* DataBridge::GetTrackCoordinatesVector()
{
    return &trackCoordinates;
}
QVector<DataBridge::GeoPoint>* DataBridge::GetConeCoordinatesVector()
{
    return &coneCoordinatesVector;
}

QVector<DataBridge::GeoPoint>* DataBridge::GetWarningsCoordinatesVector()
{
    return &warningsCoordinatesVector;
}

QVector<DataBridge::WarningsPlacemark>* DataBridge::GetWarningsDataVector()
{
    return &warningsData;
}
DataBridge::GeoPoint DataBridge::GetBoundBoxLeft()
{
    return boundBoxLeft;
}

DataBridge::GeoPoint DataBridge::GetBoundBoxRight()
{
    return boundBoxRight;
}

MapRenderer* DataBridge::GetMapRendererPtr()
{
    return mapRendererPtr;
}

//---------------------------------------READ CONE, WARNING, WARNINGS FROM FILE----------------------------------//
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
                                        coneCoordinatesVector.push_back(GeoPoint(xCoord.toFloat(), yCoord.toFloat(), zCoord.toFloat()));

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

                        /*for (int i = 0; i < coneCoordinatesVector.size(); i++)
                        {
                            qDebug() << coneCoordinatesVector[i].x << " " << coneCoordinatesVector[i].y << " " << coneCoordinatesVector[i].z;
                        } */

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
                QVector<GeoPoint> _warningsCoordinatesVector;
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
                                warningsCoordinatesVector.push_back(GeoPoint(xCoord.toFloat(), yCoord.toFloat(), zCoord.toFloat()));
                                _warningsCoordinatesVector.push_back(GeoPoint(xCoord.toFloat(), yCoord.toFloat(), zCoord.toFloat()));


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
                warningsData.push_back(WarningsPlacemark(warningName, advisoryDate, _warningsCoordinatesVector));

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


        while (!Component.isNull())
        {
            //Check if the tag name is <name
            //track files don't have a tag name named name. haha
            if (Component.tagName() == "Folder")
            {

                // Get the first child of the component
                QDomElement Child=Component.firstChild().toElement();

                // Read each child of the component node
                while (!Child.isNull())
                {

                    if (Child.tagName() == "Placemark")
                    {

                        QDomElement Gchild = Child.firstChild().toElement();
                        while (!Gchild.isNull())
                        {

                            if (Gchild.tagName() == "LineString")
                            {
                                // if the placemark node has a linestring child, we know
                                // it is not one of the correct placemark nodes to be looking at
                                // so we break to go to the next sibling placemark
                                // go to next sibling
                                break;
                             }

                             if (Gchild.tagName() == "Point")
                             {

                                QString track = Gchild.firstChild().toElement().text();

                                // get x
                                int delimiter = track.indexOf(",", 0);
                                QString xCoord = track.mid(0, delimiter);
                                xCoord.remove(0, 1); // removes extra white space at beginning
                                track.remove(0, delimiter + 1);

                                // get y
                                delimiter = track.indexOf(",", 0);
                                QString yCoord = track.mid(0, delimiter);
                                track.remove(0, delimiter + 1);

                                // get z
                                delimiter = track.indexOf(",", 0);
                                QString zCoord = track.mid(0, delimiter);
                                zCoord.remove(zCoord.length() - 1, 1); // removes extra white space at end

                                //convert strings to float
                                float x = xCoord.toFloat();
                                float y = yCoord.toFloat();
                                float z = zCoord.toFloat();

                                //create struct and add to vector of structs
                                trackCoordinates.push_back(GeoPoint(x, y, z));

                             }

                            Gchild = Gchild.nextSibling().toElement();
                        } 
                    }
                    Child = Child.nextSibling().toElement();
                }
            }
            Component = Component.nextSibling().toElement();    //next component
        }
    }

    file.close();
}

//---------------------------CONVERSION FROM EARTH COORD TO ON-SCREEN COORD-----------------------------//
QPair<int,int> DataBridge::LatLonToScreenCoord(float x, float y)    //converts geo coords to screen coords
{
    QPair<int, int> ScreenPoint;
    ScreenPoint.first = (x - boundBoxLeft.x)/CoordPerPixel(widthInPixels, heightInPixels).first;
    ScreenPoint.second = (boundBoxLeft.y - y)/CoordPerPixel(widthInPixels, heightInPixels).second;
    return ScreenPoint;
}

QPair<float, float> DataBridge::CoordPerPixel(int widthInPixels, int heightInPixels)  //finds the number of coordinates per screen pixel. width in pixels must be known
{
    QPair<float, float> CoordPerPixel;
    float x1 = boundBoxLeft.x;          //left longitude
    float x2 = boundBoxRight.x;         //right longitude
    float y1 = boundBoxLeft.y;
    float y2 = boundBoxRight.y;


    CoordPerPixel.first = (x2 - x1)/widthInPixels;
    CoordPerPixel.second = (y2 - y1)/heightInPixels;
    return CoordPerPixel;
}

//-------------------------------------CONSTRUCTOR-----------------------------------------------------//
DataBridge::DataBridge(QString fileName, MapRenderer *renderer)
     :mapRendererPtr(renderer),
      boundBoxLeft(-89.3958, 34.0412, 0),
      boundBoxRight(-73.3042, 24.1008, 0),
      widthInPixels(renderer->getOpenGLNodeSize().first),
      heightInPixels(renderer->getOpenGLNodeSize().second)
      //get width in pixels
{
    ReadTrackFile(fileName + "/noaa_cache/0/track/al172022_005Aadv_TRACK.kml");
    ReadConeFile(fileName + "/noaa_cache/0/cone/al172022_005Aadv_CONE.kml");
    ReadWarningsFile(fileName + "/noaa_cache/0/warnings/al172022_005Aadv_WW.kml");
}

