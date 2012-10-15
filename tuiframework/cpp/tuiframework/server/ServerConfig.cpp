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


#include "ServerConfig.h"

#include <iostream>

using namespace tuiframework;
using namespace std;

namespace tuiframework {

ServerConfig::ServerConfig() {
}


ServerConfig::~ServerConfig() {
}


ServerStartupConfig & ServerConfig::getServerStartupConfig() {
    return this->serverStartupConfig;
}


const ServerStartupConfig & ServerConfig::getServerStartupConfig() const {
    return this->serverStartupConfig;
}


void ServerConfig::setServerStartupConfig(const ServerStartupConfig & serverStartupConfig) {
    this->serverStartupConfig = serverStartupConfig;
}


std::map<std::string, DeviceType> & ServerConfig::getDeviceTypeMap() {
    return this->deviceTypeMap;
}


const std::map<std::string, DeviceType> & ServerConfig::getDeviceTypeMap() const {
    return this->deviceTypeMap;
}


void ServerConfig::setDeviceTypeMap(const std::map<std::string, tuiframework::DeviceType> & deviceTypeMap) {
    this->deviceTypeMap = deviceTypeMap;
}


std::map<std::string, DeviceInstance> & ServerConfig::getDeviceInstanceMap() {
    return this->deviceInstanceMap;
}


const std::map<std::string, DeviceInstance> & ServerConfig::getDeviceInstanceMap() const {
    return this->deviceInstanceMap;
}


void ServerConfig::setDeviceInstanceMap(const std::map<std::string, tuiframework::DeviceInstance> & deviceInstanceMap) {
    this->deviceInstanceMap = deviceInstanceMap;
}
    

std::map<std::string, TUIObjectType> & ServerConfig::getTUIObjectTypeMap() {
    return this->tuiObjectTypeMap;
}


const std::map<std::string, TUIObjectType> & ServerConfig::getTUIObjectTypeMap() const {
    return this->tuiObjectTypeMap;
}


void ServerConfig::setTUIObjectTypeMap(const std::map<std::string, tuiframework::TUIObjectType> & tuiObjectTypeMap) {
    this->tuiObjectTypeMap = tuiObjectTypeMap;
}


std::map<std::string, TUIObjectInstance> & ServerConfig::getTUIObjectInstanceMap() {
    return this->tuiObjectInstanceMap;
}


const std::map<std::string, TUIObjectInstance> & ServerConfig::getTUIObjectInstanceMap() const {
    return this->tuiObjectInstanceMap;
}


void ServerConfig::setTUIObjectInstanceMap(const std::map<std::string, TUIObjectInstance> & tuiObjectInstanceMap) {
    this->tuiObjectInstanceMap = tuiObjectInstanceMap;
}


std::map<std::string, MSPInstance> & ServerConfig::getMSPInstanceMap() {
    return this->mspInstanceMap;
}


const std::map<std::string, MSPInstance> & ServerConfig::getMSPInstanceMap() const {
    return this->mspInstanceMap;
}


void ServerConfig::setMSPInstanceMap(const std::map<std::string, MSPInstance> & mspInstanceMap) {
    this->mspInstanceMap = mspInstanceMap;
}


vector<Connector> & ServerConfig::getConnectorVector() {
    return this->connectorVector;
}


const std::vector<Connector> & ServerConfig::getConnectorVector() const {
    return this->connectorVector;
}


void ServerConfig::setConnectorVector(const std::vector<Connector> & connectorVector) {
    this->connectorVector = connectorVector;
}


std::ostream & ServerConfig::serialize(std::ostream & os) const {

    os << this->serverStartupConfig;
    os << " " << this->deviceTypeMap.size();
    {
        map<string, DeviceType>::const_iterator i = this->deviceTypeMap.begin();
        map<string, DeviceType>::const_iterator e = this->deviceTypeMap.end();
        while (i != e) {
            os << " " << (*i).second;
            i++;
        }
    }

    os << " " << this->deviceInstanceMap.size();
    {
        map<string, DeviceInstance>::const_iterator i = this->deviceInstanceMap.begin();
        map<string, DeviceInstance>::const_iterator e = this->deviceInstanceMap.end();
        while (i != e) {
            os << " " << (*i).second;
            i++;
        }
    }

    os << " " << this->tuiObjectTypeMap.size();
    {
        map<string, TUIObjectType>::const_iterator i = this->tuiObjectTypeMap.begin();
        map<string, TUIObjectType>::const_iterator e = this->tuiObjectTypeMap.end();
        while (i != e) {
            os << " " << (*i).second;
            i++;
        }
    }

    os << " " << this->tuiObjectInstanceMap.size();
    {
        map<string, TUIObjectInstance>::const_iterator i = this->tuiObjectInstanceMap.begin();
        map<string, TUIObjectInstance>::const_iterator e = this->tuiObjectInstanceMap.end();
        while (i != e) {
            os << " " << (*i).second;
            i++;
        }
    }
    return os;
}


std::istream & ServerConfig::deSerialize(std::istream & is) {
    this->deviceTypeMap.clear();
    this->deviceInstanceMap.clear();
    this->tuiObjectTypeMap.clear();
    this->tuiObjectInstanceMap.clear();

    is >> this->serverStartupConfig;

    {
        int count;
        is >> count;
        
        DeviceType deviceType;
        is >> deviceType;
        this->deviceTypeMap[deviceType.getDeviceTypeName()] = deviceType;
    }

    {
        int count;
        is >> count;
        
        DeviceInstance deviceInstance;
        is >> deviceInstance;
        this->deviceInstanceMap[deviceInstance.getName()] = deviceInstance;
    }


    {
        int count;
        is >> count;
        
        TUIObjectType tuiObjectType;
        is >> tuiObjectType;
        this->tuiObjectTypeMap[tuiObjectType.getName()] = tuiObjectType;
    }

    {
        int count;
        is >> count;
        
        TUIObjectInstance tuiObjectInstance;
        is >> tuiObjectInstance;
        this->tuiObjectInstanceMap[tuiObjectInstance.getName()] = tuiObjectInstance;
    }


    return is;
}

}

