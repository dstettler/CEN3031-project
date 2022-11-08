#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/databridge.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DataBridge d("C:/Users/piyum/Downloads/test.xml");
}

MainWindow::~MainWindow()
{
    delete ui;
}

