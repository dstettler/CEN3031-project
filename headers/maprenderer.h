#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include "headers/rendereropenglwidget.h"

#include <QVector>
#include <QPixmap>
#include <QSharedPointer>
#include <QOpenGLWidget>
#include <QPair>

class MapRenderer
{
public:
    enum RenderLayer
    {
        Map = 0,
        Warnings = 1,
        Cone = 2,
        Track = 3,        
    };

private:
    QVector<QPixmap> layers;
    QPixmap composite;
    
    QPair<int, int> openGLNodeSize;

    QSharedPointer<RendererOpenGLWidget> openGLNode;

    void generateCompositeFromLayers();

public:
    explicit MapRenderer();
    void addLayer(QPixmap pixmap);
    bool updateLayer(RenderLayer layer, QPixmap newPixmap);
    void updateImage();
    void updateOpenGLNode(QSharedPointer<RendererOpenGLWidget> openGLNode);
    QPair<int, int> getOpenGLNodeSize();
};

#endif // MAPRENDERER_H
