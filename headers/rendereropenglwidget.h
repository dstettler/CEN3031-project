#ifndef RENDEREROPENGLWIDGET_H
#define RENDEREROPENGLWIDGET_H

class RendererOpenGLWidget : public QOpenGLWidget
{
    public:
    RendererOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {}

    void paintGL() override;
};

#endif // RENDEREROPENGLWIDGET_H
