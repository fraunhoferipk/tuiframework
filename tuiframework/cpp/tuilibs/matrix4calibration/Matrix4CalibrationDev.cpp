/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    
    The TUIFramework library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    The TUIFramework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with the TUIFramework.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Matrix4CalibrationDev.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/core/Exception.h>
#include <tuiframework/server/DeviceConfig.h>


#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

#include <cstdlib> // GCC 4.3 related build problem

#include <ctime>

#include <iostream>
#include <fstream>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace tuiframework;
using namespace std;

#define PortNR_Vector1              1
#define PortNR_Vector2              2
#define PortNR_CalibratedMatrix     3
#define PortNR_ApplyVectors         4
#define PortNR_ClearVectors         5
#define PortNR_LoadCalibratedMatrix 6
#define PortNR_SaveCalibratedMatrix 7
#define PortNR_SendCalibratedMatrix 8
#define PortNR_CalcCalibratedMatrix 9

namespace tuidevices {

static void addPort(const Port & port, int portNr, map<string, Port> & portMap, map<string, int> & namePortNrMap) {
    portMap[port.getName()] = port;
    namePortNrMap[port.getName()] = portNr;
}


IDevice * Matrix4CalibrationDev::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new Matrix4CalibrationDev(*deviceConfig);
}


std::string Matrix4CalibrationDev::getDeviceName() {
    return "Matrix4CalibrationDev";
}


Matrix4CalibrationDev::Matrix4CalibrationDev(const DeviceConfig & deviceConfig) :
    senderThreadActive(false),
    receiverThreadActive(false),
    curVec1Avail(false),
    curVec2Avail(false) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;
    map<string, int> namePortNrMap;
    
    addPort(Port("Vector1",               "Vector3",        Port::Sink, ""),    PortNR_Vector1,                 portMap, namePortNrMap);
    addPort(Port("Vector2",               "Vector3",        Port::Sink, ""),    PortNR_Vector2,                 portMap, namePortNrMap);
    addPort(Port("CalibratedMatrix",      "Matrix4",        Port::Source, ""),  PortNR_CalibratedMatrix,        portMap, namePortNrMap);
    addPort(Port("ApplyVectors",          "DigitalChannel", Port::Sink, ""),    PortNR_ApplyVectors,            portMap, namePortNrMap);
    addPort(Port("ClearVectors",          "DigitalChannel", Port::Sink, ""),    PortNR_ClearVectors,            portMap, namePortNrMap);
    addPort(Port("LoadCalibratedMatrix",  "DigitalChannel", Port::Sink, ""),    PortNR_LoadCalibratedMatrix,    portMap, namePortNrMap);
    addPort(Port("SaveCalibratedMatrix",  "DigitalChannel", Port::Sink, ""),    PortNR_SaveCalibratedMatrix,    portMap, namePortNrMap);
    addPort(Port("SendCalibratedMatrix",  "DigitalChannel", Port::Sink, ""),    PortNR_SendCalibratedMatrix,    portMap, namePortNrMap);
    addPort(Port("CalcCalibratedMatrix",  "DigitalChannel", Port::Sink, ""),    PortNR_CalcCalibratedMatrix,    portMap, namePortNrMap);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName("Matrix4CalibrationDev");
    deviceType.setDescription("");

    this->deviceDescriptor.setDeviceType(deviceType);
    this->deviceDescriptor.setNameChannelNrMap(namePortNrMap);
    
    try {
        this->initialSend = deviceConfig.getParameterGroup().getInt("CalibratedMatrix|InitialSend") != 0;
        TFDEBUG("initialSend = " << this->initialSend);
        
        this->calibratedMatrixPath = deviceConfig.getParameterGroup().getString("CalibratedMatrix|Path");
        TFDEBUG("calibratedMatrixPath = " << this->calibratedMatrixPath);
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
    }
}


Matrix4CalibrationDev::~Matrix4CalibrationDev() {
}


bool Matrix4CalibrationDev::deviceExecute() {
    bool receiver_thread_created = false;
    try {
        {
            int rc = pthread_create(&this->receiverThread, NULL, Matrix4CalibrationDev::receiverThread_run, this);
            if (rc) {
                throw Exception(TFSTR("pthread_create() failed, return code:" << rc), __FILE__, __LINE__);
            }
            receiver_thread_created = true;
        }

        {
            int rc = pthread_create(&this->senderThread, NULL, Matrix4CalibrationDev::senderThread_run, this);
            if (rc) {
                throw Exception(TFSTR("pthread_create() failed, return code:" << rc), __FILE__, __LINE__);
            }
        }
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
        if (receiver_thread_created) {
            pthread_cancel(this->receiverThread);
        }
        return false;
    }
    return true;
}


