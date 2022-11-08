#include "headers/maprenderer.h"

#include <QPixmap>
#include <QPainter>

MapRenderer::MapRenderer(QSharedPointer<QOpenGLWidget> openglNode)
    :
      openglNode(openglNode)
{
    QPixmap _img("smile.png");

    QPainter _painter;

}

