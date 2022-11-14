#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/databridge.h"
#include <QDir>
#include <QString>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

