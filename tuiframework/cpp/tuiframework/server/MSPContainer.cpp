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


#include "MSPContainer.h"

#include "MSPFactorySingleton.h"

#include "../core/IEventSink.h"
#include "../core/IMSP.h"
#include "../server/IMSPFactory.h"
#include "../server/MSPConfig.h"
#include "../logging/Logger.h"

#include <sstream>

#include <iostream>


using namespace std;

namespace tuiframework {

MSPContainer::MSPContainer(IMSPFactory & mspFactory) :
    mspFactory(mspFactory) {
}


MSPContainer::~MSPContainer() {
}


void MSPContainer::createInstances(const std::map<std::string, MSPInstance> & instanceMap) {
    map<string, MSPInstance>::const_iterator i = instanceMap.begin();
    map<string, MSPInstance>::const_iterator e = instanceMap.end();
    while (i != e) {
        MSPConfig config((*i).second.getParameterGroup());
        config.setInstanceName((*i).second.getName());
        config.setTypeName((*i).second.getTypeName());
        IMSP * msp = mspFactory.createInstance((*i).second.getTypeName(), &config);
        if (msp) {
            this->instanceMap[(*i).second.getName()] = msp;
            TFDEBUG("MSP Instance '" << (*i).second.getName() << "' of type '" << (*i).second.getTypeName() << "'" << "created");
        } else {
            TFDEBUG("ERROR creating MSP '" << (*i).second.getName() << "' of type '" << (*i).second.getTypeName() << "'");
        }
        ++i;
    }
}


void MSPContainer::freeInstances() {
    map<string, IMSP *>::iterator i = this->instanceMap.begin();
    map<string, IMSP *>::iterator e = this->instanceMap.end();
    while (i != e) {
        delete (*i).second;
        ++i;
    }
    this->instanceMap.clear();
}


IEventSink * MSPContainer::getEventSink(const std::string & instanceName, const std::string & connectorName) {
    map<string, IMSP *>::iterator iter = this->instanceMap.find(instanceName);
    if (iter == this->instanceMap.end()) {
        //TODO MSPContainer::getEventSink -> Throw exception
        TFERROR("");
    }

    return (*iter).second->getEventSink(connectorName);
}


void MSPContainer::registerEventSink(const std::string & instanceName, const std::string & connectorName, IEventSink * eventSink) {
    map<string, IMSP *>::iterator iter = this->instanceMap.find(instanceName);
    if (iter == this->instanceMap.end()) {
        //TODO MSPContainer::getEventSink -> Throw exception
        TFERROR("");
    }

    (*iter).second->registerEventSink(connectorName, eventSink);
}

}
