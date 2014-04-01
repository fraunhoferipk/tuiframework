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


#ifndef _tuiframework_TUIClientAppSingleton_h_
#define _tuiframework_TUIClientAppSingleton_h_

#include "TUIClientApp.h"

#include "../core/Multiton.h"

namespace tuiframework {

 /**
 *
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */    
    
typedef Multiton<TUIClientApp> TUIClientAppProvider;

class IEventChannelFactory;
class IEventFactory;
class IEventSink;
class IEventChannel;

/// returns the TUI object source channel by its name
IEventChannel * getSourceChannel(const std::string & tuiObjectName, const std::string & channelName, int index = 0);
/// returns the TUI object source channel by its name
IEventChannel * getSinkChannel(const std::string & tuiObjectName, const std::string & channelName, int index = 0);


bool connectWithTUIServer(int mySendPortNr,
        int myReceivePortNr,
        const std::string & serverAddress,
        IEventSink * systemNotificationSink,
        bool ownership = true, int index = 0);



bool disconnectFromTUIServer(int index = 0);
void processEvents(int index = 0);

IEventFactory & getEventFactory(int index = 0);
IEventChannelFactory & getEventChannelFactory(int index = 0);

}

#endif