void Matrix4CalibrationDev::deviceStop() {
    if (this->senderThreadActive) {
        this->senderThreadActive = false;
        pthread_cancel(this->senderThread);
        pthread_join(this->senderThread, NULL);
    }
    
    if (this->receiverThreadActive) {
        this->receiverThreadActive = false;
        pthread_cancel(this->receiverThread);
        this->receiverEventQueue.cleanupAfterWaitCanceled(); 
        pthread_join(this->receiverThread, NULL);
    }
}


void Matrix4CalibrationDev::push(tuiframework::IEvent * event) {
    this->receiverEventQueue.push(event);
}


void Matrix4CalibrationDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void Matrix4CalibrationDev::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void Matrix4CalibrationDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & Matrix4CalibrationDev::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * Matrix4CalibrationDev::senderThread_run(void * arg) {
    printf("Matrix4CalibrationDev - input loop thread started\n");
    Matrix4CalibrationDev * dummyDevice = static_cast<Matrix4CalibrationDev *>(arg);
    dummyDevice->senderThreadLoop();
    return 0;
}


void Matrix4CalibrationDev::senderThreadLoop() {
    this->senderThreadActive = true;
    while (this->senderThreadActive) {
#ifndef _WIN32
        fd_set rfds;
        struct timeval tv;
        int retval;

        FD_ZERO(&rfds);
        FD_SET(0, &rfds);

        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        retval = select(1, &rfds, 0, 0, &tv);
#endif
#ifdef _WIN32
        Sleep(100);
#endif
        if (this->eventSink) {
            {
                int a = 0;
                float c = 0;
                map<std::string, int>::iterator i = this->deviceDescriptor.getNameChannelNrMap().begin();
                map<std::string, int>::iterator e = this->deviceDescriptor.getNameChannelNrMap().end();
                while (i != e && a < 1) {
                    Matrix4Event * event = new Matrix4Event();
                    event->setAddress(EPAddress(this->entityID, (*i).second));
                    Matrix4<double> mat;
                    /*
                    mat[0][0] = 1;
                    mat[0][1] = 0;
                    mat[0][2] = 0;
                    mat[0][3] = 0;
                    mat[1][0] = 0;
                    mat[1][1] = 1;
                    mat[1][2] = 0;
                    mat[1][3] = 0;
                    mat[2][0] = 0;
                    mat[2][1] = 0;
                    mat[2][2] = 1;
                    mat[2][3] = 0;
                    mat[3][0] = c;
                    mat[3][1] = 0;
                    mat[3][2] = 0;
                    mat[3][3] = 1;
                    */
                    
                    mat[0][0] = 1;
                    mat[0][1] = 0.5;
                    mat[0][2] = 1;
                    mat[0][3] = 1;
                    
                    mat[1][0] = 1;
                    mat[1][1] = 1;
                    mat[1][2] = 1;
                    mat[1][3] = -1;
                    
                    mat[2][0] = 1;
                    mat[2][1] = 2;
                    mat[2][2] = 1;
                    mat[2][3] = -1;
                    
                    mat[3][0] = 2;
                    mat[3][1] = 2;
                    mat[3][2] = 1;
                    mat[3][3] = 1;
                    
                    event->setPayload(mat);
                    eventSink->push(event);
                    c += 1.0f;
                    ++a;
                    ++i;
                }
            }
        }
    }
}


void Matrix4CalibrationDev::handleVector1(Vector3Event * event) {
    this->curVec1 = event->getPayload();
    this->curVec1Avail = true;
}


void Matrix4CalibrationDev::handleVector2(Vector3Event * event) {
    this->curVec2 = event->getPayload();
    this->curVec2Avail = true;
}


void Matrix4CalibrationDev::applyVectors(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }
    if (this->curVec1Avail && this->curVec2Avail) {
        this->vecPairVector.push_back(pair<Vector3<double>, Vector3<double> >(this->curVec1, this->curVec2));
        this->curVec1Avail = false;
        this->curVec2Avail = false;
    }
}


void Matrix4CalibrationDev::clearVectors(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }
    this->vecPairVector.clear();
}


void Matrix4CalibrationDev::loadCalibratedMatrix(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }
    
    std::fstream fs;
    fs.open (this->calibratedMatrixPath.c_str(), std::fstream::in);
    if (fs.is_open()) {
        fs >> this->matrix;
        fs.close();
    } else {
        //@@TODO
    }
}


