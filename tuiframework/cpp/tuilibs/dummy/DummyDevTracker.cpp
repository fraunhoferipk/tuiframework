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



#include "DummyDevTracker.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuitypes/common/CommonTypeReg.h>

#include <cstdlib> // GCC 4.3 related build problem

#include <ctime>

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace tuiframework;
using namespace std;

namespace tuidevices {


IDevice * DummyDevTracker::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new DummyDevTracker(*deviceConfig);
}


std::string DummyDevTracker::getDeviceName() {
    return "DummyDeviceTracker";
}



DummyDevTracker::DummyDevTracker(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["1"] = Port("1", "TrackerChannel", Port::Source, "6DOF");
    portMap["2"] = Port("2", "TrackerChannel", Port::Source, "6DOF");
    portMap["3"] = Port("3", "TrackerChannel", Port::Source, "6DOF");
    portMap["4"] = Port("4", "TrackerChannel", Port::Source, "6DOF");

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName("DummyDevTracker");
    deviceType.setDescription("Digital and Analog IO");

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


DummyDevTracker::~DummyDevTracker() {
}




bool DummyDevTracker::deviceExecute() {
    {
        int rc = pthread_create(&this->outputLoopThread, NULL, DummyDevTracker::outputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    {
        int rc = pthread_create(&this->inputLoopThread, NULL, DummyDevTracker::inputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    return true;
}


void DummyDevTracker::deviceStop() {
    pthread_cancel(this->inputLoopThread);
    pthread_join(this->inputLoopThread, 0);
    pthread_cancel(this->outputLoopThread);
    pthread_join(this->outputLoopThread, 0);
}


void DummyDevTracker::push(tuiframework::IEvent * event) {
    this->outputEventQueue.push(event);
}


void DummyDevTracker::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void DummyDevTracker::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void DummyDevTracker::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & DummyDevTracker::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * DummyDevTracker::inputLoopThread_run(void * arg) {
    printf("DummyDevTracker - input loop thread started\n");
    DummyDevTracker * dummyDevice = static_cast<DummyDevTracker *>(arg);
    dummyDevice->executeInputLoop();
    return 0;
}


void DummyDevTracker::executeInputLoop() {
#ifndef _WIN32
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 300000;
    select(0, 0, 0, 0, &tv); 
#endif
    int ac1 = 0;
    this->inputLoopRunning = true;
    while (this->inputLoopRunning) {
#ifndef _WIN32
        tv.tv_sec = 0;
        tv.tv_usec = 300000;
        select(0, 0, 0, 0, &tv); 
#endif
#ifdef _WIN32
		Sleep(300);
#endif
        if (this->eventSink) {
            {
                TrackerChangedEvent * event = new TrackerChangedEvent();
                event->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["1"]));
                TrackerData trackerData;
                trackerData.setPos(3.1, 4.1, 5.9);
                trackerData.setQuat(10.1, 11.2, 12.3, static_cast<float>(rand())/static_cast<float>(RAND_MAX));
                event->setPayload(trackerData);
                eventSink->push(event);
            }

            if (ac1 % 3 == 0) {
                TrackerChangedEvent * event = new TrackerChangedEvent();
                event->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["2"]));
                TrackerData trackerData;
                trackerData.setPos(2.9, 3.45, 4.39);
                trackerData.setQuat(11.1, 11.32, 19.13, static_cast<float>(rand())/static_cast<float>(RAND_MAX));
                event->setPayload(trackerData);
                eventSink->push(event);
            }

            if (ac1 % 5 == 0) {
                TrackerChangedEvent * event = new TrackerChangedEvent();
                event->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["3"]));
                TrackerData trackerData;
                trackerData.setPos(0.1, 0.1, 0.9);
                trackerData.setQuat(20.1, 1.2, 2.93, static_cast<float>(rand())/static_cast<float>(RAND_MAX));
                event->setPayload(trackerData);
                eventSink->push(event);
            }
        }
    }
}


void DummyDevTracker::executeOutputLoop() {
    this->outputLoopRunning = true;
    while (outputLoopRunning) {
        this->outputEventQueue.waitForData();
        IEvent * event = this->outputEventQueue.pop();
        if (event) {
            cout << "DummyDevTracker: " << event->getEventTypeID() << " " << event << endl;
            //delete event;
        }
    }
}


void * DummyDevTracker::outputLoopThread_run(void * arg) {
    printf("DummyDevTracker - output loop thread started\n");
    DummyDevTracker * dummyDevice = static_cast<DummyDevTracker *>(arg);
    dummyDevice->executeOutputLoop();
    return 0;
}

}

