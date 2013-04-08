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


#ifndef _tuiframework_EventDelegate_h_
#define _tuiframework_EventDelegate_h_

#include "IEventSink.h"

#include <iostream>

namespace tuiframework {

/**
 *  Delegates an event to a specified method.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
*/

template <class E, class R>
class EventDelegate : public IEventSink {
public:
    EventDelegate() {
    }

        /// Construction by delivering the destination method.
    EventDelegate(R * receiver, void (R:: * method)(const E *)) :
        receiver(receiver),
        method(method) {
    }

    virtual ~EventDelegate() {
    }

        /// Sets the destination method.
    void setReceiver(R * receiver, void (R:: * method)(const E *)) {
        this->receiver = receiver;
        this->method = method;
    }

        /// Forwards the delivered event to the destination method.
    virtual void push(IEvent * event) {
        E * e = static_cast<E *>(event);
        (this->receiver->*this->method)(e);
    }

protected:
        /// Class instance pointer.
    R * receiver;
        /// Method pointer.
    void (R:: * method)(const E *);
};



template <class E, class R>
class EventDelegateNC : public IEventSink {
public:
    EventDelegateNC() {
    }

        /// Construction by delivering the destination method.
    EventDelegateNC(R * receiver, void (R:: * method)(E *)) :
        receiver(receiver),
        method(method) {
    }

    virtual ~EventDelegateNC() {
    }

        /// Sets the destination method.
    void setReceiver(R * receiver, void (R:: * method)(E *)) {
        this->receiver = receiver;
        this->method = method;
    }

        /// Forwards the delivered event to the destination method.
    virtual void push(IEvent * event) {
        E * e = static_cast<E *>(event);
        (this->receiver->*this->method)(e);
    }

protected:
        /// Class instance pointer.
    R * receiver;
        /// Method pointer.
    void (R:: * method)(E *);
};


}

#endif

