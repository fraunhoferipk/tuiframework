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

    void calibrationMatrix(const Matrix4Event * e);
    
public slots:
    void connectTUIServer();
    void disconnectTUIServer();
    void addPoints();
    void addPointsAverage();
    void clearPoints();
    void queryMatrix();
    void storeMatrix();
    void restoreMatrix();
    void exit();

    void processTUIEvents();

    Matrix4<double> createTransfMatrix() const;
    std::pair<Vector4<double>, Vector4<double> > generateVector4Pair() const;

    void printMatrix(const Matrix4<double> & m) const;
    void printVectors(const Vector4<double> & v1, const Vector4<double> & v2) const;

private:
    Ui::MainWindow *ui;
    QTimer timer;
    bool connected;

    tuiframework::IEventChannel * calcCalibratedMatrix;
    tuiframework::IEventChannel * clearVectors;
    tuiframework::IEventChannel * applyVectors;
    tuiframework::IEventChannel * sendCalibratedMatrix;
    tuiframework::IEventChannel * vector1;
    tuiframework::IEventChannel * vector2;
    tuiframework::IEventChannel * loadCalibratedMatrix;
    tuiframework::IEventChannel * saveCalibratedMatrix;
};

#endif // MAINWINDOW_H
