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



#include <tuiframework/logging/Logger.h>
#include <tuiframework/core/Exception.h>

#include "MSPMatrix4x4Transf.h"
#include "Matrix4x4TransfUnit.h"

using namespace std;

namespace tuiframework {

static const char * inputChannelTag = "IN";
static const char * outputChannelTag = "OUT";


IMSP * MSPMatrix4x4Transf::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new MSPMatrix4x4Transf(*config);
}

static std::string typeName = "Matrix4x4Transf";

std::string MSPMatrix4x4Transf::getMSPTypeName() {
    return typeName;
}

MSPMatrix4x4Transf::MSPMatrix4x4Transf(const MSPConfig & config) :
    config(config) {

    try {
        this->transfUnit.setParameterGroup(this->config.getParameterGroup().getParameterGroup("transf"));
    }
    catch (Exception & e) {
        e.addErrorMessage("in MSPMatrix4x4Transf.", __FILE__, __LINE__);
        TFERROR(e.getFormattedString());
    }

    this->eventSinkMap[inputChannelTag] = &this->transfUnit;
    this->registeredEventSinkMap[outputChannelTag] = 0;
}


MSPMatrix4x4Transf::~MSPMatrix4x4Transf() {
}


const std::string & MSPMatrix4x4Transf::getTypeName() const {
    return typeName;
}


IEventSink * MSPMatrix4x4Transf::getEventSink(const std::string & name) {
    map<string, IEventSink *>::iterator iter = this->eventSinkMap.find(name);
    if (iter != this->eventSinkMap.end()) {
        return (*iter).second;
    }
    TFERROR("");
    return 0;
}


void MSPMatrix4x4Transf::registerEventSink(const std::string & name, IEventSink * eventSink) {
    this->transfUnit.setEventSink(eventSink);
    map<string, IEventSink *>::iterator iter = this->registeredEventSinkMap.find(name);
    if (iter != this->registeredEventSinkMap.end()) {
        (*iter).second = eventSink;
    } else {
        TFERROR("");
        // TODO exception
    }
}



const MSPType & MSPMatrix4x4Transf::getMSPType() const {
    return this->type;
}


}
