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

#include "PortAddress.h"

#include <iostream>

namespace tuiframework {


PortAddress::PortAddress() {
}


PortAddress::PortAddress(OwnerType type,
        const std::string & name,
        const std::string & channelName) :
        type(type),
        name(name),
        portName(portName) {

}


PortAddress::~PortAddress() {
}


PortAddress::OwnerType PortAddress::getOwnerType() const {
    return static_cast<PortAddress::OwnerType>(this->type);
}


void PortAddress::setOwnerType(PortAddress::OwnerType type) {
    this->type = static_cast<int>(type);
}


const std::string & PortAddress::getName() const {
    return this->name;
}


void PortAddress::setName(const std::string & name) {
    this->name = name;
}

const std::string & PortAddress::getPortName() const {
    return this->portName;
}


void PortAddress::setPortName(const std::string & portName) {
    this->portName = portName;
}



std::ostream & PortAddress::serialize(std::ostream & os) const {
    os << this->type << " " << this->name << " " << this->portName;
    return os;
}


std::istream & PortAddress::deSerialize(std::istream & is) {
    is >> this->type >> this->name >> this->portName;
    return is;
}



}
