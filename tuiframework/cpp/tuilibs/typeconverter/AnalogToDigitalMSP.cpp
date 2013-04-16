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
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the TUIFramework. If not, see <http://www.gnu.org/licenses/>.
*/

#include "AnalogToDigitalMSP.h"

#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inTag = "in";
static const char * outTag = "out";

using namespace std;

namespace tuiframework {

IMSP * AnalogToDigitalMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new AnalogToDigitalMSP(*config);
}

static std::string typeName = "AnalogToDigitalMSP";

std::string AnalogToDigitalMSP::getMSPTypeName() {
    return typeName;
}


AnalogToDigitalMSP::AnalogToDigitalMSP(const MSPConfig & config) :
    config(config),
    out(0) {

    this->eventDelegate.setReceiver(this, &AnalogToDigitalMSP::handleEvent);
    
    try {
        this->threshold = static_cast<float>(config.getParameterGroup().getDouble("Convert|Threshold"));
        TFDEBUG("Threshold = " << this->threshold);
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
    }
}


AnalogToDigitalMSP::~AnalogToDigitalMSP() {
}


const std::string & AnalogToDigitalMSP::getTypeName() const {
    return this->type.getTypeName();
}


IEventSink * AnalogToDigitalMSP::getEventSink(const std::string & name) {
    if (name.compare(inTag) == 0) {
        return &eventDelegate;
    }
    return 0;
}


void AnalogToDigitalMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
     if (name.compare(outTag) == 0) {
         this->out = eventSink;
     }
}


const MSPType & AnalogToDigitalMSP::getMSPType() const {
    return this->type;
}


void AnalogToDigitalMSP::handleEvent(AnalogChangedEvent * e) {
    if (this->out) {
        this->out->push(new DigitalChangedEvent(-1, -1, e->getPayload() >= this->threshold));
    }

    delete e;
}

}