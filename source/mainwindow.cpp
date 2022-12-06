#include "headers/mainwindow.h"
#include "headers/maprenderer.h"
#include "headers/rendereropenglwidget.h"
#include "headers/libosmhandler.h"

#include "ui_mainwindow.h"

#include <locale>

#include <QMessageBox>
#include <QSharedPointer>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mapRenderer()
{
    ui->setupUi(this);

    // Signal and slot to ensure events are not called until everything has been initialized
    connect(this, &MainWindow::windowShown, this, &MainWindow::onWindowShown);
    
    windowShownSwitch = false;

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

        osmHandler->renderMap();
        mapRenderer.updateImage();
    }
}

void MainWindow::on_pushButton_clicked()
{
    // Relief funds window will be launched here
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

    // Initial map image render
    osmHandler->renderMap();
    mapRenderer.updateImage();

    windowShownSwitch = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

