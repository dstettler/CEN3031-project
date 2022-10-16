#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include <QVector>
#include <QPixmap>
#include <QSharedPointer>
#include <QOpenGLWidget>

class MapRenderer
{
    QVector<QPixmap> layers;
    QSharedPointer<QOpenGLWidget> openglNode;

public:
    MapRenderer(QSharedPointer<QOpenGLWidget> openglNode);
};

#endif // MAPRENDERER_H
