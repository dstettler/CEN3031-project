#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "headers/maprenderer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void show();
    ~MainWindow();

public Q_SLOTS:
    void onWindowShown();

Q_SIGNALS:
    void windowShown();

private:
    MapRenderer mapRenderer;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
