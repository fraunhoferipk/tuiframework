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


#include "DeviceContainer.h"

#include "DeviceFactory.h"
#include "DeviceConfig.h"
#include "../core/IDevice.h"
#include "../core/Exception.h"
#include "../logging/Logger.h"

#include <iostream>

using namespace std;

namespace tuiframework {

DeviceContainer::DeviceContainer(IDeviceFactory & deviceFactory) :
    deviceFactory(deviceFactory) {
}


DeviceContainer::~DeviceContainer() {
}


void DeviceContainer::createDeviceInstances(const std::map<std::string, DeviceInstance> & deviceInstanceMap) {
    int entityID = 1;
    map<string, DeviceInstance>::const_iterator i = deviceInstanceMap.begin();
    map<string, DeviceInstance>::const_iterator e = deviceInstanceMap.end();
    while (i != e) {
        DeviceConfig * deviceConfig = new DeviceConfig((*i).second.getParameterGroup());
        deviceConfig->setEntityID(entityID);
        deviceConfig->setInstanceName((*i).second.getName());
        deviceConfig->setTypeName((*i).second.getDeviceTypeName());

        IDevice * device = deviceFactory.createInstance((*i).second.getDeviceTypeName(), deviceConfig);
        if (device) {
            this->deviceMap[entityID] = device;
            this->nameIDMap[(*i).second.getName()] = entityID;
        } else {
            TFERROR("Unknown device type: \"" << (*i).second.getDeviceTypeName() << "\"");
        }
        ++entityID;
        ++i;
    }
}


void DeviceContainer::freeDeviceInstances() {
    map<int, IDevice *>::iterator i = this->deviceMap.begin();
    map<int, IDevice *>::iterator e = this->deviceMap.end();
    while (i != e) {
        (*i).second->deviceStop();
        delete (*i).second;
        ++i;
    }
    this->deviceMap.clear();
}


void DeviceContainer::connectDevices(ITUIServer & tuiServer) {
    map<int, IDevice *>::iterator i = this->deviceMap.begin();
    map<int, IDevice *>::iterator e = this->deviceMap.end();
    while (i != e) {
        (*i).second->deviceConnect(tuiServer);
        ++i;
    }
}


void DeviceContainer::disconnectDevices() {
    map<int, IDevice *>::iterator i = this->deviceMap.begin();
    map<int, IDevice *>::iterator e = this->deviceMap.end();
    while (i != e) {
        (*i).second->deviceDisconnect();
        ++i;
    }
}


bool DeviceContainer::executeDevices() {
    map<int, IDevice *>::iterator i = this->deviceMap.begin();
    map<int, IDevice *>::iterator e = this->deviceMap.end();
    while (i != e) {
        (*i).second->deviceExecute();
        ++i;
    }
    return true;
}


vector<DeviceDescriptor> DeviceContainer::getDeviceDescriptorVector() const {
    vector<DeviceDescriptor> descriptor;
    map<int, IDevice *>::const_iterator i = this->deviceMap.begin();
    map<int, IDevice *>::const_iterator e = this->deviceMap.end();
    while (i != e) {
        descriptor.push_back((*i).second->getDeviceDescriptor());
        ++i;
    }
    return descriptor;
}


int DeviceContainer::getEntityIDByName(const std::string & name) const {
    map<string, int>::const_iterator iter = this->nameIDMap.find(name);
    if (iter == this->nameIDMap.end()) {
        throw Exception(TFSTR("Device \"" << name << "\" not found."));
    }
    return (*iter).second;
}


int DeviceContainer::getChannelIDByName(int entityID, const std::string & channelName) const {
    map<int, IDevice *>::const_iterator iter = this->deviceMap.find(entityID);
    if (iter == this->deviceMap.end()) {
        throw Exception(TFSTR("Device with ID \"" << entityID << "\" not found."));
    }

    const map<string, int> & nameChannelNrMap = (*iter).second->getDeviceDescriptor().getNameChannelNrMap();
    map<string, int>::const_iterator iter2 = nameChannelNrMap.find(channelName);
    if (iter2 == nameChannelNrMap.end()) {
        throw Exception(TFSTR("In Device: \"" << (*iter).second->getDeviceDescriptor().getInstanceName() <<
                "\" of type \"" << (*iter).second->getDeviceDescriptor().getDeviceType().getDeviceTypeName() <<
                "\" portName \"" << channelName << "\" not found."));
    }

    return (*iter2).second;
}

}

