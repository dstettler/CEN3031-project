#include "headers/databridge.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QtXml>

void DataBridge::ReadXMLFile(QString fileName)
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

                //qDebug() << Component.text();

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
                                qDebug() << Gchild.firstChild().toElement().text();
                                trackCoordinates = Gchild.firstChild().toText().data();
                                qDebug() << trackCoordinates.toStdString().c_str();
                                //maybe Qstring isnt the best way to store coordinates at this point?
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
    qDebug() << "la la";
    ReadXMLFile(fileName);
}
