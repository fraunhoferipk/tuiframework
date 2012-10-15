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


#include "TUIObjectDescriptor.h"

#include <iostream>


namespace tuiframework {

TUIObjectDescriptor::TUIObjectDescriptor() {
}


TUIObjectDescriptor::~TUIObjectDescriptor() {
}


const TUIObjectInstance & TUIObjectDescriptor::getTUIObjectInstance() const {
    return this->tuiObjectInstance;
}


void TUIObjectDescriptor::setTUIObjectInstance(const TUIObjectInstance & tuiObjectInstance) {
    this->tuiObjectInstance = tuiObjectInstance;
}


const TUIObjectType & TUIObjectDescriptor::getTUIObjectType() const {
    return this->tuiObjectType;
}


void TUIObjectDescriptor::setTUIObjectType(const TUIObjectType & tuiObjectType) {
    this->tuiObjectType = tuiObjectType;
}


const std::map<std::string, int> & TUIObjectDescriptor::getNameChannelNrMap() const {
    return this->nameChannelNrMap;
}


std::map<std::string, int> & TUIObjectDescriptor::getNameChannelNrMap() {
    return this->nameChannelNrMap;
}


void TUIObjectDescriptor::setNameChannelNrMap(const std::map<std::string, int> & nameChannelNrMap) {
    this->nameChannelNrMap = nameChannelNrMap;
}


std::ostream & TUIObjectDescriptor::serialize(std::ostream & os) const {
    os << this->tuiObjectInstance << " " << this->tuiObjectType << " ";

    os << static_cast<int>(this->nameChannelNrMap.size());

    std::map<std::string, int>::const_iterator i = this->nameChannelNrMap.begin();
    std::map<std::string, int>::const_iterator e = this->nameChannelNrMap.end();
    while (i != e) {
        os << " " << (*i).first << " " << (*i).second;
        i++;
    }

    return os;
}


std::istream & TUIObjectDescriptor::deSerialize(std::istream & is) {
    is >> this->tuiObjectInstance >> this->tuiObjectType;

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

