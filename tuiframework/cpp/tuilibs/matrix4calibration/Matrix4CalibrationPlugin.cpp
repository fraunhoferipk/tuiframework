/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    designed for creating applications with a tangible user interface (TUI).
    
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



#include "Matrix4CalibrationPlugin.h"
#include "Matrix4CalibrationDev.h"

#include <tuiframework/server/DeviceFactorySingleton.h>

#include <tuiframework/core/Version.h>
#include <tuiframework/core/IDevice.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuiframework/server/IDeviceFactory.h>

using namespace tuidevices;
using namespace tuiframework;

namespace tuiframework {

static std::vector<std::string> deviceTypeNameVector;

void soInitPlugin() {
    DeviceFactorySingleton::getInstance()->registerCreateFunction(Matrix4CalibrationDev::getDeviceName(), Matrix4CalibrationDev::createFunction);
    deviceTypeNameVector.push_back(Matrix4CalibrationDev::getDeviceName());
}


std::string soGetTFVersion() {
    return TUIFRAMEWORK_VERSION;
}


std::string soGetSOVersion() {
    return "1";
}


void soDeviceRegistration(IDeviceFactory * deviceFactory) {
    deviceFactory->registerCreateFunction(Matrix4CalibrationDev::getDeviceName(), Matrix4CalibrationDev::createFunction);
}


const std::vector<std::string> & soGetDeviceTypeNameVector() {
    return deviceTypeNameVector;
}


std::string soGetDeviceTypeVersion(const std::string & deviceTypeName) {
    return "1";
}


IDevice * soNewDeviceInstance(const std::string & deviceTypeName, DeviceConfig & deviceConfig) {
    return DeviceFactorySingleton::getInstance()->createInstance(deviceTypeName, &deviceConfig);
}


void soDeleteDeviceInstance(IDevice * device) {
    delete device;
}


}

