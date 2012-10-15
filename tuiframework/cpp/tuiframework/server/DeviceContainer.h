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


#ifndef _tuiframework_DeviceContainer_h_
#define _tuiframework_DeviceContainer_h_

#include "DeviceInstance.h"
#include "../server/DeviceDescriptor.h"

#include <string>
#include <map>
#include <vector>


namespace tuiframework {
    class ITUIServer;
    class IDevice;
    class IDeviceFactory;

/**
 *  Creates and stores a set of device objects.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class DeviceContainer {
public:
    DeviceContainer(IDeviceFactory & deviceFactory);
    virtual ~DeviceContainer();

        /// Creates the device instances by the entries of the device instance description table.
    void createDeviceInstances(const std::map<std::string, tuiframework::DeviceInstance> & deviceInstanceMap);
        /// Frees the device instances
    void freeDeviceInstances();

        /// Connects all devices with the TUI server
    void connectDevices(tuiframework::ITUIServer & tuiServer);
        /// Disconnects all devices from the TUI server
    void disconnectDevices();
        /// Executes all devices
    bool executeDevices();

        /// Returns a table of device descriptors of the instantiated devices.
    std::vector<tuiframework::DeviceDescriptor> getDeviceDescriptorVector() const;

    int getEntityIDByName(const std::string & name) const;
    int getChannelIDByName(int entityID, const std::string & channelName) const;

protected:
        /// A mapping table, which consists of a device ID as key and
        /// a device as value.
    std::map<std::string, int> nameIDMap;
    std::map<int, IDevice *> deviceMap;
    
    IDeviceFactory & deviceFactory;
};

}

#endif

