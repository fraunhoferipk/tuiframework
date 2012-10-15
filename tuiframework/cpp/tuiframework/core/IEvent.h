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


#ifndef _tuiframework_IEvent_h_
#define _tuiframework_IEvent_h_

#include "ISerializable.h"

#include <iosfwd>

namespace tuiframework {

/**
 *  Represents an event.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class IEvent : public ISerializable {
public:
        /// Contains an operator which returns the address type ID.
    struct DstAddressTypeID {
        int operator()(const IEvent * a) const {
            return a->getAddressTypeID();
        }
    };

        /// Contains an operator which returns the event type ID.
    struct EventTypeID {
        int operator()(const IEvent * a) const {
            return a->getEventTypeID();
        }
    };
public:
    virtual ~IEvent() { }

        /// returns the type ID of the destination address
    virtual int getAddressTypeID() const = 0;
        /// returns the type ID of the event
    virtual int getEventTypeID() const = 0;
        /// returns a duplicate of the event
    virtual IEvent * getDuplicate() const = 0;
};

}

#endif

