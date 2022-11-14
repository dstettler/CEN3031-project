#include "headers/mainwindow.h"
#include "headers/maprenderer.h"
#include "headers/rendereropenglwidget.h"

#include "ui_mainwindow.h"
#include "headers/databridge.h"
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
    emit windowShown();
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
        QString _fileName = fi.canonicalFilePath() + "/ConeTest.xml";

    //For Windows
    #else
        QString _fileName = QDir::currentPath() + "/ConeTest.xml";
    #endif

    DataBridge d(_fileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

