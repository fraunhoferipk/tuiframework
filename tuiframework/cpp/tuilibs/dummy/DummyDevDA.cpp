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



#include "DummyDevDA.h"

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


IDevice * DummyDevDA::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new DummyDevDA(*deviceConfig);
}


std::string DummyDevDA::getDeviceName() {
    return "DummyDeviceDA";
}


DummyDevDA::DummyDevDA(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();
    //cout << "===> DummyDevDA: " << this->entityID << endl;

    map<string, Port> portMap;

    portMap["DO1"] = Port("DO1", "DigitalChannel", Port::Source);
    portMap["DO2"] = Port("DO2", "DigitalChannel", Port::Source);
    portMap["DO3"] = Port("DO3", "DigitalChannel", Port::Source);
    portMap["DO4"] = Port("DO4", "DigitalChannel", Port::Source);

    portMap["DI1"] = Port("DI1", "DigitalChannel", Port::Sink);
    portMap["DI2"] = Port("DI2", "DigitalChannel", Port::Sink);
    portMap["DI3"] = Port("DI3", "DigitalChannel", Port::Sink);
    portMap["DI4"] = Port("DI4", "DigitalChannel", Port::Sink);

    portMap["DIO1"] = Port("DIO1", "DigitalChannel", Port::SourceAndSink);
    portMap["DIO2"] = Port("DIO2", "DigitalChannel", Port::SourceAndSink);
    portMap["DIO3"] = Port("DIO3", "DigitalChannel", Port::SourceAndSink);
    portMap["DIO4"] = Port("DIO4", "DigitalChannel", Port::SourceAndSink);

    portMap["AO1"] = Port("AO1", "AnalogChannel", Port::Source);
    portMap["AO2"] = Port("AO2", "AnalogChannel", Port::Source);
    portMap["AO3"] = Port("AO3", "AnalogChannel", Port::Source);
    portMap["AO4"] = Port("AO4", "AnalogChannel", Port::Source);

    portMap["AI1"] = Port("AI1", "AnalogChannel", Port::Sink);
    portMap["AI2"] = Port("AI2", "AnalogChannel", Port::Sink);
    portMap["AI3"] = Port("AI3", "AnalogChannel", Port::Sink);
    portMap["AI4"] = Port("AI4", "AnalogChannel", Port::Sink);

    portMap["AIO1"] = Port("AIO1", "AnalogChannel", Port::SourceAndSink);
    portMap["AIO2"] = Port("AIO2", "AnalogChannel", Port::SourceAndSink);
    portMap["AIO3"] = Port("AIO3", "AnalogChannel", Port::SourceAndSink);
    portMap["AIO4"] = Port("AIO4", "AnalogChannel", Port::SourceAndSink);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName(getDeviceName());
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


DummyDevDA::~DummyDevDA() {
}



bool DummyDevDA::deviceExecute() {
    {
        int rc = pthread_create(&this->outputLoopThread, NULL, DummyDevDA::outputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    {
        int rc = pthread_create(&this->inputLoopThread, NULL, DummyDevDA::inputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    return true;
}


void DummyDevDA::deviceStop() {
}


void DummyDevDA::push(tuiframework::IEvent * event) {
    this->outputEventQueue.push(event);
}


void DummyDevDA::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void DummyDevDA::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void DummyDevDA::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & DummyDevDA::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * DummyDevDA::inputLoopThread_run(void * arg) {
    printf("DummyDevDA - input loop thread started\n");
    DummyDevDA * dummyDevice = static_cast<DummyDevDA *>(arg);
    dummyDevice->executeInputLoop();
    return 0;
}


void DummyDevDA::executeInputLoop() {

#ifndef _WIN32
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    select(0, 0, 0, 0, &tv); 
#endif
    int ac1 = 0;

    this->inputLoopRunning = true;
    while (this->inputLoopRunning) {
#ifndef _WIN32
        tv.tv_sec = 0;
        tv.tv_usec = 3000000;
        select(0, 0, 0, 0, &tv); 
#endif
#ifdef _WIN32
        Sleep(1000);
#endif
        if (this->eventSink) {
            {
                AnalogChangedEvent * event = new AnalogChangedEvent();
                event->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["AO1"]));
                event->setPayload(static_cast<float>(rand())/static_cast<float>(RAND_MAX));
                eventSink->push(event);
            }

            if (ac1 % 4 == 0) {
                DigitalChangedEvent * event = new DigitalChangedEvent();
                event->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["DO1"]));
                event->setPayload(ac1 % 8 == 0);
                eventSink->push(event);
            }

            if (ac1 % 7 == 0) {
                DigitalChangedEvent * event = new DigitalChangedEvent();
                event->setAddress(EPAddress(this->entityID, this->deviceDescriptor.getNameChannelNrMap()["DO2"]));
                event->setPayload(ac1 % 14 == 0);
                eventSink->push(event);
            }            
            
            ac1++;
        }
    }
}


void DummyDevDA::executeOutputLoop() {
    this->outputLoopRunning = true;
    while (outputLoopRunning) {
        this->outputEventQueue.waitForData();
        IEvent * event = this->outputEventQueue.pop();
        if (event) {
            cout << "DummyDevDA: " << event->getEventTypeID() << " " << event << endl;
            //delete event;
        }
    }
}


void * DummyDevDA::outputLoopThread_run(void * arg) {
    printf("DummyDevDA - output loop thread started\n");
    DummyDevDA * dummyDevice = static_cast<DummyDevDA *>(arg);
    dummyDevice->executeOutputLoop();
    return 0;
}

}

