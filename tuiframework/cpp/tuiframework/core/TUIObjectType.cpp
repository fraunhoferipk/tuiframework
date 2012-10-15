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


#include "TUIObjectType.h"

#include <iostream>

namespace tuiframework {

TUIObjectType::TUIObjectType() {
}


TUIObjectType::~TUIObjectType() {
}


void TUIObjectType::setName(const std::string & name) {
    this->name = name;
}


const std::string & TUIObjectType::getName() const {
    return this->name;
}


void TUIObjectType::setDescription(const std::string & description) {
    this->description = description;
}


const std::string & TUIObjectType::getDescription() {
    return this->description;
}


std::map<std::string, Port> & TUIObjectType::getPortMap() {
    return this->portMap;
}


const std::map<std::string , Port> & TUIObjectType::getPortMap() const {
    return this->portMap;
}


void TUIObjectType::setPortMap(const std::map<std::string, Port> & portMap) {
    this->portMap = portMap;
}


std::ostream & TUIObjectType::serialize(std::ostream & os) const {
    //os << this->tuiObjectTypeName << " " << this->description << " ";
    os << this->name << " ";
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


std::istream & TUIObjectType::deSerialize(std::istream & is) {
    //is >> this->tuiObjectTypeName >> this->description;
    is >> this->name;
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




