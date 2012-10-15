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


#ifndef _tuiframework_Event_h_
#define _tuiframework_Event_h_

#include "IEvent.h"

#include <string>
#include <iostream>

namespace tuiframework {

/**
 *  A concrete event which stores a payload of type T.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template <class T>
class Event : public IEvent {
public:
        /// Initializes the event with the delivered payload.
    Event(const T & payload = T()) :
        payload(payload) {
    }

    virtual ~Event() {
    }
    

    virtual std::ostream & serialize(std::ostream & os) const {
        os << this->payload;
        return os;
    }


    virtual std::istream & deSerialize(std::istream & is) {
        is >> this->payload;
        return is;
    }

        /// Sets the payload of the event.
    virtual void setPayload(const T & payload) {
        this->payload = payload;
    }


        /// Returns the payload of the event.
    virtual const T & getPayload() const {
        return this->payload;
    }

        /// Returns 0, because this event type does'nt store a destination address.
    virtual int getAddressTypeID() const {
        return 0;
    }

        /// Returns the event type ID.
    virtual int getEventTypeID() const {
        return Event<T>::eventTypeID;
    }

        /// Returns a clone of the event
    virtual IEvent * getDuplicate() const {
        return new Event<T>(this->payload);
    }
    
        /// Returns the event type ID.
    static int EventTypeID() {
        return Event<T>::eventTypeID;
    }

protected:
        /// Type ID of the event
    static int eventTypeID;
        /// Payload
    T payload;
};

}

#endif

