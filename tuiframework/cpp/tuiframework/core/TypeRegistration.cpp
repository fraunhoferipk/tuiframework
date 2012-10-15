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


#include "TypeRegistration.h"
#include <map>

using namespace std;

namespace tuiframework {

template<> int SystemMsg::eventTypeID = 1;
template<> int SystemCmdMsg::eventTypeID = 2;
template<> int AttachedObjectsMsg::eventTypeID = 3;
template<> int MulticastGroupInvitationMsg::eventTypeID = 4;
template<> int GUIDEventTypeIDVectorMsg::eventTypeID = 5;
template<> int HostEvent::eventTypeID = 6;

int EPEventMsgTypeIDOffset = 10;


void initTypeRegistration(IEventFactory & eventFactory) {
    doEventRegistration<SystemMsg>(eventFactory);
    doEventRegistration<SystemCmdMsg>(eventFactory);
    doEventRegistration<AttachedObjectsMsg>(eventFactory);
    doEventRegistrationWithArg<HostEvent>(eventFactory);
    doEventRegistration<GUIDEventTypeIDVectorMsg>(eventFactory);
    doEventRegistration<MulticastGroupInvitationMsg>(eventFactory);
}

}


