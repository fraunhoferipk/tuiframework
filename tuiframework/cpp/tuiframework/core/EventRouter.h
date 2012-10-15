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


#ifndef _tuiframework_EventRouter_h_
#define _tuiframework_EventRouter_h_

#include "IEventSink.h"
#include "IEvent.h"
#include <map>

namespace tuiframework {

/**
 *  Forwards event messages as a function of their ID.
 *  The forwarding decision is determined by the routing table.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template <class T, class V>
class EventRouter : public IEventSink {
public:
    EventRouter() {
    }

    virtual ~EventRouter() {
    }

        /// Forwards an event message as a function of its ID
        /// to a destination which is determined by the routing table.
    virtual void push(IEvent * event) {
        V v;
        typename std::map<int, T *>::iterator iter = this->routingTable.find(v(event));
        if (iter != this->routingTable.end()) {
            (*iter).second->push(event);
        }
    }

        /// Adds an entry to the routing table.
    virtual void addRoutingTableEntry(int address, IEventSink * dst) {
        routingTable[address] = dst;
    }


protected:
        /// A mapping table, which consists of an event ID as key and
        /// a destination as value.
    std::map<int, T *> routingTable;
};

}

#endif
