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


#include "DTrackDev.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/core/EPAddress.h>
#include <tuiframework/core/Exception.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuitypes/common/CommonTypeReg.h>

#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

#include <stdio.h>
#include <iostream>
#include <sstream>

#ifdef _WIN32
	#include <winsock2.h>
	#include <windows.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
#endif
#include <ctime>
#include <assert.h>



using namespace tuiframework;
using namespace std;

namespace tuidevices {

IDevice * DTrackDev::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new DTrackDev(*deviceConfig);
}


std::string DTrackDev::getDeviceName() {
    return "DTrackDevice";
}


DTrackDev::DTrackDev(const DeviceConfig & deviceConfig) {

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["M4OUT1"] = Port("M4OUT1", "Matrix4Channel", Port::Source);
    portMap["M4OUT2"] = Port("M4OUT2", "Matrix4Channel", Port::Source);
    portMap["M4OUT3"] = Port("M4OUT3", "Matrix4Channel", Port::Source);
    portMap["M4OUT4"] = Port("M4OUT4", "Matrix4Channel", Port::Source);
    portMap["M4OUT5"] = Port("M4OUT5", "Matrix4Channel", Port::Source);
    portMap["M4OUT6"] = Port("M4OUT6", "Matrix4Channel", Port::Source);
    portMap["M4OUT7"] = Port("M4OUT7", "Matrix4Channel", Port::Source);
    portMap["M4OUT8"] = Port("M4OUT8", "Matrix4Channel", Port::Source);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName(getDeviceName());
    deviceType.setDescription("DTrackDev");
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

    this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);

    try {
        //this->ipAddress = deviceConfig.getParameterGroup("Server").getString("IPAddress");
        this->ipAddress = deviceConfig.getParameterGroup().getString("Server|IPAddress");
        TFDEBUG("IPAddress = " << this->ipAddress);
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
    }
}


DTrackDev::~DTrackDev() {
}


tuiframework::IEventSink * DTrackDev::getEventSink() const {
    return this->eventSink;
}


int DTrackDev::getEntityID() const {
    return this->entityID;
}

  // dtrack2 initialization
bool DTrackDev::deviceExecute() {
    if (this->ipAddress.size() == 0) {
        TFERROR("Missing DTrack server ip address.");
        return false;
    }

    int num_camera;
    ostringstream command;
    string response;
    istringstream responsestream;

    dt = new DTrack2(this->ipAddress);

    if(!dt->valid()){
        TFERROR("DTrack2 init error");
        return false;
    }

    TFINFO("connected to ARTtrack Controller '" << this->ipAddress << "' (with data port " << dt->get_data_port() << ")");

    	// get some information from ARTtrack Controller:
    if(!dt->get_parameter("system", "access", response)){  // command: get access rights
        command_error_to_console(dt);
        messages_to_console(dt);
        return false;
    }
	
    if(response.compare("full")){  // check if we have full access
        TFINFO("no 'full access' to ARTtrack Controller '" << this->ipAddress << "' response:'" << response << "'" << endl);
        return false;
    }

    if(!dt->send_command("init")){  // command: intialize ARTtrack Controller
        command_error_to_console(dt);
        messages_to_console(dt);
        return false;
    }

    num_camera = 0;

    if(!dt->get_parameter("camera", "number_of_cameras", response)){  // command: get number of cameras
        command_error_to_console(dt);
        messages_to_console(dt);
        return false;
    }

    responsestream.str(response);
    responsestream >> num_camera;  // number of cameras

    	// do some settings for ARTtrack Controller:
    command.str("");
    command << "udp my_ip " << dt->get_data_port();  // send tracking data to this application

    if(!dt->set_parameter("output", "net ch01", command.str())){  // command: request tracking data
        command_error_to_console(dt);
        messages_to_console(dt);
        return false;
    }

    	// start measurement:
    if(!dt->send_command("tracking start")){  // command: start measurement
        command_error_to_console(dt);
        messages_to_console(dt);
        return false;
    }

    int rc = pthread_create(&this->inputLoopThread, NULL, DTrackDev::inputLoopThread_run, this);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
    }

    return true;
  }


