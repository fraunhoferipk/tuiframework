#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTest>

#include <tuiframework/client/client.h>
#include <tuilibs/tuitypes/common/CommonTypeReg.h>

#include <iostream>

using namespace std;
using namespace tuiframework;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    connected(false),
    calcCalibratedMatrix(0),
    clearVectors(0),
    applyVectors(0),
    sendCalibratedMatrix(0),
    vector1(0),
    vector2(0),
    loadCalibratedMatrix(0),
    saveCalibratedMatrix(0)
{
    ui->setupUi(this);
    timer.setInterval(100);
    connect(&timer, SIGNAL(timeout()), this, SLOT(processTUIEvents()));
    timer.start();
    connect(ui->ConnectPushButton, SIGNAL(clicked()), this, SLOT(connectTUIServer()));
    connect(ui->DisconnectPushButton, SIGNAL(clicked()), this, SLOT(disconnectTUIServer()));
    connect(ui->NewPointsPushButton, SIGNAL(clicked()), this, SLOT(addPoints()));
    connect(ui->NewPointsAveragePushButton, SIGNAL(clicked()), this, SLOT(addPointsAverage()));
    connect(ui->ClearPointsPushButton, SIGNAL(clicked()), this, SLOT(clearPoints()));
    connect(ui->QueryMatrixPushButton, SIGNAL(clicked()), this, SLOT(queryMatrix()));
    connect(ui->StoreMatrixPushButton, SIGNAL(clicked()), this, SLOT(storeMatrix()));
    connect(ui->RestoreMatrixPushButton, SIGNAL(clicked()), this, SLOT(restoreMatrix()));
    connect(ui->ExitPushButton, SIGNAL(clicked()), this, SLOT(exit()));
}


MainWindow::~MainWindow()
{
    timer.stop();
    delete ui;
}


void MainWindow::connectTUIServer() {
    initTypeRegistration(getEventFactory());
    CommonTypeReg::registerTypes(&getEventFactory(), &getEventChannelFactory());
    connectWithTUIServer(8998, 8999, this->ui->TUIServerIPLineEdit->text().toStdString(), this, false);
}


void MainWindow::push(tuiframework::IEvent * event) {
    if (event->getEventTypeID() == SystemMsg::EventTypeID()) {
        SystemMsg * msg = static_cast<SystemMsg *>(event);
        if (msg->getPayload() == CONNECTION_ESTABLISHED) {
            cout << "CONNECTION ESTABLISHED" << endl;
            try {
                CONNECT(Matrix4Event, "CalibrationControler", "CalibratedMatrix", MainWindow, this, &MainWindow::calibrationMatrix);

                this->calcCalibratedMatrix  = getSinkChannel("CalibrationControler", "CalcCalibratedMatrix");
                if (this->calcCalibratedMatrix == 0) {
                    cout << "calcCalibratedMatrix == 0" << endl;
                }

                this->clearVectors          = getSinkChannel("CalibrationControler", "ClearVectors");
                if (this->clearVectors == 0) {
                    cout << "clearVectors == 0" << endl;
                }

                this->applyVectors          = getSinkChannel("CalibrationControler", "ApplyVectors");
                if (this->applyVectors == 0) {
                    cout << "applyVectors == 0" << endl;
                }

                this->sendCalibratedMatrix  = getSinkChannel("CalibrationControler", "SendCalibratedMatrix");
                if (this->sendCalibratedMatrix == 0) {
                    cout << "sendCalibratedMatrix == 0" << endl;
                }

                this->vector1               = getSinkChannel("CalibrationControler", "Vector1");
                if (this->vector1 == 0) {
                    cout << "vector1 == 0" << endl;
                }

                this->vector2               = getSinkChannel("CalibrationControler", "Vector2");
                if (this->vector2 == 0) {
                    cout << "vector2 == 0" << endl;
                }

                this->loadCalibratedMatrix  = getSinkChannel("CalibrationControler", "LoadCalibratedMatrix");
                if (this->loadCalibratedMatrix == 0) {
                    cout << "loadCalibratedMatrix == 0" << endl;
                }

                this->saveCalibratedMatrix  = getSinkChannel("CalibrationControler", "SaveCalibratedMatrix");
                if (this->saveCalibratedMatrix == 0) {
                    cout << "saveCalibratedMatrix == 0" << endl;
                }

            } catch (const Exception & e) {
                cerr << "Exception" << endl;
                cerr << e.getFormattedString() << endl;
            }
        }
    }
}


