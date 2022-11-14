#ifndef RENDEREROPENGLWIDGET_H
#define RENDEREROPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QPixmap>

class RendererOpenGLWidget : public QOpenGLWidget
{
    QPixmap composite;

    public:
    RendererOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {}

    void updateComposite(QPixmap newComposite);

    void paintGL() override;
};

#endif // RENDEREROPENGLWIDGET_H
