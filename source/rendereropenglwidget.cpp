#include "headers/rendereropenglwidget.h"

#include <QPainter>
#include <QRectF>

void RendererOpenGLWidget::updateComposite(QPixmap newComposite)
{
    composite = newComposite;
}

void RendererOpenGLWidget::paintGL()
{
    // Don't try to draw something unless we know we have an image
    if (!composite.isNull())
    {
        QPainter _painter(this);
        _painter.setPen(Qt::red);
        
        QRectF _target(0.0, 0.0, width(), height());
        QRectF _source(0.0, 0.0, composite.width(), composite.height());
        
        _painter.drawPixmap(_target, composite, _source);
    }
}