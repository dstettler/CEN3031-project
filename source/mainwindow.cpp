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

    QString buildPath = QDir::currentPath();// + "/build";

    std::string stdPath = buildPath.toStdString();

    std::string localeStr = std::locale("").name();


    osmHandler = QSharedPointer<LibOsmHandler>(new LibOsmHandler(stdPath, &mapRenderer));

    osmHandler->renderMap();

    mapRenderer.updateImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

