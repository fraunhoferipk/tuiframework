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
    vec1Size(0),
    vec2Size(0),
    trackVectors(false),
    premultiply(false),
    useAverage(false) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;
    map<string, int> namePortNrMap;
    
    addPort(Port("Vector1",               "Vector4",        Port::Sink, ""),    PortNR_Vector1,                 portMap, namePortNrMap);
    addPort(Port("Vector2",               "Vector4",        Port::Sink, ""),    PortNR_Vector2,                 portMap, namePortNrMap);
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
        
        this->premultiply = deviceConfig.getParameterGroup().getInt("CalibratedMatrix|Pre-multiply") != 0;
        TFINFO("pre-multiply = " << this->premultiply);
        
        this->useAverage = deviceConfig.getParameterGroup().getInt("ApplyVectors|useAverage") != 0;
        TFINFO("useAverage = " << this->useAverage);
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
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
        if (receiver_thread_created) {
            pthread_cancel(this->receiverThread);
        }
        return false;
    }
    
    DigitalChangedEvent e(-1, -1, true);
    if (this->calibratedMatrixPath.size()) {
        this->loadCalibratedMatrix(&e);
    }
    if (this->initialSend) {
        this->sendCalibratedMatrix(&e);
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



void Matrix4CalibrationDev::handleVector1(Vector4Event * event) {
    if (this->useAverage) {
        if (this->trackVectors) {
            this->vec1 += event->getPayload();
            ++this->vec1Size;
            TFDEBUG("track vec1: " << event->getPayload())
        }
    } else {
        this->vec1 = event->getPayload();
        //TFDEBUG("Vec1: " << this->vec1)
        this->vec1Size = 1;
    }
}


void Matrix4CalibrationDev::handleVector2(Vector4Event * event) {
    if (this->useAverage) {
        if (this->trackVectors) {
            this->vec2 += event->getPayload();
            ++this->vec2Size;
            TFDEBUG("track vec2: " << event->getPayload())
        }
    } else {
        this->vec2 = event->getPayload();
        //TFDEBUG("Vec2: " << this->vec2)
        this->vec2Size = true;
    }
}

void Matrix4CalibrationDev::applyVectors() {
    if (this->vec1Size && this->vec2Size) {
        this->vec1.scale(1.0/static_cast<double>(this->vec1Size));
        this->vec2.scale(1.0/static_cast<double>(this->vec2Size));
        this->vecPairVector.push_back(pair<Vector4<double>, Vector4<double> >(this->vec1, this->vec2));
        TFDEBUG("apply vec1: " << this->vec1 << "   vec2: " << this->vec2)
    }
    
    this->vec1 = Vector4<double>();
    this->vec2 = Vector4<double>();
    this->vec1Size = 0;
    this->vec2Size = 0;
}


void Matrix4CalibrationDev::applyVectors(DigitalChangedEvent * event) {
    TFDEBUG("applyVectors")
    if (this->useAverage) {
        if (this->trackVectors != event->getPayload()) {
            if ( ! event->getPayload()) {
                this->applyVectors();
            }
            this->trackVectors = event->getPayload();
        }
        
    } else {
        if ( ! event->getPayload()) {
            return;
        }
        this->applyVectors();
    }
}


void Matrix4CalibrationDev::clearVectors(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }
    TFDEBUG("clearVectors")
    this->vecPairVector.clear();
}


void Matrix4CalibrationDev::loadCalibratedMatrix(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }
    TFDEBUG("loadCalibratedMatrix")
    std::fstream fs;
    fs.open(this->calibratedMatrixPath.c_str(), std::fstream::in);
    if (fs.is_open()) {
        fs >> this->matrix;
        fs.close();
    } else {
        TFERROR("")
        //@@TODO
    }
}


void Matrix4CalibrationDev::saveCalibratedMatrix(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }
    TFDEBUG("saveCalibratedMatrix")
    std::fstream fs;
    fs.open(this->calibratedMatrixPath.c_str(), std::fstream::out);
    if (fs.is_open()) {
        fs << this->matrix;
        fs.close();
    } else {
        TFERROR("")
        //@@TODO
    }
}


void Matrix4CalibrationDev::sendCalibratedMatrix(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }
    TFDEBUG("sendCalibratedMatrix")
    Matrix4Event * e = new Matrix4Event();
    e->setAddress(EPAddress(this->entityID, PortNR_CalibratedMatrix));
    e->setPayload(this->matrix);
    this->eventSink->push(e);
}

static double epsilon = 0.000000001;

