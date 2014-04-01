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


#include "TUIClientAppProvider.h"

namespace tuiframework {

//template<> TUIEventHandler * TUIEventHandlerSingleton::instance = 0;    

template <> std::map<int, TUIClientApp *> TUIClientAppProvider::instanceMap = std::map<int, TUIClientApp *>();

// get TUI Object Source Channel
IEventChannel * getSourceChannel(const std::string & tuiObjectName, const std::string & channelName, int index) {
    return TUIClientAppProvider::getInstance(index)->getTUIObjectStubContainer().getSourceChannel(tuiObjectName, channelName);
}

// get TUI Object Source Channel
IEventChannel * getSinkChannel(const std::string & tuiObjectName, const std::string & channelName, int index) {
    return TUIClientAppProvider::getInstance(index)->getTUIObjectStubContainer().getSinkChannel(tuiObjectName, channelName);
}

bool connectWithTUIServer(
        int mySendPortNr,
        int myReceivePortNr,
        const std::string & serverAddress,
        IEventSink * systemNotificationSink,
        bool ownership,
        int index) {

    return TUIClientAppProvider::getInstance(index)->connectWithTUIServer(
            mySendPortNr,
            myReceivePortNr,
            serverAddress,
            systemNotificationSink,
            ownership);
}


bool disconnectFromTUIServer(int index) {
    return TUIClientAppProvider::getInstance(index)->disconnectFromTUIServer();
}


IEventFactory & getEventFactory(int index) {
    return TUIClientAppProvider::getInstance(index)->getEventFactory();
}


IEventChannelFactory & getEventChannelFactory(int index) {
    return TUIClientAppProvider::getInstance(index)->getEventChannelFactory();
}


void processEvents(int index) {
    TUIClientAppProvider::getInstance(index)->processEvents();
}

}

