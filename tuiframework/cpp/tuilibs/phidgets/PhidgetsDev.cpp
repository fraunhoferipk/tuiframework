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



#include "PhidgetsDev.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/server/DeviceConfig.h>

#include <tuitypes/common/CommonTypeReg.h>

#include <stdio.h>
#include <iostream>

using namespace tuiframework;
using namespace std;

namespace tuidevices {

IDevice * __cdecl PhidgetsDev::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new PhidgetsDev(*deviceConfig);
}


std::string __cdecl PhidgetsDev::getDeviceName() {
    return "PhidgetInterfaceKit-8-8-8";
}

static int __stdcall attachHandler(CPhidgetHandle handle, void * arg) {
    int serialNo;
    const char *name;

    CPhidget_getDeviceName(handle, &name);
    CPhidget_getSerialNumber(handle, &serialNo);

    printf("%s %10d attached!\n", name, serialNo);

    return 0;
}


static int __stdcall detachHandler(CPhidgetHandle handle, void * arg) {
    int serialNo;
    const char *name;

    CPhidget_getDeviceName (handle, &name);
    CPhidget_getSerialNumber(handle, &serialNo);

    printf("%s %10d detached!\n", name, serialNo);

    return 0;
}


static int __stdcall errorHandler(CPhidgetHandle handle, void * arg, int errorCode, const char * unknown) {
    printf("Error handled. %d - %s", errorCode, unknown);
    return 0;
}


static int __stdcall inputChangeHandler(CPhidgetInterfaceKitHandle handle, void * arg, int index, int state) {
    printf("Digital Input: %d > state: %d\n", index, state);
    //CPhidgetInterfaceKit_setOutputstate(IFK, index, state);
    //Change the sensitivity trigger of the sensors

    PhidgetsDev * phidgetsDev = static_cast<PhidgetsDev *>(arg);
    IEventSink * eventSink = phidgetsDev->getEventSink();
    if (eventSink) {
    	
     	int offset = phidgetsDev->getDOutOffset();
        DigitalChangedEvent * event = new DigitalChangedEvent();
        event->setAddress(EPAddress(phidgetsDev->getEntityID(), offset + index));
        event->setPayload(state == 1);
 		cout << "PhidgetsDev event: " << event << endl;       
        eventSink->push(event);
    }

    return 0;
}


static int __stdcall outputChangeHandler(CPhidgetInterfaceKitHandle handle, void * arg, int index, int state) {
	PhidgetsDev * phidgetsDev = static_cast<PhidgetsDev *>(arg);
    printf("Digital Output: %d > state: %d\n", index, state);
    return 0;
}


static int __stdcall sensorChangeHandler(CPhidgetInterfaceKitHandle handle, void * arg, int index, int value) {
    printf("Sensor: %d > Value: %d\n", index, value);

    PhidgetsDev * phidgetsDev = static_cast<PhidgetsDev *>(arg);
	int offset = phidgetsDev->getAOutOffset();

    IEventSink * eventSink = phidgetsDev->getEventSink();
    if (eventSink) {
        AnalogChangedEvent * event = new AnalogChangedEvent();
        event->setAddress(EPAddress(phidgetsDev->getEntityID(), offset + index));
        event->setPayload(static_cast<float>(value));
        eventSink->push(event);
    }

    return 0;
}


PhidgetsDev::PhidgetsDev(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["AOUT1"] = Port("AOUT1", "AnalogChannel", Port::Source);
    portMap["AOUT2"] = Port("AOUT2", "AnalogChannel", Port::Source);
    portMap["AOUT3"] = Port("AOUT3", "AnalogChannel", Port::Source);
    portMap["AOUT4"] = Port("AOUT4", "AnalogChannel", Port::Source);
    portMap["AOUT5"] = Port("AOUT5", "AnalogChannel", Port::Source);
    portMap["AOUT6"] = Port("AOUT6", "AnalogChannel", Port::Source);
    portMap["AOUT7"] = Port("AOUT7", "AnalogChannel", Port::Source);
    portMap["AOUT8"] = Port("AOUT8", "AnalogChannel", Port::Source);

    portMap["DOUT1"] = Port("DOUT1", "DigitalChannel", Port::Source);
    portMap["DOUT2"] = Port("DOUT2", "DigitalChannel", Port::Source);
    portMap["DOUT3"] = Port("DOUT3", "DigitalChannel", Port::Source);
    portMap["DOUT4"] = Port("DOUT4", "DigitalChannel", Port::Source);
    portMap["DOUT5"] = Port("DOUT5", "DigitalChannel", Port::Source);
    portMap["DOUT6"] = Port("DOUT6", "DigitalChannel", Port::Source);
    portMap["DOUT7"] = Port("DOUT7", "DigitalChannel", Port::Source);
    portMap["DOUT8"] = Port("DOUT8", "DigitalChannel", Port::Source);

    portMap["DIN1"] = Port("DIN1", "DigitalChannel", Port::Sink);
    portMap["DIN2"] = Port("DIN2", "DigitalChannel", Port::Sink);
    portMap["DIN3"] = Port("DIN3", "DigitalChannel", Port::Sink);
    portMap["DIN4"] = Port("DIN4", "DigitalChannel", Port::Sink);
    portMap["DIN5"] = Port("DIN5", "DigitalChannel", Port::Sink);
    portMap["DIN6"] = Port("DIN6", "DigitalChannel", Port::Sink);
    portMap["DIN7"] = Port("DIN7", "DigitalChannel", Port::Sink);
    portMap["DIN8"] = Port("DIN8", "DigitalChannel", Port::Sink);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName(getDeviceName());
    deviceType.setDescription("PhidgetsDev Interface Kit 8-8-8");
    this->deviceDescriptor.setDeviceType(deviceType);

    std::map<std::string, int> nameChannelNrMap;
    int nr = 0;
    map<string, Port>::iterator i = portMap.begin();
    map<string, Port>::iterator e = portMap.end();
    while (i != e) {
        nameChannelNrMap[(*i).second.getName()] = nr;
        ++nr;
        ++i;
    }
    
    this->aoutOffset = nameChannelNrMap["AOUT1"];
	this->doutOffset = nameChannelNrMap["DOUT1"];
	this->dinOffset = nameChannelNrMap["DIN1"];	

    this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);
}




