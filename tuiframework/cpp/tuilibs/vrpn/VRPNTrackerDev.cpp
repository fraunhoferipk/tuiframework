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

#include "VRPNTrackerDev.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/core/Exception.h>
#include <tuiframework/core/IEventSink.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuitypes/common/CommonTypeReg.h>


#include <vrpn_Tracker.h>

#include <cstdlib> // GCC 4.3 related build problem

#include <ctime>
#include <iostream>
#include <sstream>

using namespace tuiframework;
using namespace std;

namespace tuidevices {
  
static void VRPN_CALLBACK vrpnEventHandler(void * arg,  const vrpn_TRACKERCB t) {
    VRPNTrackerDev * device = static_cast<VRPNTrackerDev *>(arg);
    IEventSink * eventSink = device->getEventSink();
    if (eventSink) {
        TrackerData td;
        td.setPos(t.pos[0], t.pos[1], t.pos[2]);
        td.setQuat(t.quat[0], t.quat[1], t.quat[2], t.quat[3]);
        TrackerChangedEvent * event = new TrackerChangedEvent();
        event->setAddress(EPAddress(device->getEntityID(), t.sensor));
        event->setPayload(td);
        eventSink->push(event);
    }
}  
  

IDevice * VRPNTrackerDev::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new VRPNTrackerDev(*deviceConfig);
}


std::string VRPNTrackerDev::getDeviceName() {
    return "VRPNTrackerDevice";
}


VRPNTrackerDev::VRPNTrackerDev(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;
    portMap["T1"] = Port("T1", "TrackerChannel", Port::Source);
    portMap["T2"] = Port("T2", "TrackerChannel", Port::Source);
    portMap["T3"] = Port("T3", "TrackerChannel", Port::Source);
    portMap["T4"] = Port("T4", "TrackerChannel", Port::Source);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName(getDeviceName());
    deviceType.setDescription("");
    this->deviceDescriptor.setDeviceType(deviceType);

    std::map<std::string, int> nameChannelNrMap;
    nameChannelNrMap["T1"] = 0;
    nameChannelNrMap["T2"] = 1;
    nameChannelNrMap["T3"] = 2;
    nameChannelNrMap["T4"] = 3;

    this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);

    try {
        this->trackerName = deviceConfig.getParameterGroup().getString("vrpn|trackerName");
        TFDEBUG("trackerName = " << this->trackerName);
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
    }
    
    this->tracker = new vrpn_Tracker_Remote(this->trackerName.c_str());
}


VRPNTrackerDev::~VRPNTrackerDev() {
}


bool VRPNTrackerDev::deviceExecute() {
  
    this->tracker->register_change_handler(this, vrpnEventHandler);
    
    {
        int rc = pthread_create(&this->inputLoopThread, NULL, VRPNTrackerDev::inputLoopThread_run, this);
        if (rc) {
            TFERROR("ERROR; return code from pthread_create() is " << rc)
        }
    }
    return true;
}


void VRPNTrackerDev::deviceStop() {
    pthread_cancel(this->inputLoopThread);
    pthread_join(this->inputLoopThread, 0);
}


void VRPNTrackerDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void VRPNTrackerDev::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer = 0;
    }
}


void VRPNTrackerDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & VRPNTrackerDev::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


int VRPNTrackerDev::getEntityID() const {
    return this->entityID;
}


IEventSink * VRPNTrackerDev::getEventSink() {
  return this->eventSink;
}


void * VRPNTrackerDev::inputLoopThread_run(void * arg) {
    TFDEBUG("VRPNTrackerDev - input loop thread started\n");
    VRPNTrackerDev * trackerDev = static_cast<VRPNTrackerDev *>(arg);
    trackerDev->executeInputLoop();
    return 0;
}


void VRPNTrackerDev::executeInputLoop() {
    this->inputLoopRunning = true;
    while (this->inputLoopRunning) {
        this->tracker->mainloop();
    }
}


}

