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


#ifndef _tuiframework_DeviceDescriptor_h_
#define _tuiframework_DeviceDescriptor_h_

#include "DeviceConfig.h"
#include "DeviceType.h"
#include "../core/ISerializable.h"

#include <map>
#include <iosfwd>

namespace tuiframework {


/**
 *  A Describtion of an instantiated device.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class DeviceDescriptor : public ISerializable {
public:
    DeviceDescriptor();
    virtual ~DeviceDescriptor();

    void setInstanceName(const std::string & instanceName);
    const std::string & getInstanceName() const;

        /// Returns the device ID.
    int getEntityID() const;
        /// Sets the configuration of the instantiated device.
    void setEntityID(int entityID);
    
        /// Returns the type description of the instantiated device.
    const DeviceType & getDeviceType() const;
    /// Returns the type description of the instantiated device.
    DeviceType & getDeviceType();
        /// Sets the type description of the instantiated device.
    void setDeviceType(const DeviceType & deviceType);
    
        /// Returns a mapping table, which consists of a channel name as key and channel number as value.
    const std::map<std::string, int> & getNameChannelNrMap() const;
        /// Returns a mapping table, which consists of a channel name as key and channel number as value.
    std::map<std::string, int> & getNameChannelNrMap();
        /// Sets a mapping table, which consists of a channel name as key and channel number as value.
    void setNameChannelNrMap(const std::map<std::string, int> & nameChannelNrMap);

    std::ostream & serialize(std::ostream & os) const;
    std::istream & deSerialize(std::istream & is);

protected:

    int entityID;
    std::string instanceName;
        /// Device type description.
    DeviceType deviceType;
        /// A mapping table, which consists of a channel name as key and channel number as value.
    std::map<std::string, int> nameChannelNrMap;
};

}

#endif