PhidgetsDev::~PhidgetsDev() {
}


int PhidgetsDev::getAOutOffset() const {
	return this->aoutOffset;
}


int PhidgetsDev::getDOutOffset() const {
	return this->doutOffset;
}


int PhidgetsDev::getDInOffset() const {
	return this->dinOffset;
}


tuiframework::IEventSink * PhidgetsDev::getEventSink() const {
    return this->eventSink;
}


int PhidgetsDev::getEntityID() const {
    return this->entityID;
}


bool PhidgetsDev::deviceExecute() {
    CPhidgetInterfaceKit_create(&this->ifKit);

    CPhidget_set_OnAttach_Handler(reinterpret_cast<CPhidgetHandle>(this->ifKit), attachHandler, this);
    CPhidget_set_OnDetach_Handler(reinterpret_cast<CPhidgetHandle>(this->ifKit), detachHandler, this);
    CPhidget_set_OnError_Handler(reinterpret_cast<CPhidgetHandle>(this->ifKit), errorHandler, this);

    CPhidgetInterfaceKit_set_OnInputChange_Handler(ifKit, inputChangeHandler, this);
    CPhidgetInterfaceKit_set_OnSensorChange_Handler(ifKit, sensorChangeHandler, this);
    CPhidgetInterfaceKit_set_OnOutputChange_Handler(ifKit, outputChangeHandler, this);

    CPhidget_open(reinterpret_cast<CPhidgetHandle>(this->ifKit), -1);

    printf("Waiting for interface kit to be attached....");
    int result;
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 10000)))
    {
        const char * err;
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
        return false;
    }

    return true;
}


void PhidgetsDev::deviceStop() {
    printf("Closing...\n");
    CPhidget_close(reinterpret_cast<CPhidgetHandle>(this->ifKit));
    CPhidget_delete(reinterpret_cast<CPhidgetHandle>(this->ifKit));
}


void PhidgetsDev::push(IEvent * event) {
    if (event->getEventTypeID() == DigitalChangedEvent::EventTypeID()) {
        printf("PhidgetsDev::push -  DigitalChangedEvent\n");
        DigitalChangedEvent * digitalChangedEvent = static_cast<DigitalChangedEvent *>(event);
        CPhidgetInterfaceKit_setOutputState(this->ifKit,
        	digitalChangedEvent->getAddress().getPortNr() - this->getDInOffset(),
        	digitalChangedEvent->getPayload() ? 1 : 0);
    }
}


void PhidgetsDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void PhidgetsDev::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void PhidgetsDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & PhidgetsDev::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}



void PhidgetsDev::displayProperties() {
    int serialNo, version, numInputs, numOutputs, numSensors, triggerVal, ratiometric, i;
    const char * ptr;

    CPhidget_getDeviceType(reinterpret_cast<CPhidgetHandle>(this->ifKit), &ptr);
    CPhidget_getSerialNumber(reinterpret_cast<CPhidgetHandle>(this->ifKit), &serialNo);
    CPhidget_getDeviceVersion(reinterpret_cast<CPhidgetHandle>(this->ifKit), &version);

    CPhidgetInterfaceKit_getInputCount(this->ifKit, &numInputs);
    CPhidgetInterfaceKit_getOutputCount(this->ifKit, &numOutputs);
    CPhidgetInterfaceKit_getSensorCount(this->ifKit, &numSensors);
    CPhidgetInterfaceKit_getRatiometric(this->ifKit, &ratiometric);

    printf("%s\n", ptr);
    printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
    printf("# Digital Inputs: %d\n# Digital Outputs: %d\n", numInputs, numOutputs);
    printf("# Sensors: %d\n", numSensors);
    printf("Ratiometric: %d\n", ratiometric);

    for(i = 0; i < numSensors; i++)
    {
        CPhidgetInterfaceKit_getSensorChangeTrigger (this->ifKit, i, &triggerVal);

        printf("Sensor#: %d > Sensitivity Trigger: %d\n", i, triggerVal);
    }
}



}