// Output data error:
// return value (o): last action was successfull
bool DTrackDev::data_error_to_console(DTrack2 * dt)
{

    // SDK errors:

    if(dt->data_timeout()){
        cout << "--- timeout while waiting for tracking data" << endl;
        return false;
    }

    if(dt->data_neterror()){
        cout << "--- error while receiving tracking data" << endl;
        return false;
    }
            
    if(dt->data_parseerror()){
        cout << "--- error while parsing tracking data" << endl;
        return false;
    }

    return true;
}


// Output ARTtrack Controller command error:
// return value (o): command was successfull
bool DTrackDev::command_error_to_console(DTrack2 * dt)
{
    int errcode;
    string errstring;

    	// SDK errors:
    if(dt->server_noconnection()) {
        cout << "--- no connection to ARTtrack Controller" << endl;
        return false;
    }

    if(dt->server_timeout()) {
        cout << "--- timeout while waiting for server answer" << endl;
        return false;
    }

    if(dt->server_neterror()) {
        cout << "--- error while waiting for server answer" << endl;
        return false;
    }

    if(dt->server_parseerror()) {
        cout << "--- error while parsing server answer" << endl;
        return false;
    }

    // ARTtrack Controller errors:
    if(!dt->get_lasterror(errcode)){
        return true;
    }

    dt->get_lasterror(errstring);
    cout << "ARTtrack Controller command error: " << errcode << " \"" << errstring << "\"" << endl;

    return false;
}


void * DTrackDev::inputLoopThread_run(void * arg) {
    printf("DTrackDev - input loop thread started\n");
    DTrackDev * dTrackDevice = static_cast<DTrackDev *>(arg);
    dTrackDevice->executeInputLoop();
    return 0;
}


void DTrackDev::executeInputLoop() {

    bool ok;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    select(0, 0, 0, 0, &tv); 
  
    this->inputLoopRunning = true;
    while (this->inputLoopRunning) {
/*
        tv.tv_sec = 0;
        tv.tv_usec = 1000;
        select(0, 0, 0, 0, &tv);
*/
        // receive tracking data:
        
        ok = dt->receive();

        static bool first = true;
        if (first) {
            cout << endl << "frame " << dt->get_framecounter() << " ts " << dt->get_timestamp()
            << " nbod " << dt->get_num_body() << " nfly " << dt->get_num_flystick()
            << " nmea " << dt->get_num_meatool() << " nhand " << dt->get_num_hand()
            << " nmar " << dt->get_num_marker() << endl;
            first = false;
        }
        
        if(!ok) {
            data_error_to_console(dt);
        } else if (this->eventSink) {
            //      data_error_to_console(dt);
            int i;
            dtrack2_body_type body;
            Matrix4Data mat;
            for (i=0; i < dt->get_num_body() && i < 8; i++) {
                body = dt->get_body(i);
                
                if (body.quality < 0) {
                    cout << "bod " << body.id << " not tracked" << endl;
                } else {
                    Matrix4ChangedEvent * event = new Matrix4ChangedEvent();
                    event->setAddress(EPAddress(this->entityID, i));
                    mat.setRow(0,body.rot[0], body.rot[1], body.rot[2], 0);
                    mat.setRow(1,body.rot[3], body.rot[4], body.rot[5], 0);
                    mat.setRow(2,body.rot[6], body.rot[7], body.rot[8], 0);
                    mat.setRow(3,body.loc[0], body.loc[1], body.loc[2], 1);
                    event->setPayload(mat);
                    eventSink->push(event);
                    std::cout << "mat send "<< std::endl;

                    cout << "bodyid: " << body.id << " " << mat << endl;
                }
            }
        }
    }
}


// Output ARTtrack Controller messages:
void DTrackDev::messages_to_console(DTrack2 * dt)
{

    while(dt->get_message()){
        cout << "ARTtrack Controller message: \"" << dt->get_message_status()
            << "\" \"" << dt->get_message_msg() << "\"" << endl;
    }
}


void DTrackDev::deviceStop() {

    this->inputLoopRunning = false;

  		// stop measurement:
  
    if(!dt->set_parameter("output", "net ch01", "none")){  // command: deactivate tracking data
        command_error_to_console(dt);
        messages_to_console(dt);
        return;
    }

    if(!dt->send_command("tracking stop")){  // command: stop measurement
        command_error_to_console(dt);
        messages_to_console(dt);
        return;
    }

	delete this->dt;
	cout << "DTrackDev::deviceStop()" << endl;
}


void DTrackDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    // tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void DTrackDev::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        // this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void DTrackDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & DTrackDev::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


}

