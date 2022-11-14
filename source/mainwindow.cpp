#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/databridge.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DataBridge d("C:/Users/piyum/Downloads/0warnings.xml");
    //DataBridge d("/Users/johnnyhuynh/Downloads/ConeTest.xml");
}

MainWindow::~MainWindow()
{
    delete ui;
}

