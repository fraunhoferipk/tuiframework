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


#include <tuiframework/server/ServerConfig.h>
#include <tuiframework/server/TUIServerApp.h>

#include <tuiframework/xml/ServerConfigXMLReader.h>
#include <tuiframework/core/TypeRegistration.h>
#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/logging/Logger.h>

//devices
#include <dummy/DummyDevDA.h>
#include <dummy/DummyDevTracker.h>
#include <dummy/DummyDevMatrix4.h>
#include <dummy/DummyDevMatrix4x4.h>
#include <kinect/KinectDev.h>
#include <dtrack/DTrackDev.h>

#include <m4transf/MSPMatrix4x4Transf.h>
#include <kinecttransf/MSPKinectARTTransformation.h>

#include <map>
#include <string>

#include <iostream>
#include <stdlib.h>

#include <cstdio> // GCC 4.3 related build problem

using namespace tuidevices;
using namespace tuiframework;
using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 4) {
        cout << "Usage tuiserver1 <sender port> <receiver port> <server config path>" << endl;
        return 1;
    }

    TUIServerApp serverApp(atoi(argv[1]), atoi(argv[2]), true);
    tuiframework::initTypeRegistration(serverApp.getEventFactory());
    CommonTypeReg::registerTypes(&serverApp.getEventFactory(), 0);

        // initialize device factory
    IDeviceFactory & deviceFactory = serverApp.getDeviceFactory();
    deviceFactory.registerCreateFunction(DummyDevDA::getDeviceName(), DummyDevDA::createFunction);
    deviceFactory.registerCreateFunction(DummyDevTracker::getDeviceName(), DummyDevTracker::createFunction);
    deviceFactory.registerCreateFunction(DummyDevMatrix4::getDeviceName(), DummyDevMatrix4::createFunction);
    deviceFactory.registerCreateFunction(DummyDevMatrix4x4::getDeviceName(), DummyDevMatrix4x4::createFunction);

    deviceFactory.registerCreateFunction(KinectDev::getDeviceName(), KinectDev::createFunction);
    deviceFactory.registerCreateFunction(DTrackDev::getDeviceName(), DTrackDev::createFunction);

    IMSPFactory & mspFactory = serverApp.getMSPFactory();
    mspFactory.registerCreateFunction(MSPMatrix4x4Transf::getMSPTypeName(), MSPMatrix4x4Transf::createFunction);
    mspFactory.registerCreateFunction(MSPKinectARTTransformation::getMSPTypeName(), MSPKinectARTTransformation::createFunction);

    ServerConfig serverConfig;
    ServerConfigXMLReader serverConfigXMLReader;
    if (serverConfigXMLReader.readServerConfig(argv[3])) {
        serverConfig = serverConfigXMLReader.getServerConfig();
    } else {
        TFERROR("Error reading configuration");
        return 0;
    }
    serverApp.init(serverConfig);
    serverApp.tuiServerExecute();

    getchar();

    return 0;
}

