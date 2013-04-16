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

#include "DigitalToAnalogMSP.h"

#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inTag = "in";
static const char * outTag = "out";

using namespace std;

namespace tuiframework {

IMSP * DigitalToAnalogMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new DigitalToAnalogMSP(*config);
}

static std::string typeName = "DigitalToAnalogMSP";

std::string DigitalToAnalogMSP::getMSPTypeName() {
    return typeName;
}


DigitalToAnalogMSP::DigitalToAnalogMSP(const MSPConfig & config) :
    config(config),
    out(0) {

    this->eventDelegate.setReceiver(this, &DigitalToAnalogMSP::handleEvent);
    
    try {
        this->value0 = config.getParameterGroup().getDouble("Convert|Value_0");
        this->value1 = config.getParameterGroup().getDouble("Convert|Value_1");
        TFDEBUG("Value_0 = " << this->value0);
        TFDEBUG("Value_1 = " << this->value1);
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
    }
}


DigitalToAnalogMSP::~DigitalToAnalogMSP() {
}


const std::string & DigitalToAnalogMSP::getTypeName() const {
    return this->type.getTypeName();
}


IEventSink * DigitalToAnalogMSP::getEventSink(const std::string & name) {
    if (name.compare(inTag) == 0) {
        return &eventDelegate;
    }
    return 0;
}


void DigitalToAnalogMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
     if (name.compare(outTag) == 0) {
         this->out = eventSink;
     }
}


const MSPType & DigitalToAnalogMSP::getMSPType() const {
    return this->type;
}


void DigitalToAnalogMSP::handleEvent(DigitalChangedEvent * e) {
    if (this->out) {
        if (e->getPayload()) {
            this->out->push(new AnalogChangedEvent(-1, -1, this->value1));
        } else {
            this->out->push(new AnalogChangedEvent(-1, -1, this->value0));
        }
    }

    delete e;
}

}