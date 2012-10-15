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


#ifndef _tuiframework_TUIObjectStubContainer_h_
#define _tuiframework_TUIObjectStubContainer_h_

#include "../core/TUIObjectDescriptor.h"
#include "../core/TypeRegistration.h"

#include <string>
#include <vector>
#include <map>

namespace tuiframework {

class IEvent;
class IEventSink;
class IEventChannel;
class IEventChannelFactory;
class TUIObjectStub;

/**
 *  Creates and stores a set of TUI stub objects.
 *  The TUI stub objects are created by a table of TUI descriptions
 *  of instantiated TUI objects.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class TUIObjectStubContainer {
public:
    TUIObjectStubContainer(IEventSink & eventSink, IEventChannelFactory & eventChannelFactory);
    ~TUIObjectStubContainer();

        /// Initializes the TUI stub container by the TUI object instance vector
        /// and the appropriate TUI object type vector.
        /// Creates the TUI stubs by the description of
        /// instantiated TUI objects and passes the event sink to them.
    void createStubs(const std::vector<TUIObjectInstance> & instanceVector,
            const std::vector<TUIObjectType> & typeVector);

        /// Returns a TUI object stub by its ID.
    TUIObjectStub * getStub(int id);
        /// Returns a TUI object stub by its name.
    TUIObjectStub * getStub(const std::string & name);

        /// Returns a source event channel by the TUI object stub name and by the channel name.
    IEventChannel * getSourceChannel(const std::string & stubName, const std::string & channelName);
        /// Returns a sink event channel by the device stub name and the channel name.
    IEventChannel * getSinkChannel(const std::string & stubName, const std::string & channelName);

        /// Delegates IEventMsg<EPAddress> objects to the appropiate TUI object stubs.
    void handleEvent(IEventMsg<EPAddress> * eventMsg);


protected:
        /// A mapping table, which consists of a TUI object name as key and
        /// a TUI object ID as value.
    std::map<std::string, int> nameIDMap;

        /// A mapping table, which consists of a TUI object ID as key and
        /// a TUI object stub as value.
    std::map<int, TUIObjectStub *> stubMap;

    IEventSink & eventSink;
    IEventChannelFactory & eventChannelFactory;
};

}

#endif

