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


#ifndef _tuiframework_EventMsg_h_
#define _tuiframework_EventMsg_h_

#include "IEventMsg.h"

#include <string>
#include <iostream>

namespace tuiframework {

/**
 *  A concrete event message which stores a destination address of type A and
 *  a payload of type T.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template <class A, class T>
class EventMsg : public IEventMsg<A> {
public:
        /// Initializes the event with the delivered destination address and payload.
    EventMsg(const A & address = A(), const T & payload = T()) :
        address(address),
        payload(payload) {
    }


    virtual ~EventMsg() {
    }


    virtual std::ostream & serialize(std::ostream & os) const {
        os << this->address << " " << this->payload;
        return os;
    }


    virtual std::istream & deSerialize(std::istream & is) {
        is >> this->address >> this->payload;
        return is;
    }


    virtual void setAddress(const A & address) {
        this->address = address;
    }


    virtual const A & getAddress() const {
        return this->address;
    }

    virtual void setPayload(const T & payload) {
        this->payload = payload;
    }

    virtual const T & getPayload() const {
        return this->payload;
    }


    virtual int getAddressTypeID() const {
        return this->address.getAddressTypeID();
    }

protected:
    T payload;
        /// Destination address
    A address;
};

}

#endif

