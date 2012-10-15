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

#include "MSPKinectARTTransformation.h"

using namespace std;

namespace tuiframework {


IMSP * MSPKinectARTTransformation::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new MSPKinectARTTransformation(*config);
}

static std::string typeName = "MSPKinectARTTransformation";

std::string MSPKinectARTTransformation::getMSPTypeName() {
    return typeName;
}

MSPKinectARTTransformation::MSPKinectARTTransformation(const MSPConfig & config) :
    config(config) {

    this->eventSinkMap["ARTPOSITION_IN"] = &this->transfUnit;

    this->eventSinkMap["SKEL_HEAD"] = &this->transfUnit;
    this->eventSinkMap["SKEL_NECK"] = &this->transfUnit;
    this->eventSinkMap["SKEL_TORSO"] = &this->transfUnit;
    this->eventSinkMap["SKEL_LEFT_SHOULDER"] = &this->transfUnit;
    this->eventSinkMap["SKEL_LEFT_ELBOW"] = &this->transfUnit;
    this->eventSinkMap["SKEL_LEFT_HAND"] = &this->transfUnit;
    this->eventSinkMap["SKEL_RIGHT_SHOULDER"] = &this->transfUnit;
    this->eventSinkMap["SKEL_RIGHT_ELBOW"] = &this->transfUnit;
    this->eventSinkMap["SKEL_RIGHT_HAND"] = &this->transfUnit;
    this->eventSinkMap["SKEL_LEFT_HIP"] = &this->transfUnit;
    this->eventSinkMap["SKEL_LEFT_KNEE"] = &this->transfUnit;
    this->eventSinkMap["SKEL_LEFT_FOOT"] = &this->transfUnit;
    this->eventSinkMap["SKEL_RIGHT_HIP"] = &this->transfUnit;
    this->eventSinkMap["SKEL_RIGHT_KNEE"] = &this->transfUnit;
    this->eventSinkMap["SKEL_RIGHT_FOOT"] = &this->transfUnit;
    this->eventSinkMap["TUI_GESTURE"] = &this->transfUnit;

    this->transfUnit.setEventSinkMap(&(this->registeredEventSinkMap));

    this->registeredEventSinkMap["TransfRight_OUT"] = 0;
    this->registeredEventSinkMap["TransfLeft_OUT"] = 0;
    this->registeredEventSinkMap["Draw_OUT"] = 0;

    this->registeredEventSinkMap["TransfTong_OUT"] = 0;
    this->registeredEventSinkMap["Grab_OUT"] = 0;
    this->registeredEventSinkMap["TransfStroke_OUT"] = 0;
    this->registeredEventSinkMap["DrawStroke_OUT"] = 0;
    this->registeredEventSinkMap["Menu_OUT"] = 0;

}


MSPKinectARTTransformation::~MSPKinectARTTransformation() {
}


const std::string & MSPKinectARTTransformation::getTypeName() const {
    return typeName;
}


IEventSink * MSPKinectARTTransformation::getEventSink(const std::string & name) {
    map<string, IEventSink *>::iterator iter = this->eventSinkMap.find(name);
    if (iter != this->eventSinkMap.end()) {
        return (*iter).second;
    }
    TFERROR("");
    return 0;
}


void MSPKinectARTTransformation::registerEventSink(const std::string & name, IEventSink * eventSink) {
    map<string, IEventSink *>::iterator iter = this->registeredEventSinkMap.find(name);
    if (iter != this->registeredEventSinkMap.end()) {
        (*iter).second = eventSink;
    } else {
        TFERROR("");
        // TODO exception
    }

}



const MSPType & MSPKinectARTTransformation::getMSPType() const {
    return this->type;
}


}
