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


#ifndef _tuiframework_Port_h_
#define _tuiframework_Port_h_

#include "ISerializable.h"

#include <string>
#include <iosfwd>

namespace tuiframework {

/**
 *  Describes a port.
 *  Devices, TUI objects and MSP objects consists of ports of a
 *  specific type.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class Port : public ISerializable {
public:
    enum DataFlowDirection {
        None = 0,
        Source = 1,
        Sink = 2,
        SourceAndSink = 3
    };

    Port();
    Port(const std::string & name, const std::string & typeName, DataFlowDirection dataFlowDirection, const std::string & description = "");
    virtual ~Port();

        /// Returns the port name.
    const std::string & getName() const;
        /// Sets the port name
    void setName(const std::string & name);

        /// Returns a port description.
    const std::string & getDescription() const;
        /// Sets a port description.
    void setDescription(const std::string & description);

        /// Returns the type name of the port.
    const std::string & getTypeName() const;
        /// Sets the type name of the port.
    void setTypeName(const std::string & typeName);

        /// Returns the data flow direction of the port.
    DataFlowDirection getDataFlowDirection() const;
        /// Sets the data flow direction of the port.
    void setDataFlowDirection(DataFlowDirection dataFlowDirection);

    std::ostream & serialize(std::ostream & os) const;
    std::istream & deSerialize(std::istream & is);

protected:
        /// Port name
    std::string name;
        /// Description of the port
    std::string description;
        /// Type name of the port
    std::string typeName;
        /// Data flow direction.
    int dataFlowDirection;
};

}



#endif