void MainWindow::disconnectTUIServer() {
    std::cout << "MainWindow::disconnectTUIServer()" << std::endl;
}



void MainWindow::addPoints() {
    std::cout << "MainWindow::addPoints()" << std::endl;

    for (int i = 0; i < 8; ++i) {
        pair<Vector4<double>, Vector4<double> > pv = this->generateVector4Pair();
       // cout << pv.first << "  " << pv.second << endl;
        if (this->vector1) this->vector1->push(new Vector4Event(-1, -1, pv.first));
        if (this->vector2) this->vector2->push(new Vector4Event(-1, -1, pv.second));
        QTest::qWait(100);
        if (this->applyVectors) this->applyVectors->push(new DigitalChangedEvent(-1, -1, true));
    }


    return;

    //pair<Vector4<double>, Vector4<double> > pv[8];
/*
  //−915.581×78.7307 + 57.7019×718.481 + 130.762×17.9698 − 22321.6
    pv[0] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(776.736, -39.654, 1628.62), Vector4<double>(77.8456, -719.356, 130.799));
    pv[1] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(693.537, -22.194, 1655), Vector4<double>(77.9437, -719.507, 130.821));
    pv[2] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(-353.171, 48.85, 1414.24), Vector4<double>(78.8377, -718.735, 131.018));
    pv[3] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(368.238, 2.729, 1479.51), Vector4<double>(78.2005, -718.786, 131.01));
*/
/*
    pv[0] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(-219.901, 380.634, 1546.24), Vector4<double>(78.7307, -718.481, 130.762));
    pv[1] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(205.011, -74.673, 1689.18), Vector4<double>(78.3263, -718.896, 130.834));
    pv[2] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(147.747, 361.227, 1476.48), Vector4<double>(78.327, -718.462, 130.862));
    pv[3] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(-25.67, -78.023, 1377.84), Vector4<double>(78.5611, -718.77, 131.155));
*/
/*
    pv[0] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(522.533, -121.136, 1299.49), Vector4<double>(0.306405, -0.0573421, 1.90411));
    pv[1] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(250.918, -98.991, 884.109), Vector4<double>(0.0958315, 0.124358, 1.48461));
    pv[2] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(404.848, -102.868, 914.839), Vector4<double>(0.203942, 0.112578, 1.52845));
    pv[3] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(571.042, 50.285, 1125.52), Vector4<double>(0.345788, 0.165765, 1.80666));

    pv[4] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(207.742, -18.918, 1267.98), Vector4<double>(-0.0381521, 0.0562438, 1.85544));
    pv[5] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(312.921, -10.813, 1286.0), Vector4<double>(0.0571454, 0.0627858, 1.88701));
    pv[6] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(348.472, -9.996, 1286.85), Vector4<double>(0.0912501, 0.0608848, 1.89758));
    pv[7] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(77.094, -44.822, 1205.43), Vector4<double>(-0.142751, 0.0610404, 1.77602));
*/
    pair<Vector4<double>, Vector4<double> > pv[100];
    int i = 0;
/*
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(36.8181, -718.622, -455.811), Vector4<double>(598.069, -103.963, 1267.28));
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(37.1976, -718.227, -454.852), Vector4<double>(-126.671, -81.539, 283.229));
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(37.0978, -718.768, -455.536), Vector4<double>(71.505, -193.078, 1181.87));
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(37.1845, -718.546, -456.411), Vector4<double>(256.852, 269.361, 1850.72));
 */

    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(36.9249, -718.576, -455.495), Vector4<double>(431.334, -242.123, 1059.94));
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(37.4273, -718.172, -455.579), Vector4<double>(-168.369, 279.603, 813.793));
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(37.2145, -718.236, -454.716), Vector4<double>(-133.438, -68.66, 250.556));
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(37.195, -718.269, -455.05), Vector4<double>(188.309, -17.704, 618.857));

    for (int j = 0; j < i; ++j) {
        if (this->vector1) this->vector1->push(new Vector4Event(-1, -1, pv[j].first));
        if (this->vector2) this->vector2->push(new Vector4Event(-1, -1, pv[j].second));
        //if (this->vector1) this->vector1->push(new Vector4Event(-1, -1, pv[j].second));
        //if (this->vector2) this->vector2->push(new Vector4Event(-1, -1, pv[j].first));
        QTest::qWait(100);
        if (this->applyVectors) this->applyVectors->push(new DigitalChangedEvent(-1, -1, true));
    }

}


