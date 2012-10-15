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


#ifndef _tuiframework_DeviceType_h_
#define _tuiframework_DeviceType_h_

#include "../core/Port.h"
#include "../core/ISerializable.h"

#include <string>
#include <map>
#include <iosfwd>

namespace tuiframework {

/**
 *  Represents a device type.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class DeviceType : public ISerializable {
public:
    DeviceType();
    virtual ~DeviceType();

        /// Returns the name of the device type.
    const std::string & getDeviceTypeName() const;
        /// Sets the name of the device type.
    void setDeviceTypeName(const std::string & deviceTypeName);

        /// Returns the description of the device type.
    const std::string & getDescription() const;
        /// Sets the description of the device type.
    void setDescription(const std::string & description);


        /// Returns a mapping table, which consists of a port name as key and port type as value.
    const std::map<std::string, Port> & getPortMap() const;
        /// Returns a mapping table, which consists of a port name as key and port type as value.
    std::map<std::string, Port> & getPortMap();
        /// Sets a mapping table, which consists of a port name as key and port type as value.
    void setPortMap(const std::map<std::string, Port> & portMap);

    std::ostream & serialize(std::ostream & os) const;
    std::istream & deSerialize(std::istream & is);

protected:
        /// Name of the device type.
    std::string deviceTypeName;
        /// Description of the device type
    std::string description;
        /// Mapping table, which consists of a port name as key and port type as value.
    std::map<std::string, Port> portMap;
};

}

#endif