void Matrix4CalibrationDev::saveCalibratedMatrix(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }
    
    std::fstream fs;
    fs.open (this->calibratedMatrixPath.c_str(), std::fstream::in);
    if (fs.is_open()) {
        fs >> this->matrix;
        fs.close();
    } else {
        //@@TODO
    }
}


void Matrix4CalibrationDev::sendCalibratedMatrix(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }
    Matrix4Event * e = new Matrix4Event();
    e->setAddress(EPAddress(this->entityID, PortNR_SendCalibratedMatrix));
    e->setPayload(this->matrix);
    this->eventSink->push(e);
}

static double epsilon = 0.001;

/*
static double det(const Vector3<double> & a, const Vector3<double> & b, const Vector3<double> & c) {   
    return a[0]*b[1]*c[2] + a[2]*b[0]*c[1] + a[1]*b[2]*c[0] - a[0]*b[2]*c[1] - a[2]*b[1]*c[0] - a[1]*b[0]*c[2];
}

void Matrix4CalibrationDev::calculateCalibratedMatrix3() {
    Vector3<double> a[3];
    Vector3<double> b[3];
    vector<pair<Vector3<double>, Vector3<double> > >::const_iterator i = this->vecPairVector.begin();
    vector<pair<Vector3<double>, Vector3<double> > >::const_iterator j = this->vecPairVector.begin();
    vector<pair<Vector3<double>, Vector3<double> > >::const_iterator e = this->vecPairVector.end();
    while (i != e) {
            // calculate vector difference to eliminate translation offset
        for (int k = 0; k < 3; ++k) {
            j = i;
            ++i;
            if (i == e) {
                break;
            }
            a[k] = (*i).first - (*j).first;
            b[k] = (*i).second - (*j).second;
        }
        
        if (k == 3) {
                // transpose
            Vector3<double> c[3];
            Vector3<double> d[3];
            for (int l = 0; l < 3; ++l) {
                for (int m = 0; m < 3; ++m) {
                    c[m][l] = a[l][m];
                    d[m][l] = b[l][m];
                }
            }
                // all vectors must not be linear dependent
                // check this with the denominator determinant
                // Cramer's rule is used to solve the system of linear equations
            double n1 = det(c[0], c[1], c[2]);
            double n2 = det(d[0], d[1], d[2]);
            
            if (n1 < epsilon && -n1 < epsilon) {
                break;
            }
            
            Vector3<double> m[3];
            m[0][0] = det(d[0], c[1], c[2]);
            m[1][0] = det(c[0], d[0], c[2]);
            m[2][0] = det(c[0], c[1], d[0]);
            
            m[0][1] = det(d[1], c[1], c[2]);
            m[1][1] = det(c[0], d[1], c[2]);
            m[2][1] = det(c[0], c[1], d[1]);
            
            m[0][2] = det(d[2], c[1], c[2]);
            m[1][2] = det(c[0], d[2], c[2]);
            m[2][2] = det(c[0], c[1], d[2]);            
            
            
        }
    }
}
*/