void MainWindow::addPointsAverage() {
    std::cout << "MainWindow::addPointsAverage()" << std::endl;

    if (this->applyVectors) this->applyVectors->push(new DigitalChangedEvent(-1, -1, true));
    QTest::qWait(100);
    for (int i = 0; i < 8; ++i) {
        pair<Vector4<double>, Vector4<double> > pv = this->generateVector4Pair();
       // cout << pv.first << "  " << pv.second << endl;
        if (this->vector1) this->vector1->push(new Vector4Event(-1, -1, pv.first));
        if (this->vector2) this->vector2->push(new Vector4Event(-1, -1, pv.second));
        QTest::qWait(100);
    }
    if (this->applyVectors) this->applyVectors->push(new DigitalChangedEvent(-1, -1, false));
}


void MainWindow::clearPoints() {
    std::cout << "MainWindow::clearPoints()" << std::endl;
    if (this->clearVectors) this->clearVectors->push(new DigitalChangedEvent(-1, -1, true));
}


void MainWindow::queryMatrix() {
    std::cout << "MainWindow::queryMatrix()" << std::endl;
    if (this->calcCalibratedMatrix) this->calcCalibratedMatrix->push(new DigitalChangedEvent(-1, -1, true));
    if (this->sendCalibratedMatrix) this->sendCalibratedMatrix->push(new DigitalChangedEvent(-1, -1, true));
}


void MainWindow::storeMatrix() {
    std::cout << "MainWindow::storeMatrix()" << std::endl;
    this->printMatrix(createTransfMatrix());
    if (this->saveCalibratedMatrix) this->saveCalibratedMatrix->push(new DigitalChangedEvent(-1, -1, true));
}


void MainWindow::restoreMatrix() {
    std::cout << "MainWindow::restoreMatrix()" << std::endl;
    if (this->loadCalibratedMatrix) this->loadCalibratedMatrix->push(new DigitalChangedEvent(-1, -1, true));
}


void MainWindow::exit() {
    std::cout << "MainWindow::exit()" << std::endl;
    this->close();
}


void MainWindow::calibrationMatrix(const Matrix4Event * e) {
    cout << e << endl;
    this->ui->textEdit->insertHtml("<b>Original transformation matrix:</b>");
    this->printMatrix(createTransfMatrix());
    this->ui->textEdit->insertHtml("<b>Calculated matrix:</b>");
    this->printMatrix(e->getPayload());
/*
    pair<Vector4<double>, Vector4<double> > pv[100];
    int i = 0;
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(36.9249, -718.576, -455.495), Vector4<double>(431.334, -242.123, 1059.94));
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(37.4273, -718.172, -455.579), Vector4<double>(-168.369, 279.603, 813.793));
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(37.2145, -718.236, -454.716), Vector4<double>(-133.438, -68.66, 250.556));
    pv[i++] = pair<Vector4<double>, Vector4<double> >(Vector4<double>(37.195, -718.269, -455.05), Vector4<double>(188.309, -17.704, 618.857));
    for (int j = 0; j < i; ++j) {
        this->printVectors(pv[j].first, e->getPayload()*pv[j].first);
    }
    */
}


void MainWindow::processTUIEvents() {
    //std::cout << "MainWindow::processTUIEvents()" << std::endl;
    processEvents();

}


Matrix4<double> createRotXMatrix(double theta) {
    double c = cos(theta);
    double s = sin(theta);
    Matrix4<double> m;
    m.setRow(0, 1.0, 0,  0,   0);
    m.setRow(1,   0, c, -s,   0);
    m.setRow(2,   0, s,  c,   0);
    m.setRow(3,   0, 0,  0, 1.0);
    return m;
}


Matrix4<double> createRotYMatrix(double theta) {
    double c = cos(theta);
    double s = sin(theta);
    Matrix4<double> m;
    m.setRow(0,  c, 0, s,   0);
    m.setRow(1,  0, 1, 0,   0);
    m.setRow(2, -s, 0, c,   0);
    m.setRow(3,  0, 0, 0, 1.0);
    return m;
}


Matrix4<double> createRotZMatrix(double theta) {
    double c = cos(theta);
    double s = sin(theta);
    Matrix4<double> m;
    m.setRow(0,  c, -s, 0,   0);
    m.setRow(1,  s,  c, 0,   0);
    m.setRow(2,  0,  0, 1,   0);
    m.setRow(3,  0,  0, 0, 1.0);
    return m;
}


