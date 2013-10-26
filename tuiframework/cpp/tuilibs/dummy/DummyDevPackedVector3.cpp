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



#include "DummyDevPackedVector3.h"

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


IDevice * DummyDevPackedVector3::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new DummyDevPackedVector3(*deviceConfig);
}


std::string DummyDevPackedVector3::getDeviceName() {
    return "DummyDevPackedVector3";
}



DummyDevPackedVector3::DummyDevPackedVector3(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["pv1"] = Port("pv1", "PackedVector3", Port::Source, "");

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName("DummyDevPackedVector3");
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


DummyDevPackedVector3::~DummyDevPackedVector3() {
}




bool DummyDevPackedVector3::deviceExecute() {
    {
        int rc = pthread_create(&this->outputLoopThread, NULL, DummyDevPackedVector3::outputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    {
        int rc = pthread_create(&this->inputLoopThread, NULL, DummyDevPackedVector3::inputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    return true;
}


void DummyDevPackedVector3::deviceStop() {
}


void DummyDevPackedVector3::push(tuiframework::IEvent * event) {
    this->outputEventQueue.push(event);
}


void DummyDevPackedVector3::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void DummyDevPackedVector3::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void DummyDevPackedVector3::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & DummyDevPackedVector3::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * DummyDevPackedVector3::inputLoopThread_run(void * arg) {
    printf("DummyDevPackedVector3 - input loop thread started\n");
    DummyDevPackedVector3 * dummyDevice = static_cast<DummyDevPackedVector3 *>(arg);
    dummyDevice->executeInputLoop();
    return 0;
}


void DummyDevPackedVector3::executeInputLoop() {
    this->inputLoopRunning = true;
    int a = 0;
    double c = 0;
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
                    PackedVector3Event * event = new PackedVector3Event();
                    event->setAddress(EPAddress(this->entityID, (*i).second));
                    PackedType<Vector3<double> > pv;
                    vector<pair<int, Vector3<double> > > & v = pv.getItems();
                    
                    for (int i2 = 0; i2 < 24; ++i2) {
                        v.push_back(pair<int, Vector3<double> >(101 + i2, Vector3<double>(101.0 + i2, a, c)));
                        c += 1.0;
                    }
                    
                    event->setPayload(pv);
                    eventSink->push(event);
                    
                    a += 1.0;
                    ++i;
                }
            }
        }
    }
}


void DummyDevPackedVector3::executeOutputLoop() {
    this->outputLoopRunning = true;
    while (outputLoopRunning) {
        this->outputEventQueue.waitForData();
        IEvent * event = this->outputEventQueue.pop();
        if (event) {
            cout << "DummyDevPackedVector3: " << event->getEventTypeID() << " " << event << endl;
            //delete event;
        }
    }
}


void * DummyDevPackedVector3::outputLoopThread_run(void * arg) {
    printf("DummyDevPackedVector3 - output loop thread started\n");
    DummyDevPackedVector3 * dummyDevice = static_cast<DummyDevPackedVector3 *>(arg);
    dummyDevice->executeOutputLoop();
    return 0;
}

}


