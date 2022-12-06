#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers/maprenderer.h"
#include "headers/libosmhandler.h"

#include <QMainWindow>
#include <QSharedPointer>
#include <QSharedPointer>
#include "headers/databridge.h"
#include "headers/drawing.h"

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

private Q_SLOTS:
    void on_pushButton_clicked() const;

Q_SIGNALS:
    void windowShown();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    MapRenderer mapRenderer;
    QSharedPointer<LibOsmHandler> osmHandler;

    bool windowShownSwitch;

    //DataBridge Pointer
    QSharedPointer<DataBridge> dataBridge;
    Drawing hurricaneDrawing;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
