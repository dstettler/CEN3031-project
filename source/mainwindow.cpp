#include "headers/mainwindow.h"
#include "headers/maprenderer.h"
#include "headers/rendereropenglwidget.h"

#include "ui_mainwindow.h"
#include "headers/databridge.h"
#include "headers/drawing.h"
#include <QDir>
#include <QString>
#include <QFileInfo>

#include <QMessageBox>
#include <QSharedPointer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mapRenderer()
{
    ui->setupUi(this);

    // Signal and slot to ensure events are not called until everything has been initialized
    connect(this, &MainWindow::windowShown, this, &MainWindow::onWindowShown);
}

void MainWindow::show()
{
    QMainWindow::show();
    QApplication::processEvents();
    Q_EMIT windowShown();
}

void MainWindow::onWindowShown()
{
    // Now that the QOpenGLWidget has been initialized we can give the MapRenderer its pointer
    mapRenderer.updateOpenGLNode(QSharedPointer<RendererOpenGLWidget>(ui->openGLWidget));
    mapRenderer.updateImage();
    
    //For Mac
    #ifdef Q_OS_MACX
        QDir test(QDir::currentPath());

        //Go up from .app
        test.cdUp();
        test.cdUp();
        test.cdUp();

        QString tempString;

        QFileInfo fi(test, tempString);
        QString _fileName = fi.canonicalFilePath();

    //For Windows
    #else
        QString _fileName = QDir::currentPath();
    #endif

    /* Notes for how to change file paths
     * If you are windows the only thing you change is inside the quotes under "//For Windows"
     * You have to move the file manually into a folder called build-hurrigators-project...etc, (PICK MOST RECENT DATE) same level as hurrigators-project
     * Make sure to also change the function call under databridge.cpp
    */

    //Making dataBridge point to DataBridge instance
    dataBridge = QSharedPointer<DataBridge> (new DataBridge(_fileName, &mapRenderer));

    hurricaneDrawing.drawWarnings(dataBridge.get());
    hurricaneDrawing.drawCone(dataBridge.get());
    hurricaneDrawing.drawTrack(dataBridge.get());


    mapRenderer.updateImage();

}

MainWindow::~MainWindow()
{
    delete ui;
}

