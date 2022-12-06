#ifndef DRAWING_H
#define DRAWING_H
#include "headers/databridge.h"
#include <QPixmap>
#include <QPainter>

class Drawing
{
public:
    void drawCone(DataBridge* context);
    void drawTrack(DataBridge* context);
    void drawWarnings(DataBridge* context);
};

#endif // DRAWING_H
