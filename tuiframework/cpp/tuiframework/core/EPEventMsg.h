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


#ifndef _tuiframework_EPEventMsg_h_
#define _tuiframework_EPEventMsg_h_

#include "EventMsg.h"
#include "EPAddress.h"

#include <map>

namespace tuiframework {
    
/**
 *  A concrete event message which stores a device channel address
 *  and a payload of type T.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template <class T, int typeID>
class EPEventMsg : public EventMsg<EPAddress, T> {
public:
    // TODO Provide a constructor without destination address
        /// Initializes the event message with the device ID and
        /// channel number as address and the payload.
    EPEventMsg(int entityID = -1, int portNr = -1, const T & payload = T()) :
        EventMsg<EPAddress, T>(EPAddress(entityID, portNr), payload) {
    }

    EPEventMsg(const EPAddress & address, const T & payload = T()) :
        EventMsg<EPAddress, T>(address, payload) {
    }

    virtual ~EPEventMsg() {
    }


    virtual IEvent * getDuplicate() const {
        return new EPEventMsg(this->address, this->payload);
    }
    

    virtual int getEventTypeID() const {
        return typeID;
    }
    
    static int EventTypeID() {
        return typeID;
    }
};

}

#endif
