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

#include "MotionCaptureStubDev.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuitypes/common/CommonTypeReg.h>

#include <cstdlib> // GCC 4.3 related build problem

#include <ctime>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace tuiframework;
using namespace std;

namespace tuidevices {

IDevice * MotionCaptureStubDev::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new MotionCaptureStubDev(*deviceConfig);
}


std::string MotionCaptureStubDev::getDeviceName() {
    return "MotionCaptureStubDev";
}


MotionCaptureStubDev::MotionCaptureStubDev(const DeviceConfig & deviceConfig) :
    receiver(queue) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["pv"] = Port("pv", "PackedVector4", Port::Source);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName(getDeviceName());
    deviceType.setDescription("");
    this->deviceDescriptor.setDeviceType(deviceType);

    std::map<std::string, int> nameChannelNrMap;
    nameChannelNrMap["pv"] = 1;

    this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);

    try {
        this->portNr = deviceConfig.getParameterGroup().getInt("Client|PortNr");
        TFDEBUG("PortNr = " << this->portNr);
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
    }

    this->receiver.setMyPort(this->portNr);
}


MotionCaptureStubDev::~MotionCaptureStubDev() {
}


bool MotionCaptureStubDev::deviceExecute() {
    {
        int rc = pthread_create(&this->inputLoopThread, NULL, MotionCaptureStubDev::inputLoopThread_run, this);
        if (rc) {
            TFERROR("ERROR; return code from pthread_create() is " << rc)
        }
    }
    this->receiver.setHostEventPrefix(false);
    this->receiver.create();
    return true;
}


void MotionCaptureStubDev::deviceStop() {
    this->receiver.cancel();
    this->receiver.join();
    pthread_cancel(this->inputLoopThread);
    pthread_join(this->inputLoopThread, 0);
}


void MotionCaptureStubDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void MotionCaptureStubDev::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer = 0;
    }
}


void MotionCaptureStubDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & MotionCaptureStubDev::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * MotionCaptureStubDev::inputLoopThread_run(void * arg) {
    TFDEBUG("MotionCaptureStubDev - input loop thread started\n");
    MotionCaptureStubDev * mouseDev = static_cast<MotionCaptureStubDev *>(arg);
    mouseDev->executeInputLoop();
    return 0;
}


void MotionCaptureStubDev::executeInputLoop() {
    stringstream ss;
    while(true) {
        queue.waitForData();
        pair <char*,int> serializedData = queue.pop();
        if (serializedData.first != 0) {
            ss.clear();
            ss.str(serializedData.first);
            delete serializedData.first;
            PackedType<Vector4<double> > pv;
            ss >> pv;
            // one user supported
            PackedVector4Event * e = new PackedVector4Event(this->entityID, 1, pv);
            eventSink->push(e);
        }
    }
}


}


