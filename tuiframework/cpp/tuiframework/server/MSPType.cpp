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


#include "MSPType.h"

namespace tuiframework {

MSPType::MSPType() {
}


MSPType::~MSPType() {
}


const std::string & MSPType::getTypeName() const {
    return this->typeName;
}


void MSPType::setTypeName(const std::string & typeName) {
    this->typeName = typeName;
}


void MSPType::setDescription(const std::string & description) {
    this->description = description;
}


const std::string & MSPType::getDescription() const {
    return this->description;
}


std::map<std::string, Port> & MSPType::getPortMap() {
    return this->portMap;
}


const std::map<std::string , Port> & MSPType::getPortMap() const {
    return this->portMap;
}


void MSPType::setPortMap(const std::map<std::string, Port> & portMap) {
    this->portMap = portMap;
}

}
