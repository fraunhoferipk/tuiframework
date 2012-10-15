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


#include "AttachedObjects.h"

#if _debug_AttachedObjects_cpp_ && !USE_TFDEBUG
#define USE_TFDEBUG
#endif

#include "../logging/Logger.h"

#include <iostream>

using namespace std;

namespace tuiframework {

AttachedObjects::AttachedObjects() {
}


AttachedObjects::~AttachedObjects() {
}


void AttachedObjects::setTUIObjectTypeVector(const vector<TUIObjectType> & tuiObjectTypeVector) {
    this->tuiObjectTypeVector = tuiObjectTypeVector;
}


const vector<TUIObjectType> & AttachedObjects::getTUIObjectTypeVector() const {
    return this->tuiObjectTypeVector;
}


void AttachedObjects::setTUIObjectInstanceVector(const vector<TUIObjectInstance> & tuiObjectInstanceVector) {
    this->tuiObjectInstanceVector = tuiObjectInstanceVector;
}


const vector<TUIObjectInstance> & AttachedObjects::getTUIObjectInstanceVector() const {
    return this->tuiObjectInstanceVector;
}


ostream & AttachedObjects::serialize(ostream & os) const {

    {
        os << this->tuiObjectTypeVector.size() << " ";
        vector<TUIObjectType>::const_iterator i = this->tuiObjectTypeVector.begin();
        vector<TUIObjectType>::const_iterator e = this->tuiObjectTypeVector.end();
        while (i != e) {
            os << *i << " ";
            ++i;
        }
    }
    {
        os << this->tuiObjectInstanceVector.size() << " ";
        vector<TUIObjectInstance>::const_iterator i = this->tuiObjectInstanceVector.begin();
        vector<TUIObjectInstance>::const_iterator e = this->tuiObjectInstanceVector.end();
        while (i != e) {
            os << *i << " ";
            ++i;
        }
    }
    return os;
}


istream & AttachedObjects::deSerialize(istream & is) {
    int count;

    is >> count;
    for (int i = 0; i < count; ++i) {
        TUIObjectType tuiObjectType;
        is >> tuiObjectType;
        this->tuiObjectTypeVector.push_back(tuiObjectType);
    }
    is >> count;
    for (int i = 0; i < count; ++i) {
        TUIObjectInstance tuiObjectInstance;
        is >> tuiObjectInstance;
        this->tuiObjectInstanceVector.push_back(tuiObjectInstance);
    }

    return is;

    TFDEBUG(*this)
}

}

