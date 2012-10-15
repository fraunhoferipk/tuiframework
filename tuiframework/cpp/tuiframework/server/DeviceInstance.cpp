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


#include "DeviceInstance.h"

#include <iostream>

namespace tuiframework {


DeviceInstance::DeviceInstance() {
}


DeviceInstance::~DeviceInstance() {
}


const std::string & DeviceInstance::getName() const {
    return this->name;
}


void DeviceInstance::setName(const std::string & name) {
    this->name = name;
}


const std::string & DeviceInstance::getDeviceTypeName() const {
    return this->deviceTypeName;
}


void DeviceInstance::setDeviceTypeName(const std::string & deviceTypeName) {
    this->deviceTypeName = deviceTypeName;
}


void DeviceInstance::setParameterGroup(const ParameterGroup & parameterGroup) {
    this->parameterGroup = parameterGroup;
}


const ParameterGroup & DeviceInstance::getParameterGroup() const {
    return this->parameterGroup;
}


std::ostream & DeviceInstance::serialize(std::ostream & os) const {
    os << this->name << " " << this->deviceTypeName;
    return os;
}


std::istream & DeviceInstance::deSerialize(std::istream & is) {
    is >> this->name >> this->deviceTypeName;
    return is;
}


}



