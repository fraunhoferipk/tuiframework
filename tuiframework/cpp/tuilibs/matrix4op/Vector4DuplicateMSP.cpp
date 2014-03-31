/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    designed for creating applications with a tangible user interface (TUI).
    
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


#include "Vector4DuplicateMSP.h"

#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/server/ParameterGroup.h>
#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inATag = "A";
static const char * outA1Tag = "A1";
static const char * outA2Tag = "A2";

using namespace std;

namespace tuiframework {

IMSP * Vector4DuplicateMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Vector4DuplicateMSP(*config);
}

static std::string typeName = "Vector4Duplicate";

const std::string & Vector4DuplicateMSP::getMSPTypeName() {
    return typeName;
}


Vector4DuplicateMSP::Vector4DuplicateMSP(const MSPConfig & config) :
    config(config),
    outA1(0),
    outA2(0) {
    
    this->eventDelegateA.setReceiver(this, &Vector4DuplicateMSP::handleA);
}


Vector4DuplicateMSP::~Vector4DuplicateMSP() {
}


const std::string & Vector4DuplicateMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Vector4DuplicateMSP::getEventSink(const std::string & name) {
    if (name.compare(inATag) == 0) {
        return &this->eventDelegateA;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Vector4DuplicateMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outA1Tag) == 0) {
        this->outA1 = eventSink;
    } else if (name.compare(outA2Tag) == 0) {
        this->outA2 = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Vector4DuplicateMSP::getMSPType() const {
    return this->type;
}


void Vector4DuplicateMSP::handleA(Vector4Event * e) {    
    if (this->outA1) {
        this->outA1->push(new Vector4Event(-1, -1, e->getPayload()));
    }
    
    if (this->outA2) {
        this->outA2->push(new Vector4Event(-1, -1, e->getPayload()));
    }
    
    delete e;
}

}


