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


#include "TUIObjectStubContainer.h"

#include "TUIObjectStub.h"
#include "../core/IEventChannel.h"
#include "../core/IEventChannelFactory.h"

#include "../core/TypeRegistration.h"
#include "../logging/Logger.h"

#include <sstream>

using namespace std;

namespace tuiframework {

TUIObjectStubContainer::TUIObjectStubContainer(IEventSink & eventSink, IEventChannelFactory & eventChannelFactory) :
    eventSink(eventSink),
    eventChannelFactory(eventChannelFactory) {
}


TUIObjectStubContainer::~TUIObjectStubContainer() {
}


void TUIObjectStubContainer::handleEvent(IEventMsg<EPAddress> * eventMsg) {
    //TFINFO("EventMsg: " << eventMsg);
    TUIObjectStub * stub = this->stubMap[eventMsg->getAddress().getEntityID()];
    if (stub == 0) {
        TFERROR("TUIObjectStub with ID = " << eventMsg->getAddress().getEntityID() << " not found");
    } else {
        stub->handleEvent(eventMsg);
    }
}



void TUIObjectStubContainer::createStubs(const std::vector<TUIObjectInstance> & instanceVector,
        const std::vector<TUIObjectType> & typeVector) {

    map<string, TUIObjectType> typeMap;
    {
        std::vector<TUIObjectType>::const_iterator i = typeVector.begin();
        std::vector<TUIObjectType>::const_iterator e = typeVector.end();
        while (i != e) {
            typeMap[(*i).getName()] = *i;

            TFINFO("Added TUIObjectType: '" << (*i).getName() << "'");
            ++i;
        }
    }

    {
        std::vector<TUIObjectInstance>::const_iterator i = instanceVector.begin();
        std::vector<TUIObjectInstance>::const_iterator e = instanceVector.end();
        while (i != e) {
            this->nameIDMap[(*i).getName()] = (*i).getID();

            stringstream ss;
            TFINFO("Added TUIObjectInstance '" << (*i).getName() << "' of Type '" << (*i).getTypeName() << "' with ID = " << (*i).getID());
            map<string, TUIObjectType>::const_iterator iter = typeMap.find((*i).getTypeName());
            TUIObjectStub * stub = new TUIObjectStub(*i, (*iter).second, this->eventSink, this->eventChannelFactory);
            this->stubMap[(*i).getID()] = stub;

            ++i;
        }
    }
}


TUIObjectStub * TUIObjectStubContainer::getStub(int id) {
    map<int, TUIObjectStub *>::iterator iter = this->stubMap.find(id);
    if (iter != this->stubMap.end()) {
        return (*iter).second;
    }
    TFERROR("");
    return 0;
}


TUIObjectStub * TUIObjectStubContainer::getStub(const std::string & name) {
    map<string, int>::const_iterator iter = this->nameIDMap.find(name);
    if (iter == this->nameIDMap.end()) {
        TFERROR("");
        return 0;
    }
    return this->getStub((*iter).second);
}


IEventChannel * TUIObjectStubContainer::getSourceChannel(const std::string & stubName, const std::string & channelName) {
    TUIObjectStub * stub = this->getStub(stubName);
    if (!stub) {
        TFERROR("");
        return 0;
    }
    return stub->getSourceChannel(channelName);
}


IEventChannel * TUIObjectStubContainer::getSinkChannel(const std::string & stubName, const std::string & channelName) {
    TUIObjectStub * stub = this->getStub(stubName);
    if (!stub) {
        TFERROR("");
        return 0;
    }
    return stub->getSinkChannel(channelName);
}

}

