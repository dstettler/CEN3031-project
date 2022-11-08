#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include <QVector>
#include <QPixmap>
#include <QSharedPointer>
#include <QOpenGLWidget>
#include <QPair>

#include "headers/rendereropenglwidget.h"

class MapRenderer
{
    QVector<QPixmap> layers;
    QPixmap composite;
    
    QPair<int, int> openGLNodeSize;

    QSharedPointer<RendererOpenGLWidget> openGLNode;

    void generateCompositeFromLayers();

public:
    explicit MapRenderer();
    void addLayer(QPixmap pixmap);
    bool updateLayer(unsigned int layer, QPixmap newPixmap);
    void updateImage();
    void updateOpenGLNode(QSharedPointer<RendererOpenGLWidget> openGLNode);
};

#endif // MAPRENDERER_H