void Matrix4CalibrationDev::calculateCalibratedMatrix(DigitalChangedEvent * event) {
    if ( ! event->getPayload()) {
        return;
    }    
    TFDEBUG("calculateCalibratedMatrix")
    vector<Matrix4<double> > mvec;
    {
        Matrix4<double> a;
        Matrix4<double> b;
        vector<pair<Vector4<double>, Vector4<double> > >::const_iterator i = this->vecPairVector.begin();
        vector<pair<Vector4<double>, Vector4<double> > >::const_iterator e = this->vecPairVector.end();
        while (i != e) {
            int k = 0;
            while (k < 4 && i != e) {
                TFDEBUG("k =  " << k)
                TFDEBUG("Vector1: " << (*i).first)
                TFDEBUG("Vector2: " << (*i).second)
                a.setRow(k, (*i).first[0], (*i).first[1], (*i).first[2], 1.0);
                b.setRow(k, (*i).second[0], (*i).second[1], (*i).second[2], 1.0);
                ++k;
                ++i;
            }
            
            if (k == 4) {
                TFDEBUG("a: " << a)
                TFDEBUG("b: " << b)
                double n1 = a.getDeterminant();
                TFDEBUG("det a = " << n1)
                if (n1 < epsilon && -n1 < epsilon) {
                    break;
                }
                double n2 = b.getDeterminant();
                TFDEBUG("det b = " << n2)
                if (n2 < epsilon && -n2 < epsilon) {
                    break;
                }
                
                Matrix4<double> m;
                
                for (int x = 0; x < 4; ++x) {
                    for (int y = 0; y < 4; ++y) {
                        Matrix4<double> c = a;
                        c.setColumn(y, b[0][x], b[1][x], b[2][x], b[3][x]);
                        m[y][x] = c.getDeterminant()/n1;
                        if (fabs(m[y][x]) < 0.0000000001) {
                            m[y][x] = 0;
                        }
                    }
                }
                
                mvec.push_back(m);
                TFDEBUG("calculated matrix =  " << m)
            }
        }
    }
    
    if (mvec.size() == 0) {
        return;
    }
    
    {
        Matrix4<double> r;
        if (mvec.size() == 1) {
            r = mvec[0];
        } else {
            map<double, int> errorMap;
            for (int n = 0; n < mvec.size(); ++n) {
                const Matrix4<double> & m = mvec[n];
                
                double vabs = 0;
                vector<pair<Vector4<double>, Vector4<double> > >::const_iterator i = this->vecPairVector.begin();
                vector<pair<Vector4<double>, Vector4<double> > >::const_iterator e = this->vecPairVector.end();
                while (i != e) {
                    Vector4<double> v = (*i).first*m;
                    v -= (*i).second;
                    vabs += v.getAbs();
                    
                    ++i;
                }
                errorMap[vabs] = n;
            }
            
            int mpos = 0;
            map<double, int>::iterator i = errorMap.begin();
            map<double, int>::iterator e = errorMap.end();
            while (i != e) {
                TFDEBUG("errorMap pos " << mpos << ": = " << (*i).first << " -> " << (*i).second)            
                ++i;
            }
            
            int mvecIndex = (*errorMap.begin()).second;
            r = mvec[mvecIndex];
        }
        
        if (this->premultiply) {
            this->matrix = Matrix4<double>::transposed(r);
        } else {
            this->matrix = r;
        }
    }
    
    TFINFO("Matrix = " << this->matrix)
    {
        vector<pair<Vector4<double>, Vector4<double> > >::const_iterator i = this->vecPairVector.begin();
        vector<pair<Vector4<double>, Vector4<double> > >::const_iterator e = this->vecPairVector.end();
        while (i != e) {
            TFDEBUG((*i).first << " -> " << (*i).first*this->matrix << " Ref: " << (*i).second);
            ++i;
        }
    }
    
    /*
    // calculate mean matrix
    
    double absX = 0;
    double absY = 0;
    double absZ = 0;
    double count = static_cast<double>(mvec.size());
    TFDEBUG("mvec size =  " << mvec.size())
    
    Matrix4<double> r;
    r.setZero();
    
    {
        vector<Matrix4<double> >::const_iterator i = mvec.begin();
        vector<Matrix4<double> >::const_iterator e = mvec.end();
        while (i != e) {
            //absX += (*i).getColumnAbs(0, 3);
            //absY += (*i).getColumnAbs(1, 3);
            //absZ += (*i).getColumnAbs(2, 3);
            r += *i;
            ++i;
        }
    }
    this->matrix = Matrix4<double>::transposed(r);
    */
    //
    
    //
  /*  
#ifdef interpolate_mtx
    absX = r.getRowAbs(0, 3)/count;
    absY = r.getRowAbs(1, 3)/count;
    absZ = r.getRowAbs(2, 3)/count;
    
    Vector4<double> vx(r[0][0], r[0][1], r[0][2]);
    Vector4<double> vy(r[1][0], r[1][1], r[1][2]);
    Vector4<double> vz(r[2][0], r[2][1], r[2][2]);
    Vector4<double> tr(r[3][0], r[3][1], r[3][2]);
    vx.scaleToAbs(1.0);
    vy.scaleToAbs(1.0);
    vz.scaleToAbs(1.0);
    //tr.scale(1.0/count);
    
    double sxy = 0.0;
    double sxz = 0.0;
    double syz = 0.0;
    double maxCos = 0.0;
    int l = 0;
    while (l < 10) {
        TFDEBUG("vx =  " << vx)
        TFDEBUG("vy =  " << vy)
        TFDEBUG("vz =  " << vz)
        
        TFDEBUG("vx*vy = " << vx*vy)
        TFDEBUG("vx*vz = " << vx*vz)
        TFDEBUG("vy*vz = " << vy*vz)
        
        TFDEBUG("acos(vx*vy) = " << acos(vx*vy))
        TFDEBUG("acos(vx*vz) = " << acos(vx*vz))
        TFDEBUG("acos(vy*vz) = " << acos(vy*vz))
        
        //sxy = cos(0.5*(M_PI/2 - acos(vx*vy)));
        //sxz = cos(0.5*(M_PI/2 - acos(vx*vz)));
        //syz = cos(0.5*(M_PI/2 - acos(vy*vz)));
        
        sxy = cos(0.5*(acos(vx*vy) - M_PI/2) +  M_PI/2);
        sxz = cos(0.5*(acos(vx*vz) - M_PI/2) +  M_PI/2);
        syz = cos(0.5*(acos(vy*vz) - M_PI/2) +  M_PI/2);
        
        TFDEBUG("sxy = " << sxy)
        TFDEBUG("sxz = " << sxz)
        TFDEBUG("syz = " << syz)
        
        Vector4<double> nvx = vx - sxy*vy - sxz*vz;
        Vector4<double> nvy = vy - sxy*vx - syz*vz;
        Vector4<double> nvz = vz - sxz*vx - syz*vy;
        vx = nvx;
        vy = nvy;
        vz = nvz;
        
        vx.scaleToAbs(1.0);
        vy.scaleToAbs(1.0);
        vz.scaleToAbs(1.0);
        
        maxCos = fabs(sxy);
        if (maxCos > fabs(sxz)) {
            maxCos = fabs(sxz);
        }
        if (maxCos > fabs(syz)) {
            maxCos = fabs(syz);
        }
        
        TFDEBUG("maxCos =  " << maxCos)
        
        if (maxCos < epsilon) {
            break;
        }
        
        ++l;
    }
    
    TFDEBUG("absX =  " << absX)
    TFDEBUG("absY =  " << absY)
    TFDEBUG("absZ =  " << absZ)
    
    TFDEBUG("vx =  " << vx)
    TFDEBUG("vy =  " << vy)
    TFDEBUG("vz =  " << vz)
    TFDEBUG("tr =  " << tr)
    vx.scaleToAbs(absX);
    vy.scaleToAbs(absY);
    vz.scaleToAbs(absZ);
    tr.scale(1.0/count);
    
    TFDEBUG("vx =  " << vx)
    TFDEBUG("vy =  " << vy)
    TFDEBUG("vz =  " << vz)
    TFDEBUG("tr =  " << tr)
    
    for (int i = 0; i < 3; ++i) {
        r[i][0] = vx[i];
        r[i][1] = vy[i];
        r[i][2] = vz[i];        
        r[i][3] = tr[i];
    }

    r[3][0] = 0;
    r[3][1] = 0;
    r[3][2] = 0;
    r[3][3] = 1.0;

  //  this->matrix = r;
#endif
*/       
    
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
                        this->handleVector1(static_cast<Vector4Event *>(event));
                    } break;
                    case PortNR_Vector2: {
                        this->handleVector2(static_cast<Vector4Event *>(event));
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
            //cout << "Matrix4CalibrationDev: " << event->getEventTypeID() << " " << event << endl;
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


