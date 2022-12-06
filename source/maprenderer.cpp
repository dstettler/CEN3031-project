#include "headers/maprenderer.h"
#include "headers/rendereropenglwidget.h"

#include <QPixmap>
#include <QPainter>
#include <QRectF>

MapRenderer::MapRenderer()
{
    // Temporary layers
    QPixmap _baseLayer(10,10);

    for (int i = 0; i < 4; i++)
    {
        addLayer(_baseLayer);
    }

  openGLNodeSize = QPair<int, int>(0,0);
}

QPair<int, int> MapRenderer::getOpenGLNodeSize()
{
    return openGLNodeSize;
}

void MapRenderer::updateOpenGLNode(QSharedPointer<RendererOpenGLWidget> openGLNode)
{
    // This couldn't be assigned in the constructor because the MapRenderer in MainWindow 
    // is created before the QOpenGLWidget is initialized
    this->openGLNode = openGLNode;
}

void MapRenderer::addLayer(QPixmap pixmap)
{
    layers.push_back(pixmap);
}

bool MapRenderer::updateLayer(MapRenderer::RenderLayer layer, QPixmap newPixmap)
{
    if (static_cast<int>(layer) < layers.size() && static_cast<int>(layer) >= 0)
    {
        layers.remove(static_cast<int>(layer));
        layers.insert(static_cast<int>(layer), newPixmap);
        return true;
    }
    else
    {
        return false;
    }
}

void MapRenderer::generateCompositeFromLayers()
{
    QPixmap _canvas(openGLNodeSize.first, openGLNodeSize.second);
    
    _canvas.fill(Qt::transparent);

    // Use a regular pointer in place of a smart pointer to ensure a delete call can be made
    // before the pixmap is assigned (to ensure the framebuffer has finished drawing)
    QPainter* _painter = new QPainter(&_canvas);

    for (QPixmap _layerPixmap : layers)
    {
        // _target and _source are just bounding boxes for QPainter to draw within and from
        // Setting _target to the size of _canvas instead of _layerPixmap would stretch the layer
        // to the size of the entire canvas. 
        // Leaving both of these the same ensures the size and scaling of the pixmap remain constant
        QRectF _target(0.0, 0.0, _layerPixmap.width(), _layerPixmap.height());
        QRectF _source(0.0, 0.0, _layerPixmap.width(), _layerPixmap.height());
        _painter->drawPixmap(_target, _layerPixmap, _source);
    }

    delete _painter;
    composite = _canvas;
}

void MapRenderer::updateImage()
{
    openGLNodeSize = QPair<int, int>
    (
        openGLNode->width(),
        openGLNode->height()
    );

    // Ensure a composite exists before updating the OpenGL node
    generateCompositeFromLayers();

    openGLNode->updateComposite(composite);
    openGLNode->update();
}
