/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    designed for creating applications with a tangible user interface (TUI).
    
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


#ifndef _tuiframework_PluginLib_h_
#define _tuiframework_PluginLib_h_

#include <vector>
#include <map>
#include <string>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

namespace tuiframework {

class IDevice;
class DeviceConfig;
class IMSP;
class MSPConfig;
class IEventFactory; 
class IEventChannelFactory;
class IDeviceFactory;
class IMSPFactory;
/**
 *  PluginLib windows version
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class PluginLib {
public:
        /// instantiates a PluginLib instance
        /// throws an exception if the DLL is incompatible
    static PluginLib * loadPluginLib(const std::string & path);
    static void freePluginLib(PluginLib * pluginLib);

protected:
    PluginLib(const std::string & path);
    virtual ~PluginLib();	
    
    bool openDLL();
    bool isOpen() const;
    bool closeDLL();

    bool readEntityNames();

public:
    void typeRegistration(IEventFactory * eventFactory, IEventChannelFactory * eventChannelFactory);
    void deviceRegistration(IDeviceFactory * deviceFactory);
    void mspRegistration(IMSPFactory * mspFactory);

    const std::vector<std::string> & getDeviceTypeNameVector() const;
    const std::vector<std::string> & getMSPTypeNameVector() const;

    IDevice * newDeviceInstance(const std::string & deviceTypeName, const DeviceConfig & deviceConfig);
    IMSP * newMSPInstance(const std::string & mspTypeName, const MSPConfig & mspConfig);

    void deleteDeviceInstance(IDevice * device);
    void deleteMSPInstance(IMSP * msp);

protected:
	std::string path;

    HINSTANCE dllHandle;
    bool open;

protected:
	std::vector<std::string> deviceNameVector;
    std::vector<std::string> mspNameVector;

    void (* dllInitPlugin)();
    std::string (* dllGetTFVersion)();
    std::string (* dllGetDLLVersion)();
    
    void (* dllTypeRegistration)(IEventFactory * eventFactory, IEventChannelFactory * eventChannelFactory);
    void (* dllDeviceRegistration)(IDeviceFactory * deviceFactory);
    void (* dllMSPRegistration)(IMSPFactory * mspFactory);
    
    const std::vector<std::string> & (* dllGetDeviceTypeNameVector)();
    std::string (* dllGetDeviceTypeVersion)(const std::string & deviceTypeName);
    IDevice * (* dllNewDeviceInstance)(const std::string & deviceTypeName, const DeviceConfig & deviceConfig);
    void (* dllDeleteDeviceInstance)(IDevice * device);
    
    const std::vector<std::string> & (* dllGetMSPTypeNameVector)();
    std::string (* dllGetMSPTypeVersion)(const std::string & mspTypeName);
    IMSP * (* dllNewMSPInstance)(const std::string & mspTypeName, const MSPConfig & mspConfig);
    void (* dllDeleteMSPInstance)(IMSP * msp);

};

}

#endif