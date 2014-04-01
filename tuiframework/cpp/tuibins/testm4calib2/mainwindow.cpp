#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;
using namespace tuiframework;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    calcCalibratedMatrix(0),
    clearVectors(0),
    applyVectors(0),
    sendCalibratedMatrix(0),
    loadCalibratedMatrix(0),
    saveCalibratedMatrix(0)
{
    ui->setupUi(this);

    connect(&timer, SIGNAL(timeout()), this, SLOT(processTUIEvents()));
    timer.start();
    connect(ui->ConnectPushButton, SIGNAL(clicked()), this, SLOT(connectTUIServer()));
    connect(ui->DisconnectPushButton, SIGNAL(clicked()), this, SLOT(disconnectTUIServer()));
    connect(ui->applyPushButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->calcPushButton, SIGNAL(clicked()), this, SLOT(calc()));
    connect(ui->clearPushButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->loadPushButton, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->savePushButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->sendPushButton, SIGNAL(clicked()), this, SLOT(send()));
    connect(ui->exitPushButton, SIGNAL(clicked()), this, SLOT(exit()));
}

MainWindow::~MainWindow()
{
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
                CONNECT(Vector4Event, "Vector4Receiver", "v1", MainWindow, this, &MainWindow::v1Received);
                CONNECT(Vector4Event, "Vector4Receiver", "v2", MainWindow, this, &MainWindow::v2Received);

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


void MainWindow::v1Received(const Vector4Event * e) {
    this->v1 = e->getPayload();
}


void MainWindow::v2Received(const Vector4Event * e) {
    this->v2 = e->getPayload();

    std::cout << "(" << this->v1 << ")   (" << this->v2 << ")" << std::endl;
}

void MainWindow::disconnectTUIServer() {
    std::cout << "MainWindow::disconnectTUIServer()" << std::endl;
}


void MainWindow::apply() {
    std::cout << "apply" << std::endl;
    if (this->applyVectors) this->applyVectors->push(new DigitalChangedEvent(-1, -1, true));
}

void MainWindow::calc() {
    std::cout << "calc" << std::endl;
    if (this->calcCalibratedMatrix) this->calcCalibratedMatrix->push(new DigitalChangedEvent(-1, -1, true));
}

void MainWindow::clear() {
    std::cout << "clear" << std::endl;
    if (this->clearVectors) this->clearVectors->push(new DigitalChangedEvent(-1, -1, true));
}

void MainWindow::load() {
    std::cout << "load" << std::endl;
    if (this->loadCalibratedMatrix) this->loadCalibratedMatrix->push(new DigitalChangedEvent(-1, -1, true));
}

void MainWindow::save() {
    std::cout << "save" << std::endl;
    if (this->saveCalibratedMatrix) this->saveCalibratedMatrix->push(new DigitalChangedEvent(-1, -1, true));
}

void MainWindow::send() {
    std::cout << "save" << std::endl;
    if (this->sendCalibratedMatrix) this->sendCalibratedMatrix->push(new DigitalChangedEvent(-1, -1, true));
}

void MainWindow::exit() {
    std::cout << "MainWindow::exit()" << std::endl;
    this->close();
}


void MainWindow::processTUIEvents() {
    //std::cout << "MainWindow::processTUIEvents()" << std::endl;
    processEvents();

}
