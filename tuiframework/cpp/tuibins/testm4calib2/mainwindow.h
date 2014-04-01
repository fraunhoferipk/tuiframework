#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <tuiframework/client/client.h>
#include <tuilibs/tuitypes/common/CommonTypeReg.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public tuiframework::IEventSink
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void push(tuiframework::IEvent * e);

    void v1Received(const Vector4Event * e);
    void v2Received(const Vector4Event * e);

public slots:
    void connectTUIServer();
    void disconnectTUIServer();

    void apply();
    void calc();
    void clear();
    void load();
    void save();
    void send();

    void exit();

    void processTUIEvents();
    
private:
    Ui::MainWindow *ui;
    QTimer timer;

    tuiframework::IEventChannel * calcCalibratedMatrix;
    tuiframework::IEventChannel * clearVectors;
    tuiframework::IEventChannel * applyVectors;
    tuiframework::IEventChannel * sendCalibratedMatrix;
    tuiframework::IEventChannel * loadCalibratedMatrix;
    tuiframework::IEventChannel * saveCalibratedMatrix;

    Vector4<double> v1;
    Vector4<double> v2;
};

#endif // MAINWINDOW_H
