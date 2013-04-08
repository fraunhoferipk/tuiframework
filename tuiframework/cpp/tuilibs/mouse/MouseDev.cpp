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

#include "MouseDev.h"

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

IDevice * MouseDev::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new MouseDev(*deviceConfig);
}


std::string MouseDev::getDeviceName() {
    return "MouseDevice";
}


MouseDev::MouseDev(const DeviceConfig & deviceConfig) :
	receiver(queue) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["m1"] = Port("m1", "MouseChannel", Port::Source);
    portMap["m2"] = Port("m2", "MouseChannel", Port::Source);
    portMap["m3"] = Port("m3", "MouseChannel", Port::Source);
    portMap["m4"] = Port("m4", "MouseChannel", Port::Source);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName(getDeviceName());
    deviceType.setDescription("");
    this->deviceDescriptor.setDeviceType(deviceType);

    std::map<std::string, int> nameChannelNrMap;
	nameChannelNrMap["m1"] = 1;
	nameChannelNrMap["m2"] = 2;
	nameChannelNrMap["m3"] = 3;
	nameChannelNrMap["m4"] = 4;

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


MouseDev::~MouseDev() {
}


bool MouseDev::deviceExecute() {
    {
        int rc = pthread_create(&this->inputLoopThread, NULL, MouseDev::inputLoopThread_run, this);
        if (rc) {
            TFERROR("ERROR; return code from pthread_create() is " << rc)
        }
    }
	this->receiver.setHostEventPrefix(false);
	this->receiver.create();
    return true;
}


void MouseDev::deviceStop() {
	this->receiver.cancel();
	pthread_cancel(this->inputLoopThread);
}


void MouseDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void MouseDev::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer = 0;
    }
}


void MouseDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & MouseDev::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * MouseDev::inputLoopThread_run(void * arg) {
    TFDEBUG("MouseDev - input loop thread started\n");
    MouseDev * mouseDev = static_cast<MouseDev *>(arg);
    mouseDev->executeInputLoop();
    return 0;
}


void MouseDev::executeInputLoop() {
	stringstream ss;
	while(true) {
		queue.waitForData();
		pair <char*,int> serializedData = queue.pop();
		if (serializedData.first != 0) {
			ss.clear();
			ss.str(serializedData.first);
			delete serializedData.first;
			MouseData md;
			ss >> md;
				// 4 ports supported
			if (md.getId() > 0 && md.getId() <= 4) {
				MouseEvent * e = new MouseEvent();
				e->setAddress(EPAddress(this->entityID, md.getId()));
				e->setPayload(md);
				//TFDEBUG(e)
				eventSink->push(e);
			}
		}
	}
}


}


