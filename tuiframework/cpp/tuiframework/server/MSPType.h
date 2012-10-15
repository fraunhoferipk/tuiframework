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


#ifndef _tuiframework_MSPType_h_
#define _tuiframework_MSPType_h_

#include "../core/Port.h"

#include <string>
#include <map>

namespace tuiframework {
/**
 *  MSPType
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class MSPType {
public:
    MSPType();
    virtual ~MSPType();

        /// Returns the multi stream processor type name.
    const std::string & getTypeName() const;
        /// Sets the multi stream processor type name.
    void setTypeName(const std::string & typeName);

        /// Sets the additional description.
    void setDescription(const std::string & description);
        /// Returns the additional description.
    const std::string & getDescription() const;
        /// Returns a mapping table, which consists of a port name as key and a port type as value.
    std::map<std::string, Port> & getPortMap();
        /// Returns a mapping table, which consists of a port name as key and a port type as value.
    const std::map<std::string , Port> & getPortMap() const;
        /// Sets a mapping table, which consists of a port name as key and a port type as value.
    void setPortMap(const std::map<std::string, Port> & portMap);

protected:
        /// multi stream processor type name
    std::string typeName;
        /// Additional description
    std::string description;
        /// A mapping table, which consists of a port name as key and a port type as value.
    std::map<std::string, Port> portMap;
        /// A mapping table, which consists of a parameter name as key and a parameter type name as value.
    std::map<std::string, std::string> parameterTypeMap;
};


}

#endif
