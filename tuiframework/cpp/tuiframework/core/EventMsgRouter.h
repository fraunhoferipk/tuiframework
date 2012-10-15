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


#ifndef _tuiframework_EventMsgRouter_h_
#define _tuiframework_EventMsgRouter_h_

#include "IEventSink.h"
#include "IEventMsg.h"
#include <map>
#include <functional>

namespace tuiframework {

/**
 *  Forwards event messages as a function of their address.
 *  The forwarding decision is determined by the routing table.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template<class A, class T, class Compare = std::less<A> >
class EventMsgRouter : public IEventSink {
public:
    EventMsgRouter() :
        dst(0) {
    }

    virtual ~EventMsgRouter() {
    }

        /// Forwards an event message as a function of its address
        /// to a destination which is determined by the routing table.
    virtual void push(IEvent * event) {
        IEventMsg<A> * e = static_cast<IEventMsg<A> *>(event);
        typename std::map<A, T *, Compare>::iterator iter;
        iter = this->routingTable.find(e->getAddress());
        if (iter != this->routingTable.end()) {
            (*iter).second->push(event);
        } else if (this->dst) {
            this->dst->push(event);
        } else {
            delete event;
        }
    }

        /// Adds an entry to the routing table.
    virtual void addRoutingTableEntry(const A & address, T * dst) {
        routingTable[address] = dst;
    }


    virtual void setDefaultDst(T * dst) {
        this->dst = dst;
    }


protected:

        /// A mapping table, which consists of an address as key,
        /// a destination as value and a compare object which provides
        /// a compare operator
    std::map<A, T *, Compare> routingTable;
        /// default destination
    T * dst;
};

}

#endif

