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


#ifndef _tuiframework_DeviceConfig_h_
#define _tuiframework_DeviceConfig_h_

#include "ParameterGroup.h"
#include "../core/ISerializable.h"


#include <map>
#include <string>
#include <iosfwd>

namespace tuiframework {

/**
 *  Describes a device instance configuration.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class DeviceConfig : public ISerializable {
public:
    DeviceConfig(const ParameterGroup & parameterGroup);
    virtual ~DeviceConfig();

        /// Returns the device ID
    int getEntityID() const;
        /// Sets the device ID
    void setEntityID(int entityID);

        /// Returns the name of the instantiated device.
    const std::string & getInstanceName() const;
        /// Sets the name of the instantiated device.
    void setInstanceName(const std::string & instanceName);
        /// Sets the type name of the instantiated device.
    void setTypeName(const std::string & typeName);
    const std::string & getTypeName() const;

    const ParameterGroup & getParameterGroup() const;
    ParameterGroup & getParameterGroup();

    std::ostream & serialize(std::ostream & os) const;
    std::istream & deSerialize(std::istream & is);

protected:
        /// device ID
    int entityID;
        /// Name of the instantiated device.
    std::string instanceName;
        /// Name of the device type of the instantiated device.
    std::string typeName;

    ParameterGroup parameterGroup;
};

}

#endif
