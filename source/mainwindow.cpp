#include "headers/mainwindow.h"
#include "headers/maprenderer.h"
#include "ui_mainwindow.h"

#include <QSharedPointer>
#include <QOpenGLWidget>

void test();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSharedPointer<QOpenGLWidget> openglWidget = QSharedPointer<QOpenGLWidget>(ui->openGLWidget);
    MapRenderer m(openglWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

