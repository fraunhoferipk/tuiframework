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


#ifndef _tuiframework_TUIEventHandlerSingleton_h_
#define _tuiframework_TUIEventHandlerSingleton_h_

#include "TUIEventHandler.h"
#include "../core/Singleton.h"

#include "TUIClientAppProvider.h"

namespace tuiframework {
 /**
 *
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
    /// Singleton of the TUI event handler.
typedef Singleton<TUIEventHandler> TUIEventHandlerSingleton;

}

#define CONNECT(EventCType, ObjName, ChName, DstCType, DstCInstance, MethodPtr) \
    MCONNECT(EventCType, ObjName, ChName, DstCType, DstCInstance, MethodPtr, 0)

#define MCONNECT(EventCType, ObjName, ChName, DstCType, DstCInstance, MethodPtr, index) \
    try { \
        tuiframework::connect<EventCType, DstCType>(tuiframework::getSourceChannel(ObjName, ChName, index), DstCInstance, MethodPtr); \
    } catch (tuiframework::Exception & e) { \
        e.addErrorMessage("CONNECT failed", __FILE__, __LINE__); \
        throw e; \
    }

#define DISCONNECT(EventCType, ObjName, ChName, DstCType, DstCInstance, MethodPtr) \
    MDISCONNECT(EventCType, ObjName, ChName, DstCType, DstCInstance, MethodPtr, 0)

#define MDISCONNECT(EventCType, ObjName, ChName, DstCType, DstCInstance, MethodPtr, index) \
    try { \
        tuiframework::disconnect<EventCType, DstCType>(tuiframework::getSourceChannel(ObjName, ChName, index), DstCInstance, MethodPtr); \
    } catch (tuiframework::Exception & e) { \
        e.addErrorMessage("DISCONNECT failed", __FILE__, __LINE__); \
        throw e; \
    }

namespace tuiframework {

    /// Connects the source event channel with a functor.
template <class E, class R>
void connect(IEventChannel * source, R * receiver, void (R:: * method)(const E *)) {
    TUIEventHandlerSingleton::getInstance()->channelConnect<E, R>(source, receiver, method);
}

    /// Disconnects the source event channel from a functor.
template <class E, class R>
void disconnect(IEventChannel * source, R * receiver, void (R:: * method)(const E *)) {
    TUIEventHandlerSingleton::getInstance()->channelDisconnect<E, R>(source, receiver, method);
}

}

#endif


