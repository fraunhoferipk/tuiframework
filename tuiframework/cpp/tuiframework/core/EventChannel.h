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


#ifndef _tuiframework_EventChannel_h_
#define _tuiframework_EventChannel_h_

#include "EPEventMsg.h"
#include "EPAddress.h"
#include "IEventChannel.h"

#include <iostream>

#include <list>

namespace tuiframework {

/**
 *  An event channel is a pathway that events take between an
 *  event publisher and one or more event consumer.
 *
 *  Furthermore it stores the last passed event payload of type T.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template <class T, int typeID>
class EventChannel : public IEventChannel {
public:
    EventChannel(int entityID, int portNr) :
        entityID(entityID), portNr(portNr) {
    }

    virtual ~EventChannel() {
    }

    virtual int getChannelTypeID() const {
        return EPEventMsg<T, typeID>::EventTypeID();
    }

    virtual int getPortNr() const {
        return this->portNr;
    }


    virtual void addEventSink(IEventSink * eventSink) {
        this->eventSinkList.push_back(eventSink);
    }


    virtual void removeEventSink(IEventSink * eventSink) {
        this->eventSinkList.remove(eventSink);
    }

        /// Receives a new IEvent object. However, a EPEventMsg object with
        /// equal type T is expected.
    virtual void push(IEvent * event) {
    	//std::cout << "Eventchannel: " << this->portNr << "  " << event << std::endl;
        EPEventMsg<T, typeID> * e = static_cast<EPEventMsg<T, typeID> *>(event);
        this->setValue(e->getPayload());
    }

        /// Returns the last passed value of type T.
    const T & getValue() const {
        return this->value;
    }

        /// Replaces the stored value with the delivered one and
        /// updates all IEventSink objects with the new value.
        /// For each stored IEventSink object a new EPEventMsg
        /// is generated and pushed in the IEventSink.
        /// All new EPEventMsg objects contain the update value, the entityID
        /// and the portNr of this EventChannel.
    void setValue(const T & value) {
        this->value = value;
        std::list<IEventSink *>::iterator i = eventSinkList.begin();
        std::list<IEventSink *>::iterator e = eventSinkList.end();
        while (i != e) {
        	// std::cout << "EventChannel" << std::endl;
            EPEventMsg<T, typeID> * event = new EPEventMsg<T, typeID>(this->entityID, this->portNr, this->value);
            //std::cout << channelEvent << std::endl;
            (*i)->push(event);
           // delete event;
            ++i;
        }
    }

protected:
        // ID of the device
    int entityID;
        // Channel number
    int portNr;
        // Value
    T value;
        // IEventSink container
    std::list<IEventSink *> eventSinkList;
};


}

#endif

