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

        QDomElement Component=root.firstChild().toElement();
        qDebug() << "In file";

        while (!Component.isNull())
        {
            qDebug() << "In while loop";
            //Check if the tag name is <name
            if (Component.tagName() == "name")
            {
                qDebug() << "In if statement";
                break;
                qDebug() << Component.text();

                // Get the first child of the component
                QDomElement Child=Component.firstChild().toElement();

                // Read each child of the component node
                while (!Child.isNull())
                {

                }
            }
        }
    }

    file.close();
}

DataBridge::DataBridge(QString fileName)
{
    ReadXMLFile(fileName);
}
