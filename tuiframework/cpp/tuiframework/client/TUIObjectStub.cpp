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


#include "TUIObjectStub.h"

#include "../core/IEventChannel.h"
#include "../core/IEventChannelFactory.h"
#include "../core/IEventMsg.h"
#include "../core/TUIObjectInstance.h"
#include "../core/TUIObjectType.h"
#include "../client/TUIClientApp.h"

#include "../logging/Logger.h"

using namespace std;

namespace tuiframework {

TUIObjectStub::TUIObjectStub(const TUIObjectInstance & instance,
                             const TUIObjectType & type,
                             IEventSink & eventSink,
                             IEventChannelFactory & eventChannelFactory) :
    eventSink(eventSink),
    eventChannelFactory(eventChannelFactory) {

    this->id = instance.getID();
    this->name = instance.getName();
    //TODO TUIObjectStub::TUIObjectStub
    this->nameChannelNrMap = instance.getNameChannelNrMap();

    const map<string, Port> & portMap = type.getPortMap();

   // cout << ">>>>>>> " << dc.getInstanceName() << " " << portMap.size() << endl;
   // cout << deviceDescriptor << endl;

    map<string, Port>::const_iterator i = portMap.begin();
    map<string, Port>::const_iterator e = portMap.end();
    while (i != e) {
        map<string, int>::const_iterator iter = this->nameChannelNrMap.find((*i).second.getName());
        int portNr = (*iter).second;

        TFINFO("PortName: " << (*iter).first << " PortNumber: " << (*iter).second <<
                " DataFlowDirection: " << (*i).second.getDataFlowDirection());

     //   cout << ">>>>>>> " << portNr << endl;

        std::pair<int, int> arg;
        arg.first = this->id;
        arg.second = portNr;

        if ((*i).second.getDataFlowDirection() & Port::Source) {
            IEventChannel * channel = eventChannelFactory.createInstance((*i).second.getTypeName(), &arg);
            this->sourceChannelMap[portNr] = channel;
        }

        if ((*i).second.getDataFlowDirection() & Port::Sink) {
            IEventChannel * channel = eventChannelFactory.createInstance((*i).second.getTypeName(), &arg);
            channel->addEventSink(this);
            this->sinkChannelMap[portNr] = channel;
        }
        ++i;
    }
}


TUIObjectStub::~TUIObjectStub() {
}


int TUIObjectStub::getID() const {
    return this->id;
}


void TUIObjectStub::push(IEvent * event) {
    this->eventSink.push(event);
}


void TUIObjectStub::handleEvent(IEventMsg<EPAddress> * eventMsg) {
    map<int, IEventChannel *>::iterator iter = this->sourceChannelMap.find(eventMsg->getAddress().getPortNr());
    if (iter != this->sourceChannelMap.end()) {
        if ((*iter).second) {
            //cout << this->entityID << "  " << eventMsg << endl;
            (*iter).second->push(eventMsg);
        }
    }
}


IEventChannel * TUIObjectStub::getSourceChannel(int portNr) {
    //TFINFO("PortNr: " << portNr);
    map<int, IEventChannel *>::iterator iter = this->sourceChannelMap.find(portNr);
    if (iter != this->sourceChannelMap.end()) {
        return (*iter).second;
    }
    TFERROR("");
    return 0;
}


IEventChannel * TUIObjectStub::getSinkChannel(int portNr) {
    map<int, IEventChannel *>::iterator iter = this->sinkChannelMap.find(portNr);
    if (iter != this->sinkChannelMap.end()) {
        return (*iter).second;
    }
    TFERROR("");
    return 0;
}


IEventChannel * TUIObjectStub::getSourceChannel(const std::string & channelName) {
    map<string, int>::const_iterator iter = this->nameChannelNrMap.find(channelName);
    if (iter != this->nameChannelNrMap.end()) {
        return this->getSourceChannel((*iter).second);
    }
    TFERROR("");
    return 0;
}


IEventChannel * TUIObjectStub::getSinkChannel(const std::string & channelName) {
    map<string, int>::const_iterator iter = this->nameChannelNrMap.find(channelName);
    if (iter != this->nameChannelNrMap.end()) {
        return this->getSinkChannel((*iter).second);
    }
    TFERROR("ERROR");
    return 0;
}

}

