#include "headers/mainwindow.h"
#include "headers/maprenderer.h"
#include "headers/rendereropenglwidget.h"
#include "headers/libosmhandler.h"
#include "headers/relieffundsdialog.h"

#include "ui_mainwindow.h"
#include "headers/databridge.h"
#include "headers/drawing.h"
#include <QDir>
#include <QString>
#include <QFileInfo>

#include <locale>

#include <QMessageBox>
#include <QSharedPointer>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mapRenderer(),
    windowShownSwitch(false)
{
    ui->setupUi(this);

    // Signal and slot to ensure events are not called until everything has been initialized
    connect(this, &MainWindow::windowShown, this, &MainWindow::onWindowShown);

    ui->pushButton->setFocusPolicy(Qt::NoFocus);
}

void MainWindow::show()
{
    QMainWindow::show();
    QApplication::processEvents();
    Q_EMIT windowShown();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (windowShownSwitch && event)
    {
        if (event->type() == QKeyEvent::KeyPress && (event->key() == Qt::Key_Up || event->key() == Qt::Key_W))
        {
            osmHandler->moveUp();
        }
        else if (event->type() == QKeyEvent::KeyPress && (event->key() == Qt::Key_Down || event->key() == Qt::Key_S))
        {
            osmHandler->moveDown();
        }
        else if (event->type() == QKeyEvent::KeyPress && (event->key() == Qt::Key_Left || event->key() == Qt::Key_A))
        {
            osmHandler->moveLeft();
        }
        else if (event->type() == QKeyEvent::KeyPress && (event->key() == Qt::Key_Right || event->key() == Qt::Key_D))
        {
            osmHandler->moveRight();
        }
        else if (event->type() == QKeyEvent::KeyPress && event->key() == Qt::Key_Minus)
        {
            osmHandler->zoomOut();
        }
        else if (event->type() == QKeyEvent::KeyPress && event->key() == Qt::Key_Plus)
        {
            osmHandler->zoomIn();
        }
        else
        {
            return;
        }

        dataBridge->SetBoundBoxLeft(osmHandler->getTopLeft());
        dataBridge->SetBoundBoxRight(osmHandler->getBottomRight());

        hurricaneDrawing.drawWarnings(dataBridge.get());
        hurricaneDrawing.drawCone(dataBridge.get());
        hurricaneDrawing.drawTrack(dataBridge.get());

        osmHandler->renderMap();
        mapRenderer.updateImage();
    }
}

void MainWindow::on_pushButton_clicked() const
{
    ReliefFundsDialog _diag;
    _diag.exec();
}


void MainWindow::onWindowShown()
{
    // Now that the QOpenGLWidget has been initialized we can give the MapRenderer its pointer
    mapRenderer.updateOpenGLNode(QSharedPointer<RendererOpenGLWidget>(ui->openGLWidget));
    mapRenderer.updateImage();

    QString buildPath = QDir::currentPath();

    // For some reason std::locale can crash if the locale cannot be found
    // (i.e. in a debugger), so calling this here to expedite the inevitable
    std::string localeStr = std::locale("").name();

    osmHandler = QSharedPointer<LibOsmHandler>(new LibOsmHandler(buildPath, &mapRenderer));

    
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


    // Initial map image render
    osmHandler->renderMap();
    mapRenderer.updateImage();

    windowShownSwitch = true;

}

MainWindow::~MainWindow()
{
    delete ui;
}