Matrix4<double> createScaleMatrix(double sx, double sy, double sz) {
    Matrix4<double> m;
    m.setRow(0,  sx, 0,  0, 0);
    m.setRow(1,  0,  sy, 0, 0);
    m.setRow(2,  0,  0, sz, 0);
    m.setRow(3,  0,  0,  0, 1.0);
    return m;
}


Matrix4<double> createTransMatrix(double tx, double ty, double tz) {
    Matrix4<double> m;
    m.setRow(0, 1.0,   0,   0, tx);
    m.setRow(1,   0, 1.0,   0, ty);
    m.setRow(2,   0,   0, 1.0, tz);
    m.setRow(3,   0,   0,   0, 1.0);
    return m;
}


Matrix4<double> MainWindow::createTransfMatrix() const {
    Matrix4<double> rx = createRotXMatrix(this->ui->RXDoubleSpinBox->value()/180*M_PI);
    Matrix4<double> ry = createRotYMatrix(this->ui->RYDoubleSpinBox->value()/180*M_PI);
    Matrix4<double> rz = createRotZMatrix(this->ui->RZDoubleSpinBox->value()/180*M_PI);
    Matrix4<double> sc = createScaleMatrix(this->ui->SXDoubleSpinBox->value(), this->ui->SYDoubleSpinBox->value(), this->ui->SZDoubleSpinBox->value());
    Matrix4<double> tr = createTransMatrix(this->ui->TXDoubleSpinBox->value(), this->ui->TYDoubleSpinBox->value(), this->ui->TZDoubleSpinBox->value());
    return tr*(rx*ry*rz*sc);
}


Vector4<double> product(const Matrix4<double> & m, const Vector4<double> & v) {
    Vector4<double> r;
    for (int i = 0; i < 3; ++i) {
        r[i] = m[i][0]*v[0] + m[i][1]*v[1] + m[i][2]*v[2] + m[i][3];
    }
    return r;
}

Vector4<double> product(const Vector4<double> & v, const Matrix4<double> & m) {
    Vector4<double> r;
    for (int i = 0; i < 3; ++i) {
        r[i] = v[0]*m[0][i] + v[1]*m[1][i] + v[2]*m[2][i] + m[3][i];
    }
    return r;
}



double getRandomDouble(double range) {
    return 2.0*range*(static_cast<double>(qrand())/static_cast<double>(RAND_MAX) - 0.5);
}


Vector4<double> getRandomVector4(double range) {
    return Vector4<double>(getRandomDouble(range), getRandomDouble(range), getRandomDouble(range));
}


pair<Vector4<double>, Vector4<double> > MainWindow::generateVector4Pair() const {
    double scale = this->ui->PointScaleDoubleSpinBox->value();
    double error = this->ui->PointErrorDoubleSpinBox->value();

    Matrix4<double> transfMatrix = this->createTransfMatrix();
    Vector4<double> errVec1 = getRandomVector4(error);
    Vector4<double> errVec2 = getRandomVector4(error);
    Vector4<double> vec1 = getRandomVector4(scale) + errVec1;
    Vector4<double> vec2 = product(transfMatrix, vec1) + errVec2;
    //Vector4<double> vec1 = getRandomVector4(scale);
    //Vector4<double> vec2 = product(transfMatrix, vec1);

    return pair<Vector4<double>, Vector4<double> >(vec1, vec2);
}


void MainWindow::printMatrix(const Matrix4<double> & m) const {
    QString str;
    str += "<table border=\"1\" cellspacing=\"2\" cellpadding=\"2\">";
    for (int y = 0; y < 4; ++y) {
        str += "<tr>";
        for (int x = 0; x < 4; ++x) {
            str += "<td>";
            str += QString(" %1 ").arg(m[y][x], 0, 'f', 12);

            str += "</td>";
        }
        str += "</tr>";
    }
    str += "</table>";
    this->ui->textEdit->insertHtml(str);

}

void MainWindow::printVectors(const Vector4<double> & v1, const Vector4<double> & v2) const {
    QString str;
    str += "<table border=\"1\" cellspacing=\"2\" cellpadding=\"2\">";
    for (int y = 0; y < 3; ++y) {
        str += "<tr>";

        str += "<td>";
        str += QString(" %1 ").arg(v1[y], 0, 'f', 12);
        str += "</td>";
        str += "<td>";
        str += QString(" %1 ").arg(v2[y], 0, 'f', 12);
        str += "</td>";

        str += "</tr>";
    }
    str += "</table>";
    this->ui->textEdit->insertHtml(str);

}
