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


#include "Vector4PackedDuplicateMSP.h"

#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/server/ParameterGroup.h>
#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inAPTag = "AP";
static const char * outAP1Tag = "AP1";
static const char * outAP2Tag = "AP2";

using namespace std;

namespace tuiframework {

IMSP * Vector4PackedDuplicateMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Vector4PackedDuplicateMSP(*config);
}

static std::string typeName = "Vector4PackedDuplicate";

const std::string & Vector4PackedDuplicateMSP::getMSPTypeName() {
    return typeName;
}


Vector4PackedDuplicateMSP::Vector4PackedDuplicateMSP(const MSPConfig & config) :
    config(config),
    outAP1(0),
    outAP2(0) {
    
    this->eventDelegateAP.setReceiver(this, &Vector4PackedDuplicateMSP::handleAP);
}


Vector4PackedDuplicateMSP::~Vector4PackedDuplicateMSP() {
}


const std::string & Vector4PackedDuplicateMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Vector4PackedDuplicateMSP::getEventSink(const std::string & name) {
    if (name.compare(inAPTag) == 0) {
        return &this->eventDelegateAP;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Vector4PackedDuplicateMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outAP1Tag) == 0) {
        this->outAP1 = eventSink;
    } else if (name.compare(outAP2Tag) == 0) {
        this->outAP2 = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Vector4PackedDuplicateMSP::getMSPType() const {
    return this->type;
}


void Vector4PackedDuplicateMSP::handleAP(PackedVector4Event * e) {    
    if (this->outAP1) {
        this->outAP1->push(new PackedVector4Event(-1, -1, e->getPayload()));
    }
    
    if (this->outAP2) {
        this->outAP2->push(new PackedVector4Event(-1, -1, e->getPayload()));
    }
    
    delete e;
}

}
