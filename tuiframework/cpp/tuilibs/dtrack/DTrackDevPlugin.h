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



#ifndef _DTrackDevPlugin_h_
#define _DTrackDevPlugin_h_

#include <string>
#include <vector>

namespace tuiframework {
    class IDevice;
    class IDeviceFactory;
    class DeviceConfig;
}

extern "C" {
namespace tuiframework {
    void soInitPlugin();
    std::string soGetTFVersion();
    std::string soGetSOVersion();
    void soDeviceRegistration(IDeviceFactory * deviceFactory);
    const std::vector<std::string> & soGetDeviceTypeNameVector();
    std::string soGetDeviceTypeVersion(const std::string & deviceTypeName);
    IDevice * soNewDeviceInstance(const std::string & deviceTypeName, DeviceConfig & deviceConfig);
    void soDeleteDeviceInstance(IDevice * device);
}
}

#endif
