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


#ifndef _tuiframework_TUIObjectStub_h_
#define _tuiframework_TUIObjectStub_h_

#include "../core/IEventSink.h"
#include "../core/TypeRegistration.h"

#include <string>
#include <map>

namespace tuiframework {

class TUIObjectInstance;
class TUIObjectType;
class IEventChannel;
class IEventChannelFactory;

/**
 *  A stub for a TUI objects on the client side.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class TUIObjectStub : protected IEventSink {
public:
        /// Constructs and initializes a TUI object stub by the delivered
        /// TUI object instance and type and event sink.
    TUIObjectStub(const TUIObjectInstance & instance,
                  const TUIObjectType & type,
                  IEventSink & eventSink,
                  IEventChannelFactory & eventChannelFactory);
    virtual ~TUIObjectStub();

    int getID() const;
        /// Delegates IEventMsg<EPAddress> objects to the appropiate channels.
    void handleEvent(IEventMsg<EPAddress> * eventMsg);

        /// Returns the source channel by its number.
    IEventChannel * getSourceChannel(int portNr);
        /// Returns the sink channel by its number.
    IEventChannel * getSinkChannel(int portNr);

        /// Returns the source channel by its name.
    IEventChannel * getSourceChannel(const std::string & channelName);
        /// Returns the sink channel by its name.
    IEventChannel * getSinkChannel(const std::string & channelName);

    int getSinkChannelNr(const std::string & channelName);

protected:
        /// Forwards the event to the set event sink.
    virtual void push(IEvent * event);

protected:
        /// Sink for events.
    IEventSink & eventSink;
    
    IEventChannelFactory & eventChannelFactory;
    
    int id;
        /// Name of the TUI Object.
    std::string name;

        /// A mapping table of source channel entries, which consists of a
        /// channel number as key and an event channel as value.
        /// A source event channel emits events.
    std::map<int, IEventChannel *> sourceChannelMap;
        /// A mapping table of sink channel entries, which consists of a
        /// channel number as key and an event channel as value.
        /// A sink event channel receives events.
    std::map<int, IEventChannel *> sinkChannelMap;
        /// A mapping table, which consists of a channel name as key and
        /// a channel number as value.
    std::map<std::string, int> nameChannelNrMap;
};

}

#endif

