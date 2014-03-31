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



#include "DummyDevPackedVector4.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuitypes/common/CommonTypeReg.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

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


IDevice * DummyDevPackedVector4::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new DummyDevPackedVector4(*deviceConfig);
}


std::string DummyDevPackedVector4::getDeviceName() {
    return "DummyDevPackedVector4";
}



DummyDevPackedVector4::DummyDevPackedVector4(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["pv1"] = Port("pv1", "PackedVector4", Port::Source, "");

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName("DummyDevPackedVector4");
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


DummyDevPackedVector4::~DummyDevPackedVector4() {
}




bool DummyDevPackedVector4::deviceExecute() {
    {
        int rc = pthread_create(&this->outputLoopThread, NULL, DummyDevPackedVector4::outputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    {
        int rc = pthread_create(&this->inputLoopThread, NULL, DummyDevPackedVector4::inputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    return true;
}


void DummyDevPackedVector4::deviceStop() {
    pthread_cancel(this->inputLoopThread);
    pthread_join(this->inputLoopThread, 0);
    pthread_cancel(this->outputLoopThread);
    pthread_join(this->outputLoopThread, 0);
}


void DummyDevPackedVector4::push(tuiframework::IEvent * event) {
    this->outputEventQueue.push(event);
}


void DummyDevPackedVector4::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void DummyDevPackedVector4::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void DummyDevPackedVector4::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & DummyDevPackedVector4::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * DummyDevPackedVector4::inputLoopThread_run(void * arg) {
    printf("DummyDevPackedVector4 - input loop thread started\n");
    DummyDevPackedVector4 * dummyDevice = static_cast<DummyDevPackedVector4 *>(arg);
    dummyDevice->executeInputLoop();
    return 0;
}

static double getRandomDouble(double range) {
    return 2.0*range*(static_cast<double>(rand())/static_cast<double>(RAND_MAX) - 0.5);
}


static Vector4<double> getRandomVector4(double range) {
    return Vector4<double>(getRandomDouble(range), getRandomDouble(range), getRandomDouble(range));
}


void DummyDevPackedVector4::executeInputLoop() {
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
                map<std::string, int>::iterator i = this->deviceDescriptor.getNameChannelNrMap().begin();
                map<std::string, int>::iterator e = this->deviceDescriptor.getNameChannelNrMap().end();
                while (i != e) {
                    PackedVector4Event * event = new PackedVector4Event();
                    event->setAddress(EPAddress(this->entityID, (*i).second));
                   
                    PackedType<Vector4<double> > pv;
                    vector<pair<int, Vector4<double> > > & v = pv.getItems();
                    for (int i2 = 0; i2 < 24; ++i2) {
                        v.push_back(pair<int, Vector4<double> >(101 + i2, getRandomVector4(1.0)));
                    }
                    event->setPayload(pv);
                    eventSink->push(event);
                    
                    ++i;
                }
            }
        }
    }
}


void DummyDevPackedVector4::executeOutputLoop() {
    this->outputLoopRunning = true;
    while (outputLoopRunning) {
        this->outputEventQueue.waitForData();
        IEvent * event = this->outputEventQueue.pop();
        if (event) {
            cout << "DummyDevPackedVector4: " << event->getEventTypeID() << " " << event << endl;
            //delete event;
        }
    }
}


void * DummyDevPackedVector4::outputLoopThread_run(void * arg) {
    printf("DummyDevPackedVector4 - output loop thread started\n");
    DummyDevPackedVector4 * dummyDevice = static_cast<DummyDevPackedVector4 *>(arg);
    dummyDevice->executeOutputLoop();
    return 0;
}

}


