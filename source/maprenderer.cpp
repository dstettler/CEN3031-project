#include "headers/maprenderer.h"

MapRenderer::MapRenderer(QSharedPointer<QOpenGLWidget> openglNode)
{
    this->openglNode = openglNode;
}
