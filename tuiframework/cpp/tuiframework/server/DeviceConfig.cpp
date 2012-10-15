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


#include "DeviceConfig.h"

#include "../logging/Logger.h"

#include <iostream>

using namespace std;

namespace tuiframework {


DeviceConfig::DeviceConfig(const ParameterGroup & parameterGroup) :
    parameterGroup(parameterGroup) {
}


DeviceConfig::~DeviceConfig() {
}


int DeviceConfig::getEntityID() const {
    return this->entityID;
}


void DeviceConfig::setEntityID(int entityID) {
    this->entityID = entityID;
}


const std::string & DeviceConfig::getInstanceName() const {
    return this->instanceName;
}


void DeviceConfig::setInstanceName(const string & instanceName) {
    this->instanceName = instanceName;
}


void DeviceConfig::setTypeName(const string & typeName) {
    this->typeName = typeName;
}


const string & DeviceConfig::getTypeName() const {
    return this->typeName;
}


const ParameterGroup & DeviceConfig::getParameterGroup() const {
    return this->parameterGroup;
}


ParameterGroup & DeviceConfig::getParameterGroup() {
    return this->parameterGroup;
}


std::ostream & DeviceConfig::serialize(std::ostream & os) const {
    os << this->entityID << " " << this->instanceName;
    return os;
}


std::istream & DeviceConfig::deSerialize(std::istream & is) {
    is >> this->entityID >> this->instanceName;
    return is;
}


}



