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

#ifndef _tuiserver_ServerConfig_h_
#define _tuiserver_ServerConfig_h_

#include "ServerStartupConfig.h"

#include "DeviceType.h"
#include "DeviceInstance.h"
#include "Connector.h"
#include "../core/TUIObjectInstance.h"
#include "../core/TUIObjectType.h"
#include "../server/MSPInstance.h"
#include "../server/MSPType.h"
#include "../core/ISerializable.h"

#include <iosfwd>

#include <string>
#include <vector>
#include <map>

namespace tuiframework {

/**
 *  Stores the server configuration which consists of the server startup configuration,
 *  the specification of the device types, device instances, TUI object types and
 *  TUI object instances.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class ServerConfig : public tuiframework::ISerializable {
public:
    ServerConfig();
    virtual ~ServerConfig();

        /// Returns the server start up configuration.
    ServerStartupConfig & getServerStartupConfig();
        /// Returns the server start up configuration.
    const ServerStartupConfig & getServerStartupConfig() const;
        /// Sets the server start up configuration.
    void setServerStartupConfig(const ServerStartupConfig & serverStartupConfig);

        /// Returns a mapping table, which consists of a device type name as key and
        /// a device type description object as value.
    std::map<std::string, tuiframework::DeviceType> & getDeviceTypeMap();
        /// Returns a mapping table, which consists of a device type name as key and
        /// a device type description object as value.
    const std::map<std::string, tuiframework::DeviceType> & getDeviceTypeMap() const;
        /// Sets a mapping table, which consists of a device type name as key and
        /// a device type description object as value.
    void setDeviceTypeMap(const std::map<std::string, tuiframework::DeviceType> & deviceTypeMap);

        /// Returns a mapping table, which consists of a device instance name as key and
        /// a device instance description object as value.
    std::map<std::string, tuiframework::DeviceInstance> & getDeviceInstanceMap();
        /// Returns a mapping table, which consists of a device instance name as key and
        /// a device instance description object as value.
    const std::map<std::string, tuiframework::DeviceInstance> & getDeviceInstanceMap() const;
        /// Sets a mapping table, which consists of a device instance name as key and
        /// a device instance description object as value.
    void setDeviceInstanceMap(const std::map<std::string, tuiframework::DeviceInstance> & deviceInstanceMap);
    
        /// Returns a mapping table, which consists of a TUI object type name as key and
        /// a TUI object type description object as value.
    std::map<std::string, tuiframework::TUIObjectType> & getTUIObjectTypeMap();
        /// Returns a mapping table, which consists of a TUI object type name as key and
        /// a TUI object type description object as value.
    const std::map<std::string, tuiframework::TUIObjectType> & getTUIObjectTypeMap() const;
        /// Sets a mapping table, which consists of a TUI object type name as key and
        /// a TUI object type description object as value.
    void setTUIObjectTypeMap(const std::map<std::string, tuiframework::TUIObjectType> & tuiObjectTypeMap);

        /// Returns a mapping table, which consists of a TUI object instance name as key and
        /// a TUI object instance description object as value.
    std::map<std::string, tuiframework::TUIObjectInstance> & getTUIObjectInstanceMap();
        /// Returns a mapping table, which consists of a TUI object instance name as key and
        /// a TUI object instance description object as value.
    const std::map<std::string, tuiframework::TUIObjectInstance> & getTUIObjectInstanceMap() const;
        /// Sets a mapping table, which consists of a TUI object instance name as key and
        /// a TUI object instance description object as value.
    void setTUIObjectInstanceMap(const std::map<std::string, tuiframework::TUIObjectInstance> & tuiObjectInstanceMap);

        /// Returns a mapping table, which consists of a MSP type name as key and
        /// a MSP type description object as value.
    std::map<std::string, tuiframework::MSPType> & getMSPTypeMap();
        /// Returns a mapping table, which consists of a TUI object type name as key and
        /// a TUI object type description object as value.
    const std::map<std::string, tuiframework::MSPType> & getMSPTypeMap() const;
        /// Sets a mapping table, which consists of a MSP type name as key and
        /// a MSP type description object as value.
    void setMSPTypeMap(const std::map<std::string, tuiframework::MSPType> & tuiObjectTypeMap);

        /// Returns a mapping table, which consists of a MSP instance name as key and
        /// a MSP instance description object as value.
    std::map<std::string, tuiframework::MSPInstance> & getMSPInstanceMap();
        /// Returns a mapping table, which consists of a MSP instance name as key and
        /// a MSP instance description object as value.
    const std::map<std::string, tuiframework::MSPInstance> & getMSPInstanceMap() const;
        /// Sets a mapping table, which consists of a MSP instance name as key and
        /// a MSP instance description object as value.
    void setMSPInstanceMap(const std::map<std::string, tuiframework::MSPInstance> & tuiObjectInstanceMap);

        /// Returns the channel connector map.
    std::vector<Connector> & getConnectorVector();
        /// Returns the channel connector map.
    const std::vector<Connector> & getConnectorVector() const;
        /// Sets the channel connector map.
    void setConnectorVector(const std::vector<Connector> & connectorVector);

    std::ostream & serialize(std::ostream & os) const;
    std::istream & deSerialize(std::istream & is);

protected:
        /// Start up configuration
    ServerStartupConfig serverStartupConfig;

        /// A mapping table, which consists of a device type name as key and
        /// a device type description object as value.
    std::map<std::string, tuiframework::DeviceType> deviceTypeMap;
        /// A mapping table, which consists of a device instance name as key and
        /// a device instance description object as value.
    std::map<std::string, tuiframework::DeviceInstance> deviceInstanceMap;
        /// A mapping table, which consists of a TUI object type name as key and
        /// a TUI object type description object as value.
    std::map<std::string, tuiframework::TUIObjectType> tuiObjectTypeMap;
        /// A mapping table, which consists of a TUI object instance name as key and
        /// a TUI object instance description object as value.
    std::map<std::string, tuiframework::TUIObjectInstance> tuiObjectInstanceMap;

        /// A mapping table, which consists of a MSP type name as key and
        /// a MSP type description object as value.
    std::map<std::string, tuiframework::MSPType> mspTypeMap;
        /// A mapping table, which consists of a MSP instance name as key and
        /// a MSP instance description object as value.
    std::map<std::string, tuiframework::MSPInstance> mspInstanceMap;

        /// A vector of channel connector descriptors.
    std::vector<Connector> connectorVector;
};


}


#endif


