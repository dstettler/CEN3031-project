#ifndef DRAWING_H
#define DRAWING_H
#include "headers/databridge.h"
#include <QPixmap>
#include <QPainter>

class Drawing
{
private:
    float coordsPerPixel;

public:
    void drawCone(QSharedPointer<DataBridge>context);
    void drawTrack(QSharedPointer<DataBridge>context);
    void drawWarnings(QSharedPointer<DataBridge>context);
};

#endif // DRAWING_H
