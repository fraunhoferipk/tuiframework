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


#ifndef _tuiframework_MSPContainer_h_
#define _tuiframework_MSPContainer_h_

#include "../server/MSPInstance.h"

#include <string>
#include <map>


namespace tuiframework {

class IEventSink;
class IMSP;
class IMSPFactory;

/**
 *  Creates and stores a set of multi stream processor objects.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class MSPContainer {
public:
    MSPContainer(IMSPFactory & mspFactory);
    virtual ~MSPContainer();

        /// Creates the multi stream processor instances by the entries of the description table.
    void createInstances(const std::map<std::string, MSPInstance> & instanceMap);
        /// Frees the multi stream processor instances
    void freeInstances();

    IEventSink * getEventSink(const std::string & instanceName, const std::string & connectorName);

    void registerEventSink(const std::string & instanceName, const std::string & connectorName, IEventSink * eventSink);

protected:
        /// A mapping table, which consists of a string ID as key and
        /// a multi stream processor as value.
    std::map<std::string, IMSP *> instanceMap;
    
    IMSPFactory & mspFactory;
};

}

#endif
