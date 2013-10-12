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


#ifndef _tuiframework_Events_h_
#define _tuiframework_Events_h_

#include "../logging/Logger.h"

#include "../core/EventChannel.h"
#include "../core/Event.h"
#include "../core/EPEventMsg.h"

#include "../core/AttachedObjects.h"
#include "../core/GUIDEventTypeIDVector.h"
#include "../core/HostAddress.h"
#include "../core/HostEventMsg.h"

#include "../core/IEventFactory.h"
#include "../core/IEventChannelFactory.h"

#include "SystemCmd.h"

#include <string>

namespace tuiframework {

 /**
 *  Functions for registering new types
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */    
    
typedef Event<char>                     SystemMsg;
typedef Event<SystemCmd>                SystemCmdMsg;
typedef Event<AttachedObjects>          AttachedObjectsMsg;
typedef Event<HostAddress>              MulticastGroupInvitationMsg;
typedef Event<GUIDEventTypeIDVector>    GUIDEventTypeIDVectorMsg;
typedef HostEventMsg<IEvent *>          HostEvent;


template <class T> IEvent * createEventFunction(void * arg = 0) {
    return new T();
}


template <class T> IEvent * createEventFunctionWithArg(void * arg = 0) {
    return new T(arg);
}


template <class T> IEventChannel * createEventChannelFunction(void * arg = 0) {
    std::pair<int, int> * v = static_cast<std::pair<int, int> *>(arg);
    return new T(v->first, v->second);
}


template<class T, int typeID> void registerType(IEventFactory * eventFactory,
                                                IEventChannelFactory * eventChannelFactory, 
                                                const std::string & name = std::string()) {
    if (eventFactory) {
        eventFactory->registerCreateFunction(typeID, createEventFunction<EPEventMsg<T, typeID> >);
    }
    if (eventChannelFactory) {
        eventChannelFactory->registerCreateFunction(name, createEventChannelFunction<EventChannel<T, typeID> >);
    }
    //TFDEBUG("registerType ID = " << typeID << ", channel name = " << name)
}


template<class T>
void doEventRegistration(IEventFactory & eventFactory) {
    eventFactory.registerCreateFunction(T().getEventTypeID(), createEventFunction<T>);
}


template<class T>
void doEventRegistrationWithArg(IEventFactory & eventFactory) {
    eventFactory.registerCreateFunction(T().getEventTypeID(), createEventFunctionWithArg<T>);
}


void initTypeRegistration(IEventFactory & eventFactory);

extern int EPEventMsgTypeIDOffset;

}

#endif