void Matrix4CalibrationDev::calculateCalibratedMatrix(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }    
    vector<Matrix4<double> > mvec;
    {
        Matrix4<double> a;
        Matrix4<double> b;
        vector<pair<Vector3<double>, Vector3<double> > >::const_iterator i = this->vecPairVector.begin();
        vector<pair<Vector3<double>, Vector3<double> > >::const_iterator e = this->vecPairVector.end();
        while (i != e) {
            int k = 0;
            while (k < 4 && i != e) {
                a.setRow(k, (*i).first[0], (*i).first[1], (*i).first[2], 1.0);
                b.setRow(k, (*i).second[0], (*i).second[1], (*i).second[2], 1.0);
                ++k;
                ++i;
            }
            
            if (k == 4) {
                double n1 = a.getDeterminant();
                if (n1 < epsilon && -n1 < epsilon) {
                    break;
                }
                double n2 = b.getDeterminant();
                if (n2 < epsilon && -n2 < epsilon) {
                    break;
                }
                
                Matrix4<double> m;
                
                for (int x = 0; x < 2; ++x) {
                    for (int y = 0; y < 3; ++y) {
                        Matrix4<double> c = a;
                        c.setColumn(y, b[0][x], b[1][x], b[2][x], b[3][x]);
                        m[y][x] = c.getDeterminant()/n1;
                    }
                }
                m[0][3] = 0;
                m[1][3] = 0;
                m[2][3] = 0;
                m[3][3] = 1.0;
                
                mvec.push_back(m);
            }
            ++i;
        }
    }
    
    if (mvec.size() == 0) {
        return;
    }
    
    // calculate mean matrix
    
    double absX = 0;
    double absY = 0;
    double absZ = 0;
    double count = static_cast<double>(mvec.size());
    
    Matrix4<double> r;
    r.setZero();
    
    {
        vector<Matrix4<double> >::const_iterator i = mvec.begin();
        vector<Matrix4<double> >::const_iterator e = mvec.end();
        while (i != e) {
            absX += (*i).getColumnAbs(0, 3);
            absY += (*i).getColumnAbs(1, 3);
            absZ += (*i).getColumnAbs(2, 3);
            r += *i;
            ++i;
        }
    }
    
    absX /= count;
    absY /= count;
    absZ /= count;
    
    r.scaleColumnAbs(0, 3, 1.0);
    r.scaleColumnAbs(1, 3, 1.0);
    r.scaleColumnAbs(2, 3, 1.0);
    
    Vector3<double> vx(r[0][0], r[1][0], r[2][0]);
    Vector3<double> vy(r[0][1], r[1][1], r[2][1]);
    Vector3<double> vz(r[0][2], r[1][2], r[2][2]);
    
    double sxy = 0.0;
    double sxz = 0.0;
    double syz = 0.0;
    double maxCos = 0.0;
    
    while (true) {
        
        sxy = vx*vy;
        sxz = vx*vz;
        syz = vy*vz;
        
        maxCos = fabs(sxy);
        if (maxCos > fabs(sxz)) {
            maxCos = fabs(sxz);
        }
        if (maxCos > fabs(syz)) {
            maxCos = fabs(syz);
        }
        
        if (maxCos < epsilon) {
            break;
        }
        
        Vector3<double> nvx = vx - 0.5*sxy*vy - 0.5*sxz*vz;
        Vector3<double> nvy = vy - 0.5*sxy*vx - 0.5*syz*vz;
        Vector3<double> nvz = vz - 0.5*sxz*vx - 0.5*syz*vy;
    }
    
    for (int i = 0; i < 3; ++i) {
        r[i][0] = vx[i];
        r[i][1] = vy[i];
        r[i][2] = vz[i];
    }
    
    r.scaleColumnAbs(0, 3, absX);
    r.scaleColumnAbs(1, 3, absY);
    r.scaleColumnAbs(2, 3, absZ);
    r.scaleRowAbs(3, 4, 1.0/count);
    
    this->matrix = r;
}


void Matrix4CalibrationDev::receiverThreadLoop() {
    this->receiverThreadActive = true;
    while (receiverThreadActive) {
        this->receiverEventQueue.waitForData();
        IEvent * event = this->receiverEventQueue.pop();
        if (event) {
            if (event->getAddressTypeID() == EPAddress::AddressTypeID()) {
                IEventMsg<EPAddress> * e = static_cast<IEventMsg<EPAddress> *>(event);
                switch (e->getAddress().getPortNr()) {
                    case PortNR_Vector1: {
                        this->handleVector1(static_cast<Vector3Event *>(event));
                    } break;
                    case PortNR_Vector2: {
                        this->handleVector2(static_cast<Vector3Event *>(event));
                    } break;
                    case PortNR_ApplyVectors: {
                        this->applyVectors(static_cast<DigitalChangedEvent *>(event));
                    } break;
                    case PortNR_ClearVectors: {
                        this->clearVectors(static_cast<DigitalChangedEvent *>(event));
                    } break;
                    case PortNR_LoadCalibratedMatrix: {
                        this->loadCalibratedMatrix(static_cast<DigitalChangedEvent *>(event));
                    } break;
                    case PortNR_SaveCalibratedMatrix: {
                        this->saveCalibratedMatrix(static_cast<DigitalChangedEvent *>(event));
                    } break;
                    case PortNR_SendCalibratedMatrix: {
                    this->sendCalibratedMatrix(static_cast<DigitalChangedEvent *>(event));
                    } break;
                    case PortNR_CalcCalibratedMatrix: {
                    this->calculateCalibratedMatrix(static_cast<DigitalChangedEvent *>(event));
                    } break;
                    default: {
                    }
                };
            }
            cout << "Matrix4CalibrationDev: " << event->getEventTypeID() << " " << event << endl;
        }
    }
}


void * Matrix4CalibrationDev::receiverThread_run(void * arg) {
    TFINFO("Matrix4CalibrationDev - receiver thread");
    Matrix4CalibrationDev * dummyDevice = static_cast<Matrix4CalibrationDev *>(arg);
    dummyDevice->receiverThreadLoop();
    return 0;
}

}


