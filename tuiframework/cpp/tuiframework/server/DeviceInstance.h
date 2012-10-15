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


#ifndef _tuiframework_DeviceInstance_h_
#define _tuiframework_DeviceInstance_h_

#include "ParameterGroup.h"
#include "../core/ISerializable.h"


#include <map>
#include <string>
#include <iosfwd>

namespace tuiframework {

/**
 *  Specifies a device for instantiation.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class DeviceInstance : public ISerializable {
public:
    DeviceInstance();
    virtual ~DeviceInstance();

        /// Returns the name of the device instance.
    const std::string & getName() const;
        /// Sets the name of the device instance.
    void setName(const std::string & name);

        /// Returns the name of the device type.
    const std::string & getDeviceTypeName() const;
        /// Sets the name of the device type.
    void setDeviceTypeName(const std::string & deviceTypeName);

    void setParameterGroup(const ParameterGroup & parameterGroup);
    const ParameterGroup & getParameterGroup() const;

    std::ostream & serialize(std::ostream & os) const;
    std::istream & deSerialize(std::istream & is);

protected:
        /// Name of the device instance
    std::string name;
        /// Name of the device type
    std::string deviceTypeName;

    ParameterGroup parameterGroup;
};

}


#endif

