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



#include "SoundDevice.h"


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
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif


using namespace tuiframework;
using namespace std;

namespace tuidevices {


IDevice * SoundDevice::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new SoundDevice(*deviceConfig);
}


std::string SoundDevice::getDeviceName() {
    return "SoundDevice";
}


SoundDevice::SoundDevice(const DeviceConfig & deviceConfig) {

    isPlaying = false;

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["RougnessVector"] = Port("RoughnessVector", "VectorList3dChannel", Port::Sink);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName(getDeviceName());
    deviceType.setDescription("Incoming vectors from oversketching application");
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

	//read paths from xml configuration file
	try {
		/*map<string, ParameterGroup> paramGroupMap = deviceConfig.getParameterGroup().getParameterGroupMap();
		map<string, ParameterGroup>::iterator i = paramGroupMap.begin();
		map<string, ParameterGroup>::iterator e = paramGroupMap.end();
		while (i != e) {

			cout << "parameter group is " << (*i).first << endl;
			++i;
		}*/
		m_cfgFilePath = deviceConfig.getParameterGroup().getString("cfgfile|Path");
        m_cfgFileName = deviceConfig.getParameterGroup().getString("cfgfile|CfgFilename");
		m_gradationFileName = deviceConfig.getParameterGroup().getString("cfgfile|GradationFilename");
    }
    catch(Exception & e) {
		cout << e.getFormattedString() << endl << endl;
        TFERROR(e.getFormattedString());
    }

	static const std::string& cfgFileName = m_cfgFilePath + m_cfgFileName;
	static const std::string& gradationFileName = m_cfgFilePath + m_gradationFileName;
	cout << "configuration file path is " << cfgFileName << endl;
	cout << "gradation file path is " << gradationFileName << endl;

	this->mp_soundmgr = supercollider::soundDllCreateSoundManager(cfgFileName, gradationFileName);
}


SoundDevice::~SoundDevice() {

	supercollider::soundDllDeleteSoundManager(this->mp_soundmgr);
}



bool SoundDevice::deviceExecute() {
    {
        int rc = pthread_create(&this->outputLoopThread, NULL, SoundDevice::outputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }
    
    return true;
}


void SoundDevice::deviceStop() {
}


void SoundDevice::push(tuiframework::IEvent * event) {
        this->outputEventQueue.push(event);
}


void SoundDevice::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, *this);
    this->tuiServer = &tuiServer;
}


