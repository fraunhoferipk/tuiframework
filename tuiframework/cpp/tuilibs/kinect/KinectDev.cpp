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



/**

@brief Kinect integration.

The Kinect is read in an own programm ( see Project KinectDev )
and connected to the TUIServer via network and sockets ( in this class ).

@author Erik Sniegula, Student Fu Berlin
@date 21.02.2012, 1.00 created (es)

*/

// There's another implementation ( in KinectDev.openni ), where the Kinect is read directly from TUIServer via OpenNI drivers.
// Uncomment the following block, if you want to use this implementation ( Windows only ).
/*
#ifdef _WIN32
#include "KinectDev.openni"
#else
*/

#include "KinectDev.h"


#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuitypes/common/CommonTypeReg.h>

#include <tuiframework/core/SerializedDataQueue.h>
#ifdef _WIN32
#include <tuiframework/sockets/UDPReceiverWinSock2.h>
#else
#include <tuiframework/sockets/UDPReceiverSocket.h>
#endif

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


IDevice * KinectDev::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new KinectDev(*deviceConfig);
}


std::string KinectDev::getDeviceName() {
    return "KinectDevice";
}


KinectDev::KinectDev(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["JOINT_1"] = Port("JOINT_1", "KinectChannel", Port::Source);
    portMap["JOINT_2"] = Port("JOINT_2", "KinectChannel", Port::Source);
    portMap["JOINT_3"] = Port("JOINT_3", "KinectChannel", Port::Source);
    portMap["JOINT_6"] = Port("JOINT_6", "KinectChannel", Port::Source);
    portMap["JOINT_7"] = Port("JOINT_7", "KinectChannel", Port::Source);
    portMap["JOINT_9"] = Port("JOINT_9", "KinectChannel", Port::Source);
    portMap["JOINT_12"] = Port("JOINT_12", "KinectChannel", Port::Source);
    portMap["JOINT_13"] = Port("JOINT_13", "KinectChannel", Port::Source);
    portMap["JOINT_15"] = Port("JOINT_15", "KinectChannel", Port::Source);
    portMap["JOINT_17"] = Port("JOINT_17", "KinectChannel", Port::Source);
    portMap["JOINT_18"] = Port("JOINT_18", "KinectChannel", Port::Source);
    portMap["JOINT_20"] = Port("JOINT_20", "KinectChannel", Port::Source);
    portMap["JOINT_21"] = Port("JOINT_21", "KinectChannel", Port::Source);
    portMap["JOINT_22"] = Port("JOINT_22", "KinectChannel", Port::Source);
    portMap["JOINT_24"] = Port("JOINT_24", "KinectChannel", Port::Source);
    portMap["DEVICE_GESTURE"] = Port("DEVICE_GESTURE", "GestureChannel", Port::Source);


    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName(getDeviceName());
    deviceType.setDescription("Kinect");
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


KinectDev::~KinectDev() {
}


bool KinectDev::deviceExecute() {
    {
        int rc = pthread_create(&this->outputLoopThread, NULL, KinectDev::outputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    {
        int rc = pthread_create(&this->inputLoopThread, NULL, KinectDev::inputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    {
        int rc = pthread_create(&this->outputGestureLoopThread, NULL, KinectDev::outputGestureLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    {
        int rc = pthread_create(&this->inputGestureLoopThread, NULL, KinectDev::inputGestureLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }


    return true;
}


void KinectDev::deviceStop() {
}


void KinectDev::push(tuiframework::IEvent * event) {
    this->outputEventQueue.push(event);
}


void KinectDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void KinectDev::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void KinectDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & KinectDev::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void * KinectDev::inputLoopThread_run(void * arg) {
    printf("KinectDev - input loop thread started\n");
    KinectDev * kinectDev = static_cast<KinectDev *>(arg);
    kinectDev->executeInputLoop();
    return 0;
}

void * KinectDev::inputGestureLoopThread_run(void * arg) {
    printf("KinectDev - input gesture loop thread started\n");
    KinectDev * kinectDev = static_cast<KinectDev *>(arg);
    kinectDev->executeGestureInputLoop();
    return 0;
}


void KinectDev::executeInputLoop() {

	KinectJoint jointData;
	SerializedDataQueue myQueue;
#ifdef _WIN32
    UDPReceiverWinSock2 myReceiver( myQueue );
#else
    UDPReceiverSocket myReceiver( myQueue );
#endif
	myReceiver.setMyPort( 50497 );
	myReceiver.create();


	while( true ) {

		// wait until you receive data
		myQueue.waitForData();

        //std::cout << "data received in any kind" << std::endl;

		pair <char*,int> res = myQueue.pop();
		string str = res.first;

		// cut IP address
		str = str.substr(str.length()-res.second,res.second);

		// convert stream to KinectJoint
		stringstream ss;
		ss << str;
		ss >> jointData;

		// console output for testing purposes
		std::cout << "jointData: " << jointData << std::endl;

		// send data to client
		if ( this->eventSink ) {

			// determine port name
			stringstream s;
			s << jointData.getJointId();
			string portName = "JOINT_" + s.str();

			// pack up as event
			KinectEvent * event = new KinectEvent();
			event->setAddress( EPAddress( this->entityID, this->deviceDescriptor.getNameChannelNrMap()[portName] ) );
			event->setPayload( jointData );

			// send event
			eventSink->push( event );
		}
	}
}

void KinectDev::executeGestureInputLoop() {

	Gesture gestureData;
	SerializedDataQueue myQueue;
#ifdef _WIN32
    UDPReceiverWinSock2 myReceiver( myQueue );
#else
    UDPReceiverSocket myReceiver( myQueue );
#endif
	myReceiver.setMyPort( 48234 );
	myReceiver.create();


	while( true ) {

		// wait until you receive data
		myQueue.waitForData();

        //std::cout << "data received in any kind" << std::endl;

		pair <char*,int> res = myQueue.pop();
		string str = res.first;

		// cut IP address
		str = str.substr(str.length()-res.second,res.second);

		// convert stream to Gesture
		stringstream ss;
		ss << str;
		ss >> gestureData;

		// console output for testing purposes
		//std::cout << "GestureData: " << gestureData << std::endl;

		// send data to client
		if ( this->eventSink ) {

			// set port name
			string portName = "DEVICE_GESTURE";

			// pack up as event
			GestureEvent * event = new GestureEvent();
			event->setAddress( EPAddress( this->entityID, this->deviceDescriptor.getNameChannelNrMap()[portName] ) );
			event->setPayload( gestureData );

			// send event
			eventSink->push( event );
		}

	}

}


void KinectDev::executeOutputLoop() {
    this->outputLoopRunning = true;
    while (outputLoopRunning) {
        this->outputEventQueue.waitForData();
        IEvent * event = this->outputEventQueue.pop();
        if (event) {
            cout << "KinectDev: " << event->getEventTypeID() << " " << event << endl;
            delete event;
        }
    }
}

void KinectDev::executeGestureOutputLoop() {
    this->outputGestureLoopRunning = true;
    while (outputGestureLoopRunning) {
        this->outputEventQueue.waitForData();
        IEvent * event = this->outputEventQueue.pop();
        if (event) {
            cout << "KinectDev gesture: " << event->getEventTypeID() << " " << event << endl;
            delete event;
        }
    }
}


void * KinectDev::outputLoopThread_run(void * arg) {
    printf("KinectDev - output loop thread started\n");
    KinectDev * kinect = static_cast<KinectDev *>(arg);
    kinect->executeOutputLoop();
    return 0;
}

void * KinectDev::outputGestureLoopThread_run(void * arg) {
    printf("KinectDev - output gesture loop thread started\n");
    KinectDev * kinect = static_cast<KinectDev *>(arg);
    kinect->executeGestureOutputLoop();
    return 0;
}

}

//#endif
