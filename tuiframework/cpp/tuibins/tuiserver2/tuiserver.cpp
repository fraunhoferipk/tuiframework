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


#define USE_TFDEBUG

#include <tuiframework/server/PluginShelfSingleton.h>
#include <tuiframework/xml/ServerConfigXMLReader.h>
#include <tuiframework/server/ServerConfig.h>

#include <tuiframework/server/TUIServerApp.h>
#include <tuiframework/core/TypeRegistration.h>
#include <tuiframework/logging/Logger.h>

#include <map>
#include <string>

#include <iostream>
#include <stdlib.h>

#include <cstdio> // GCC 4.3 related build problem

using namespace tuiframework;
using namespace std;


int main(int argc, char* argv[]) {

    if (argc != 5) {
        cout << "Usage tuiserver <sender port> <receiver port> <server config path> <plugin directory path>" << endl;
        return 1;
    }

    PluginShelf * pluginShelf = PluginShelfSingleton::getInstance();
    pluginShelf->scanDirectory(argv[4]);

    TUIServerApp serverApp(atoi(argv[1]), atoi(argv[2]));
    tuiframework::initTypeRegistration(serverApp.getEventFactory());

    pluginShelf->registerTypes(&serverApp.getEventFactory(), 0);
    pluginShelf->registerDevices(&serverApp.getDeviceFactory());
    pluginShelf->registerMSPs(&serverApp.getMSPFactory());
    
    ServerConfig serverConfig;
    ServerConfigXMLReader serverConfigXMLReader;
    if (serverConfigXMLReader.readServerConfig(argv[3])) {
        serverConfig = serverConfigXMLReader.getServerConfig();
    } else {
        TFERROR("Error reading configuration");
        return 0;
    }
    serverApp.init(serverConfig);
    serverApp.tuiServerExecute();

    getchar();
    cout << "before serverApp.tuiServerExit()" << endl;
    serverApp.tuiServerExit();
    cout << "after serverApp.tuiServerExit()" << endl;
    
    cout << "before pluginShelf->freePluginShelf()" << endl;
    pluginShelf->freePluginShelf();
    cout << "after pluginShelf->freePluginShelf()" << endl;
    getchar();
    // todo: delete the instance within the plugin libs 
    
    return 0;
}

