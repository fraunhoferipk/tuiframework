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

#include "TrackerToMatrix4MSP.h"

#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inTag = "in";
static const char * outTag = "out";

using namespace std;

namespace tuiframework {

IMSP * TrackerToMatrix4MSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new TrackerToMatrix4MSP(*config);
}

static std::string typeName = "TrackerToMatrix4MSP";

std::string TrackerToMatrix4MSP::getMSPTypeName() {
    return typeName;
}


TrackerToMatrix4MSP::TrackerToMatrix4MSP(const MSPConfig & config) :
    config(config),
    out(0) {

    this->eventDelegate.setReceiver(this, &TrackerToMatrix4MSP::handleEvent);
}


TrackerToMatrix4MSP::~TrackerToMatrix4MSP() {
}


const std::string & TrackerToMatrix4MSP::getTypeName() const {
    return this->type.getTypeName();
}


IEventSink * TrackerToMatrix4MSP::getEventSink(const std::string & name) {
    if (name.compare(inTag) == 0) {
        return &eventDelegate;
    }
    return 0;
}


void TrackerToMatrix4MSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
     if (name.compare(outTag) == 0) {
         this->out = eventSink;
     }
}


const MSPType & TrackerToMatrix4MSP::getMSPType() const {
    return this->type;
}


void TrackerToMatrix4MSP::handleEvent(TrackerChangedEvent * e) {
    if (this->out) {
        const TrackerData & td = e->getPayload();
        
        double q[4];
        td.getQuat(q);
        
        double p[4];
        td.getPos(p);
       
        double n = 1.0/sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
        
        q[0] *= n;
        q[1] *= n;
        q[2] *= n;
        q[3] *= n;
        
        double qx = q[0];
        double qy = q[1];
        double qz = q[2];
        double qw = q[3];
        
        Matrix4Data m;
        m.setColumn(0, 1.0f - 2.0f*qy*qy - 2.0f*qz*qz, 2.0f*qx*qy - 2.0f*qz*qw, 2.0f*qx*qz + 2.0f*qy*qw, 0.0f);
        m.setColumn(1, 2.0f*qx*qy + 2.0f*qz*qw, 1.0f - 2.0f*qx*qx - 2.0f*qz*qz, 2.0f*qy*qz - 2.0f*qx*qw, 0.0f);
        m.setColumn(2, 2.0f*qx*qz - 2.0f*qy*qw, 2.0f*qy*qz + 2.0f*qx*qw, 1.0f - 2.0f*qx*qx - 2.0f*qy*qy, 0.0f);
        m.setColumn(3, p[0], p[1], p[2], p[3]);
        
        Matrix4ChangedEvent * e2 = new Matrix4ChangedEvent(-1, -1, m);
        this->out->push(e2);
    }
    delete e;
}

}
