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


#include "MSPInstance.h"

#include <iostream>

namespace tuiframework {

MSPInstance::MSPInstance() {
}


MSPInstance::~MSPInstance() {
}


void MSPInstance::setName(const std::string & name) {
    this->name = name;
}


const std::string & MSPInstance::getName() const {
    return this->name;
}


void MSPInstance::setTypeName(const std::string & typeName) {
    this->typeName = typeName;
}


const std::string & MSPInstance::getTypeName() const {
    return this->typeName;
}


void MSPInstance::setDescription(const std::string & description) {
    this->description = description;
}


const std::string & MSPInstance::getDescription() const {
    return this->description;
}


void MSPInstance::setParameterGroup(const ParameterGroup & parameterGroup) {
    this->parameterGroup = parameterGroup;
}


const ParameterGroup & MSPInstance::getParameterGroup() const {
    return this->parameterGroup;
}


std::ostream & MSPInstance::serialize(std::ostream & os) const {
    //os << this->name << " " << this->tuiTypeName << " " << this->description;
    os << this->name << " " << this->typeName;

    return os;
}


std::istream & MSPInstance::deSerialize(std::istream & is) {
    //is >> this->name >> this->tuiTypeName >> this->description;
    is >> this->name >> this->typeName;

    return is;
}


} // namespace tuiframework

