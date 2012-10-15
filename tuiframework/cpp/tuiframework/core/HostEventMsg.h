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


#ifndef _tuiframework_HostEventMsg_h_
#define _tuiframework_HostEventMsg_h_

#include "EventMsg.h"
#include "HostAddress.h"
#include "IEventFactory.h"

#include "../logging/Logger.h"

namespace tuiframework {

/**
 *  A concrete event message which stores a host address
 *  and a payload of type T.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template <class T>
class HostEventMsg : public EventMsg<HostAddress, T> {
public:
    HostEventMsg(void * arg) {
        this->eventFactory = static_cast<IEventFactory *>(arg);
    }
    
        /// Initializes the event message with the target device ID,
        /// the channel number and the payload.
    HostEventMsg(const HostAddress & ipAddress = HostAddress(), const T & payload = T()) :
        EventMsg<HostAddress, T>(ipAddress, payload),
        eventFactory(0) {
    }

        /// Destructor (deletes the payload)
    virtual ~HostEventMsg() {
        delete this->payload;
    }

        /// Serializes the header which consists of the ip address, port number and a flag which indicates
        /// whether this event has a payload or not.
    static std::ostream & serializeHeader(const HostAddress & ipAddress, bool hasPayload, std::ostream & os) {
        os << ipAddress << " ";
        if (hasPayload) {
            os << "1";
        } else {
            os << "0";
        }
        return os;
    }
    
    
    virtual std::ostream & serialize(std::ostream & os) const {
        HostEventMsg::serializeHeader(this->address, this->payload != 0, os);
        if (this->payload) {
            os << " " << this->payload;
        }
        return os;
    }


    virtual std::istream & deSerialize(std::istream & is) {
        is >> this->address;
        int hasPayload;
        is >> hasPayload;
        if (hasPayload == 1 && eventFactory) {
            int eventType;
            is >> eventType;
            this->payload = this->eventFactory->createInstance(eventType);
            if ( ! this->payload) {
                TFERROR("[HostEventMsg] payload == 0");
            }
            is >> this->payload;
        }
        return is;
    }


    virtual IEvent * getDuplicate() const {
        return new HostEventMsg(this->address, this->payload);
    }


    virtual int getEventTypeID() const {
        return HostEventMsg<T>::eventTypeID;
    }


        /// Returns the type ID of the event
    static int EventTypeID() {
        return HostEventMsg<T>::eventTypeID;
    }

protected:
    IEventFactory * eventFactory;
    static int eventTypeID;
};

}

#endif
