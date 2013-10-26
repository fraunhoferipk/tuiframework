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



#include "DummyDevMatrix4.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuitypes/common/CommonTypeReg.h>

#include <cstdlib> // GCC 4.3 related build problem

#include <ctime>

#include <iostream>
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

namespace tuidevices {


IDevice * DummyDevMatrix4::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new DummyDevMatrix4(*deviceConfig);
}


std::string DummyDevMatrix4::getDeviceName() {
    return "DummyDevMatrix4";
}



DummyDevMatrix4::DummyDevMatrix4(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["1"] = Port("1", "Matrix4Channel", Port::Source, "6DOF");
    portMap["2"] = Port("2", "Matrix4Channel", Port::Source, "6DOF");
    portMap["3"] = Port("3", "Matrix4Channel", Port::Source, "6DOF");
    portMap["4"] = Port("4", "Matrix4Channel", Port::Source, "6DOF");
    portMap["5"] = Port("5", "Matrix4Channel", Port::Source, "6DOF");
    portMap["6"] = Port("6", "Matrix4Channel", Port::Source, "6DOF");
    portMap["7"] = Port("7", "Matrix4Channel", Port::Source, "6DOF");
    portMap["8"] = Port("8", "Matrix4Channel", Port::Source, "6DOF");

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName("DummyDevMatrix4");
    deviceType.setDescription("");

    this->deviceDescriptor.setDeviceType(deviceType);

    std::map<std::string, int> nameChannelNrMap;
    int nr = 1;
    map<string, Port>::iterator i = portMap.begin();
    map<string, Port>::iterator e = portMap.end();
    while (i != e) {
        nameChannelNrMap[(*i).second.getName()] = nr;
        ++nr;
        ++i;
    }
    this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);
}


DummyDevMatrix4::~DummyDevMatrix4() {
}




bool DummyDevMatrix4::deviceExecute() {
    {
        int rc = pthread_create(&this->outputLoopThread, NULL, DummyDevMatrix4::outputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    {
        int rc = pthread_create(&this->inputLoopThread, NULL, DummyDevMatrix4::inputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    return true;
}


void DummyDevMatrix4::deviceStop() {
}


void DummyDevMatrix4::push(tuiframework::IEvent * event) {
    this->outputEventQueue.push(event);
}


void DummyDevMatrix4::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void DummyDevMatrix4::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void DummyDevMatrix4::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & DummyDevMatrix4::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * DummyDevMatrix4::inputLoopThread_run(void * arg) {
    printf("DummyDevMatrix4 - input loop thread started\n");
    DummyDevMatrix4 * dummyDevice = static_cast<DummyDevMatrix4 *>(arg);
    dummyDevice->executeInputLoop();
    return 0;
}


void DummyDevMatrix4::executeInputLoop() {
    this->inputLoopRunning = true;
    while (this->inputLoopRunning) {
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


void DummyDevMatrix4::executeOutputLoop() {
    this->outputLoopRunning = true;
    while (outputLoopRunning) {
        this->outputEventQueue.waitForData();
        IEvent * event = this->outputEventQueue.pop();
        if (event) {
            cout << "DummyDevMatrix4: " << event->getEventTypeID() << " " << event << endl;
            //delete event;
        }
    }
}


void * DummyDevMatrix4::outputLoopThread_run(void * arg) {
    printf("DummyDevMatrix4 - output loop thread started\n");
    DummyDevMatrix4 * dummyDevice = static_cast<DummyDevMatrix4 *>(arg);
    dummyDevice->executeOutputLoop();
    return 0;
}

}


