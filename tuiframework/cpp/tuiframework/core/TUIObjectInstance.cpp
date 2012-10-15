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


#include "TUIObjectInstance.h"

#include <iostream>

namespace tuiframework {

TUIObjectInstance::TUIObjectInstance() {
}


TUIObjectInstance::~TUIObjectInstance() {
}


void TUIObjectInstance::setName(const std::string & name) {
    this->name = name;
}


const std::string & TUIObjectInstance::getName() const {
    return this->name;
}


void TUIObjectInstance::setID(int id) {
    this->id = id;
}


int TUIObjectInstance::getID() const {
    return this->id;
}


void TUIObjectInstance::setTypeName(const std::string & typeName) {
    this->typeName = typeName;
}


const std::string & TUIObjectInstance::getTypeName() const {
    return this->typeName;
}


void TUIObjectInstance::setDescription(const std::string & description) {
    this->description = description;
}


const std::string & TUIObjectInstance::getDescription() {
    return this->description;
}


const std::map<std::string, int> & TUIObjectInstance::getNameChannelNrMap() const {
    return this->nameChannelNrMap;
}


std::map<std::string, int> & TUIObjectInstance::getNameChannelNrMap() {
    return this->nameChannelNrMap;
}


void TUIObjectInstance::setNameChannelNrMap(const std::map<std::string, int> & nameChannelNrMap) {
    this->nameChannelNrMap = nameChannelNrMap;
}



std::ostream & TUIObjectInstance::serialize(std::ostream & os) const {
    //os << this->name << " " << this->tuiTypeName << " " << this->description;
    os << this->name << " " << this->typeName << " " << this->id;

    os << " " << static_cast<int>(this->nameChannelNrMap.size());
    {
        std::map<std::string, int>::const_iterator i = this->nameChannelNrMap.begin();
        std::map<std::string, int>::const_iterator e = this->nameChannelNrMap.end();
        while (i != e) {
            os << " " << (*i).first << " " << (*i).second;
            i++;
        }
    }

    return os;
}


std::istream & TUIObjectInstance::deSerialize(std::istream & is) {
    //is >> this->name >> this->tuiTypeName >> this->description;
    is >> this->name >> this->typeName >> this->id;

    int nameChannelNrCount;
    is >> nameChannelNrCount;

    this->nameChannelNrMap.clear();
    for (int i = 0; i < nameChannelNrCount; i++) {
        std::string name;
        int portNr;
        is >> name;
        is >> portNr;
        this->nameChannelNrMap[name] = portNr;
    }

    return is;
}


} // namespace tuiframework