void SoundDevice::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void SoundDevice::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & SoundDevice::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void SoundDevice::executeOutputLoop() {    

	std::vector<int> maxgradations = std::vector<int>();
	supercollider::soundDllGetMaxGradations(mp_soundmgr, maxgradations);

    int maxGradationFriction = maxgradations[0]-1;
	int maxGradationSpeed = maxgradations[1]-1;
	int gradationFriction = 0;
	double speed = 0; //double maxSpeed = 10.0;
	double distance = 0;
	double time = 0;
	int gradationSpeed = 0;
	int timeToSleep = 0;

    this->outputLoopRunning = true;

	int nothingPoppedCtr = 0;
    
	std::vector<int> testGradations = std::vector<int>();
	testGradations.push_back(5); //friction number 6
	testGradations.push_back(2); //speed number 3
	supercollider::soundDllPlay(mp_soundmgr, 0, testGradations);

	IEvent * event_tmp = 0;
	IEvent * storedEvent = 0;
	int throwawayCtr = 0;
	int callsToSupercolliderCtr = 0;
    while (outputLoopRunning) {	
		
		throwawayCtr = 0;
		this->outputEventQueue.waitForData();
		event_tmp = this->outputEventQueue.pop();
		storedEvent = 0;
		//use only most recent data!

		while(event_tmp != 0){
			storedEvent = event_tmp;
			event_tmp = this->outputEventQueue.pop();
			throwawayCtr++;
		}

        if (storedEvent) {
            std::cout << std::endl << "===============================" << std::endl;
			std::cout << "EVENT TRIGGERED!!!!! " << std::endl;
			if(throwawayCtr > 1){
				std::cout << "Threw away " << throwawayCtr-1 << " data packets" << std::endl;
			}

            VectorList3dEvent * vle = static_cast<VectorList3dEvent*>(storedEvent);
            VectorList3d vlist3d_orig = vle->getPayload();

			std::vector<Vector3d> vlist_orig = vlist3d_orig.getVectorList(); 

			if(vlist_orig.size() > 2){

				std::vector<Vector3d>::const_iterator i = vlist_orig.begin();    
				std::vector<Vector3d>::const_iterator e = vlist_orig.end();
			
				Vector3d speedVector = Vector3d((*i).getX(), (*i).getY(), (*i).getZ());
				speed = speedVector.absoluteValue();

				//std::cout << "Speed vector " << std::endl;
				//std::cout << speedVector << std::endl;

				++i;

				Vector3d startpoint = Vector3d((*i).getX(), (*i).getY(), (*i).getZ());

				std::vector<Vector3d> frictionVector = std::vector<Vector3d>();

				std::vector<Vector3d>::const_iterator i2;
				while (i != e) {
					frictionVector.push_back(Vector3d((*i).getX(), (*i).getY(), (*i).getZ()));
					i2 = i;
					++i;
				}

				VectorList3d frictionVectorList = VectorList3d(frictionVector);
			
				Vector3d endpoint = Vector3d((*i2).getX(), (*i2).getY(), (*i2).getZ());
				endpoint.subtract(startpoint);
				distance = endpoint.absoluteValue();
				time = 10*(distance / speed);
            
				//cout << "Distance is" << distance << endl;

				if(frictionVectorList.size() > 2){
					frictionVectorList.computeDirections();
					int numRemoved = 1;
					if(frictionVectorList.size() - numRemoved > 1){
                
						double avgchg = frictionVectorList.getAverageAngleChange();
                        gradationFriction = floor(avgchg*maxGradationFriction);
						
                        cout << "average change is: " << avgchg << endl;
                        cout << "FRICTION GRADATION is: " << gradationFriction << endl;
						cout << "TIME OF MOVEMENT is: " << time << endl;
                        
						double times[13] = {3.0, 2.7, 2.4, 2.1, 1.8, 1.5, 1.2, 0.9, 0.6, 0.3, 0.2, 0.08};

						bool timeSentinel = false;
						for(int timectr = 0; timectr < 13; timectr++){
							if(time >= times[timectr]){
								gradationSpeed = timectr;
								timeToSleep = int(1000*times[timectr])+100;
								timeSentinel = true;
								break;
							}
						}

						if(timeSentinel){

							cout << "SPEED GRADATION is: " << gradationSpeed << endl;

							std::vector<int> gradations = std::vector<int>();
							gradations.push_back(gradationFriction);
							gradations.push_back(gradationSpeed); //in new application, speed also needs to be set

							if(gradationFriction > maxGradationFriction){
								cout << "FRICTION GRADATION OUT OF BOUNDS" << endl;
								break;
							}else if(gradationSpeed > maxGradationSpeed){
								cout << "FRICTION GRADATION OUT OF BOUNDS" << endl;
								break;
							}

							supercollider::soundDllPlay(mp_soundmgr, 0, gradations);
							callsToSupercolliderCtr++;
							cout << "called supercollider for the " << callsToSupercolliderCtr << "-th time" << endl;
							cout << "Sleeping for " << timeToSleep << "ms" << endl << endl;
							Sleep(timeToSleep);
						}

					}
                }
            }
        }
    }

	
}


void * SoundDevice::outputLoopThread_run(void * arg) {
    printf("SoundDevice - output loop thread started\n");
    SoundDevice * dummyDevice = static_cast<SoundDevice *>(arg);
    dummyDevice->executeOutputLoop();
    return 0;
}


}

