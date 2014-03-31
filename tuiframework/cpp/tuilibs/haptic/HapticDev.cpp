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



#include "HapticDev.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/core/EPAddress.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuitypes/common/CommonTypeReg.h>

#include <stdio.h>
#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>

#include <ctime>
#include <assert.h>

#include <cstdlib>
#include <cstring>

using namespace tuiframework;
using namespace std;

namespace tuidevices {

IDevice * HapticDev::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new HapticDev(*deviceConfig);
}


std::string HapticDev::getDeviceName() {
    return "HapticDevice";
}


  HapticDev::HapticDev(const DeviceConfig & deviceConfig)
  : udpReceiverSocket(inSerializedDataQueue) {
    
      this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
      this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["HOUT1"] = Port("HOUT1", "HapticChannel", Port::Source);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName(getDeviceName());
    deviceType.setDescription("HapticDevice");
    this->deviceDescriptor.setDeviceType(deviceType);

    std::map<std::string, int> nameChannelNrMap;
    int nr = 0;

    for (std::map<string, Port>::const_iterator i = portMap.begin(),
	   e = portMap.end(); i != e; ++nr, ++i)
      nameChannelNrMap[(*i).second.getName()] = nr;

    this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);
}


HapticDev::~HapticDev() {
}


tuiframework::IEventSink * HapticDev::getEventSink() const {
    return this->eventSink;
}


int HapticDev::getEntityID() const {
    return this->entityID;
}


bool HapticDev::deviceExecute() {
  this->udpReceiverSocket.setMyPort(7777);
  this->udpReceiverSocket.create();
  
  int rc = pthread_create(&this->inputLoopThread, NULL, HapticDev::inputLoopThread_run, this);
  if (rc) {
    std::cerr << "ERROR; return code from pthread_create() is "
	      << rc << std::endl;
    return false;
  }

  return true;
}

void * HapticDev::inputLoopThread_run(void * arg) {
    printf("HapticDev - input loop thread started\n");
    HapticDev * hapticDevice = static_cast<HapticDev *>(arg);
    hapticDevice->executeInputLoop();
    return 0;
}

void HapticDev::executeInputLoop() {

  const int portNr = this->deviceDescriptor.getNameChannelNrMap()["HOUT1"];
  HapticAngle hangle;

  this->inputLoopRunning = true;
  while (this->inputLoopRunning) {

    inSerializedDataQueue.waitForData();
    std::pair<char *, int> angleData = this->inSerializedDataQueue.pop();
    if (angleData.first != 0) { // still default element in first

      std::cout << "AngleData.first " << angleData.first << std::endl;

      HapticChangedEvent * event = new HapticChangedEvent();

      event->setAddress(EPAddress(this->entityID, portNr));
      stringstream ss;
      ss.str(angleData.first);
      string str;
      ss >> str;
      ss >> str;
      ss >> str;
      
      float a;
      ss >> a;
      cout << "Float: " << a << endl;
      
      hangle.setAngle(a);
      event->setPayload(hangle);

      std::cout << "HapticDev: " << event << std::endl;

      eventSink->push(event);

      delete angleData.first;
    }
  }
}

void HapticDev::deviceStop() {
  this->inputLoopRunning = false;
  this->udpReceiverSocket.cancel(); 
  this->udpReceiverSocket.join();
  pthread_cancel(this->inputLoopThread);
  pthread_join(this->inputLoopThread, 0);
}

void HapticDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    // tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}

void HapticDev::deviceDisconnect() {
  if (tuiServer) {
    this->tuiServer->tuiServerDeregisterDevice(this->entityID);
    // this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
    this->tuiServer = 0;
  }
}


  void HapticDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
  }


const DeviceDescriptor & HapticDev::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


}

