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


#include "DeviceType.h"

#include <iostream>

namespace tuiframework {

DeviceType::DeviceType() {
}


DeviceType::~DeviceType() {
}


const std::string & DeviceType::getDeviceTypeName() const {
    return this->deviceTypeName;
}


void DeviceType::setDeviceTypeName(const std::string & deviceTypeName) {
    this->deviceTypeName = deviceTypeName;
}


const std::string & DeviceType::getDescription() const {
    return this->description;
}


void DeviceType::setDescription(const std::string & description) {
    this->description = description;
}


const std::map<std::string, Port> & DeviceType::getPortMap() const {
    return this->portMap;
}


std::map<std::string, Port> & DeviceType::getPortMap() {
    return this->portMap;
}


void DeviceType::setPortMap(const std::map<std::string, Port> & portMap) {
    this->portMap = portMap;
}


std::ostream & DeviceType::serialize(std::ostream & os) const {
    //os << this->deviceTypeName << " " << this->description << " ";
    os << this->deviceTypeName << " ";
    os << static_cast<int>(this->portMap.size());

    std::map<std::string, Port>::const_iterator i = this->portMap.begin();
    std::map<std::string, Port>::const_iterator e = this->portMap.end();
    while (i != e) {
        os << " ";
        os << (*i).second;
        i++;
    }

    return os;
}


std::istream & DeviceType::deSerialize(std::istream & is) {
    //is >> this->deviceTypeName >> this->description;
    is >> this->deviceTypeName;
    int portCount;
    is >> portCount;

    this->portMap.clear();
    for (int i = 0; i < portCount; i++) {
        Port port;
        is >> port;
        this->portMap[port.getName()] = port;
    }

    return is;
}


}


