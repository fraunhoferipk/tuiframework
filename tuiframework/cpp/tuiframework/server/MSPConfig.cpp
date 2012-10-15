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


#include "MSPConfig.h"

#include "../core/Exception.h"

#include <iostream>

using namespace std;

namespace tuiframework {


MSPConfig::MSPConfig(const ParameterGroup & parameterGroup) :
    parameterGroup(parameterGroup) {
}


MSPConfig::~MSPConfig() {
}

const std::string & MSPConfig::getInstanceName() const {
    return this->instanceName;
}


void MSPConfig::setInstanceName(const std::string & instanceName) {
    this->instanceName = instanceName;
}


void MSPConfig::setTypeName(const string & typeName) {
    this->typeName = typeName;
}


const string & MSPConfig::getTypeName() const {
    return this->typeName;
}


void MSPConfig::setParameterGroup(const ParameterGroup & parameterGroup) {
    this->parameterGroup = parameterGroup;
}


const ParameterGroup &  MSPConfig::getParameterGroup() const {
    return this->parameterGroup;
}


std::ostream & MSPConfig::serialize(std::ostream & os) const {

    os << " " << this->instanceName;
    return os;
}


std::istream & MSPConfig::deSerialize(std::istream & is) {
    is >> this->instanceName;
    return is;
}


}



