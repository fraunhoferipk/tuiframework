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


#include "Matrix4PackedDuplicateMSP.h"

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

IMSP * Matrix4PackedDuplicateMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Matrix4PackedDuplicateMSP(*config);
}

static std::string typeName = "Matrix4PackedDuplicate";

const std::string & Matrix4PackedDuplicateMSP::getMSPTypeName() {
    return typeName;
}


Matrix4PackedDuplicateMSP::Matrix4PackedDuplicateMSP(const MSPConfig & config) :
    config(config),
    outAP1(0),
    outAP2(0) {
    
    this->eventDelegateAP.setReceiver(this, &Matrix4PackedDuplicateMSP::handleAP);
}


Matrix4PackedDuplicateMSP::~Matrix4PackedDuplicateMSP() {
}


const std::string & Matrix4PackedDuplicateMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Matrix4PackedDuplicateMSP::getEventSink(const std::string & name) {
    if (name.compare(inAPTag) == 0) {
        return &this->eventDelegateAP;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Matrix4PackedDuplicateMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outAP1Tag) == 0) {
        this->outAP1 = eventSink;
    } else if (name.compare(outAP2Tag) == 0) {
        this->outAP2 = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Matrix4PackedDuplicateMSP::getMSPType() const {
    return this->type;
}


void Matrix4PackedDuplicateMSP::handleAP(PackedMatrix4Event * e) {    
    if (this->outAP1) {
        this->outAP1->push(new PackedMatrix4Event(-1, -1, e->getPayload()));
    }
    
    if (this->outAP2) {
        this->outAP2->push(new PackedMatrix4Event(-1, -1, e->getPayload()));
    }
    
    delete e;
}

}
