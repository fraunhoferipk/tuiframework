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


#define USE_TFDEBUG
#include "../logging/Logger.h"

#include "PluginShelf.h"
#include "PluginShelfSingleton.h"

#ifdef _WIN32

#include "PluginLibWin32.h"

#else

#include "PluginLib.h"
#include <string.h>
#include <dirent.h>

#endif

#include "DeviceConfig.h"
#include "MSPConfig.h"


using namespace std;

namespace tuiframework {

PluginShelf::PluginShelf() {
}


PluginShelf::~PluginShelf() {
}


#ifdef _WIN32
void PluginShelf::scanDirectory(const std::string & path) {
    WIN32_FIND_DATA fd;
    HANDLE handle = ::FindFirstFile((path + "\\*.dll").c_str(), &fd);
    if (handle != INVALID_HANDLE_VALUE) {
        do {
            PluginLib * pluginLib = PluginLib::loadPluginLib(path + "\\" + fd.cFileName);
            if (pluginLib) {
                this->pluginLibVector.push_back(pluginLib);
                {
                    const vector<string> & v = pluginLib->getDeviceTypeNameVector();
                    vector<string>::const_iterator i = v.begin();
                    vector<string>::const_iterator e = v.end();
                    while (i != e) {
                        this->devicePluginLibMap[*i] = pluginLib;
                        ++i;
                    }
                }

                {
                    const vector<string> & v = pluginLib->getMSPTypeNameVector();
                    vector<string>::const_iterator i = v.begin();
                    vector<string>::const_iterator e = v.end();
                    while (i != e) {
                        this->mspPluginLibMap[*i] = pluginLib;
                        ++i;
                    }
                }
            }

        } while (::FindNextFile(handle, &fd));
        FindClose(handle);
    }
}

#else

void PluginShelf::scanDirectory(const std::string & path) {
    TFDEBUG(path)
    DIR * d = opendir(path.c_str());
    if ( ! d) {
      return;
    }
    
    struct dirent * e;
    while (e = readdir(d)) {
        char * s = strstr(e->d_name, ".so");
        char * s1 = strstr(e->d_name, ".dylib");
        //TFINFO("Scanning ...:" << e->d_name)
        if ((s != 0 && strcmp(s, ".so") == 0) || (s1 != 0 && strcmp(s1, ".dylib") == 0)) {
            TFINFO("found shared library \'" << e->d_name << "\'")
            //PluginLib * pluginLib = PluginLib::loadPluginLib(path + "/" + e->d_name);
            PluginLib * pluginLib = PluginLib::loadPluginLib(path + "/" + e->d_name);
            if (pluginLib) {
                this->pluginLibVector.push_back(pluginLib);
                {
                    const vector<string> & v = pluginLib->getDeviceTypeNameVector();
                    vector<string>::const_iterator i = v.begin();
                    vector<string>::const_iterator e = v.end();
                    while (i != e) {
                        this->devicePluginLibMap[*i] = pluginLib;
                        ++i;
                    }
                }

                {
                    const vector<string> & v = pluginLib->getMSPTypeNameVector();
                    vector<string>::const_iterator i = v.begin();
                    vector<string>::const_iterator e = v.end();
                    while (i != e) {
                        this->mspPluginLibMap[*i] = pluginLib;
                        ++i;
                    }
                }
            }
        }
    }
    
    closedir(d);
}

#endif


void PluginShelf::registerTypes(IEventFactory * eventFactory, IEventChannelFactory * eventChannelFactory) {
    std::vector<PluginLib *>::iterator i = this->pluginLibVector.begin();
    std::vector<PluginLib *>::iterator e = this->pluginLibVector.end();
    while (i != e) {
        (*i)->typeRegistration(eventFactory, eventChannelFactory);
        ++i;
    }
}



void PluginShelf::registerDevices(IDeviceFactory * deviceFactory) {
    std::vector<PluginLib *>::iterator i = this->pluginLibVector.begin();
    std::vector<PluginLib *>::iterator e = this->pluginLibVector.end();
    while (i != e) {
        (*i)->deviceRegistration(deviceFactory);
        ++i;
    }
}


void PluginShelf::registerMSPs(IMSPFactory * mspFactory) {
    std::vector<PluginLib *>::iterator i = this->pluginLibVector.begin();
    std::vector<PluginLib *>::iterator e = this->pluginLibVector.end();
    while (i != e) {
        (*i)->mspRegistration(mspFactory);
        ++i;
    }
}



vector<string> PluginShelf::getDeviceTypeNames() const {
    vector<string> v;
    map<string, PluginLib *>::const_iterator i = this->devicePluginLibMap.begin();
    map<string, PluginLib *>::const_iterator e = this->devicePluginLibMap.end();
    while (i != e) {
        v.push_back((*i).first);
        ++i;
    }
    return v;
}


vector<string> PluginShelf::getMSPTypeNames() const {
    vector<string> v;
    map<string, PluginLib *>::const_iterator i = this->mspPluginLibMap.begin();
    map<string, PluginLib *>::const_iterator e = this->mspPluginLibMap.end();
    while (i != e) {
        v.push_back((*i).first);
        ++i;
    }
    return v;
}

void PluginShelf::freePluginShelf() {
    std::vector<PluginLib *>::iterator i = pluginLibVector.begin();
    std::vector<PluginLib *>::iterator e = pluginLibVector.end();
    while (i != e) {
        TFDEBUG("freePluginLib: " << (*i)->getPath())
        PluginLib::freePluginLib(*i);
        ++i;
    }
    
    pluginLibVector.clear();
    devicePluginLibMap.clear();
    mspPluginLibMap.clear();
}

}

