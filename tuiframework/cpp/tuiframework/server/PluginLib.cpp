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


#include "PluginLib.h"

#include "../core/Version.h"
#define USE_TFDEBUG
#include "../logging/Logger.h"


#include <dlfcn.h>

namespace tuiframework {

using namespace std;

PluginLib * PluginLib::loadPluginLib(const std::string & path) {
    PluginLib * pluginLib = new PluginLib(path);

    if ( ! pluginLib->openSO()) {
        delete pluginLib;
        return 0;
    }
    pluginLib->readEntityNames();
    /*
    if ( ! pluginLib->readEntityNames()) {
        delete pluginLib;
        return 0;
    }
    */

    return pluginLib;
}


void PluginLib::freePluginLib(PluginLib * pluginLib) {
    /*
    if (pluginLib->isOpen()) {
        pluginLib->closeSO();
    }
    */
    delete pluginLib;
}


PluginLib::PluginLib(const string & path) :
    path(path),
    soHandle(0),
    open(false),
    soGetTFVersion(0),
    soGetSOVersion(0),
    soTypeRegistration(0),
    soDeviceRegistration(0),
    soMSPRegistration(0),
    soGetDeviceTypeNameVector(0),
    soGetDeviceTypeVersion(0),
    soNewDeviceInstance(0),
    soDeleteDeviceInstance(0),
    soGetMSPTypeNameVector(0),
    soGetMSPTypeVersion(0),
    soNewMSPInstance(0),
    soDeleteMSPInstance(0) {
}


PluginLib::~PluginLib() {
    if (this->isOpen()) {
        this->closeSO();
    }
}


bool PluginLib::openSO() {
    if (this->soHandle) {
        return false;
    }

    bool genRes = true;
    bool typeRes = true;
    bool devRes = true;
    bool mspRes = true;
 
    this->soHandle = dlopen(this->path.c_str(), RTLD_LAZY);
    if ( ! this->soHandle) {
        TFERROR(" Could not open: " << this->path.c_str());
        TFERROR(" ->  " << dlerror());
        return false;
    }
    
    this->soGetTFVersion = reinterpret_cast<std::string (*)()>(dlsym(this->soHandle, "soGetTFVersion"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soGetTFVersion not found")
        genRes = false;
    }
    
    if (this->soGetTFVersion) {
        if (this->soGetTFVersion().compare(TUIFRAMEWORK_VERSION)) {
            genRes = false;
            TFDEBUG(" soGetTFVersion not equal!")
            TFDEBUG("   " << this->path.c_str() << " version: " << this->soGetTFVersion());
            TFDEBUG("   tuiframework version: " << TUIFRAMEWORK_VERSION);
        }
    }
    
    this->soInitPlugin = reinterpret_cast<void (*)()>(dlsym(this->soHandle, "soInitPlugin"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soInitPlugin not found")
        genRes = false;
    }
    
    dlerror();
    this->soGetSOVersion = reinterpret_cast<std::string (*)()>(dlsym(this->soHandle, "soGetSOVersion"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soGetSOVersion not found")
        genRes = false;
    }

    this->soTypeRegistration = reinterpret_cast<void (*)(IEventFactory * eventFactory, IEventChannelFactory * eventChannelFactory)>(dlsym(this->soHandle, "soTypeRegistration"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soTypeRegistration not found")
        typeRes = false;
    }
    
    this->soDeviceRegistration = reinterpret_cast<void (*)(IDeviceFactory * deviceFactory)>(dlsym(this->soHandle, "soDeviceRegistration"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soDeviceRegistration not found")
        devRes = false;
    }
    
    this->soMSPRegistration = reinterpret_cast<void (*)(IMSPFactory * deviceFactory)>(dlsym(this->soHandle, "soMSPRegistration"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soMSPRegistration not found")
        mspRes = false;
    }
    
    this->soGetDeviceTypeNameVector = reinterpret_cast<const std::vector<std::string> & (*)()>(dlsym(this->soHandle, "soGetDeviceTypeNameVector"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soGetDeviceTypeNameVector not found")
        devRes = false;
    }

    this->soGetDeviceTypeVersion = reinterpret_cast<std::string (*)(const std::string &)>(dlsym(this->soHandle, "soGetDeviceTypeVersion"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soGetDeviceTypeVersion not found")
        devRes = false;
    }

    this->soNewDeviceInstance = reinterpret_cast<IDevice * (*)(const std::string &, const DeviceConfig &)>(dlsym(this->soHandle, "soNewDeviceInstance"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soNewDeviceInstance not found")
        devRes = false;
    }

    this->soDeleteDeviceInstance = reinterpret_cast<void (*)(IDevice *)>(dlsym(this->soHandle, "soDeleteDeviceInstance"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soDeleteDeviceInstance not found")
        devRes = false;
    }

    this->soGetMSPTypeNameVector = reinterpret_cast<const std::vector<std::string> & (*)()>(dlsym(this->soHandle, "soGetMSPTypeNameVector"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soGetMSPTypeNameVector not found")
        mspRes = false;
    }

    this->soGetMSPTypeVersion = reinterpret_cast<std::string (*)(const std::string &)>(dlsym(this->soHandle, "soGetMSPTypeVersion"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soGetMSPTypeVersion not found")
        mspRes = false;
    }

    this->soNewMSPInstance = reinterpret_cast<IMSP * (*)(const std::string &, const MSPConfig &)>(dlsym(this->soHandle, "soNewMSPInstance"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soNewMSPInstance not found")
        mspRes = false;
    }

    this->soDeleteMSPInstance = reinterpret_cast<void (*)(IMSP *)>(dlsym(this->soHandle, "soDeleteMSPInstance"));
    if (dlerror() != NULL) {
        //TFDEBUG(" soDeleteMSPInstance not found")
        mspRes = false;
    }
    
    this->open = genRes && (typeRes || mspRes || devRes);
    
    if (this->open) {
        this->soInitPlugin();
    }
        
    return this->open;
}


bool PluginLib::isOpen() const {
    return this->open;
}


bool PluginLib::closeSO() {
    bool res = false;
    if (this->soHandle) {
        res = dlclose(this->soHandle) == 0;
        this->soHandle = 0;
    }

    this->soGetSOVersion = 0;
    this->soGetTFVersion = 0;
    this->soTypeRegistration = 0;
    this->soDeviceRegistration = 0;
    this->soMSPRegistration = 0;
    this->soGetDeviceTypeNameVector = 0;
    this->soGetDeviceTypeVersion = 0;
    this->soNewDeviceInstance = 0;
    this->soDeleteDeviceInstance = 0;
    this->soGetMSPTypeNameVector = 0;
    this->soGetMSPTypeVersion = 0;
    this->soNewMSPInstance = 0;
    this->soDeleteMSPInstance = 0;

    this->open = false;

    return res;
}


void PluginLib::typeRegistration(IEventFactory * eventFactory, IEventChannelFactory * eventChannelFactory) {
    if ( ! soTypeRegistration) {
        return;
    }
    this->soTypeRegistration(eventFactory, eventChannelFactory);
}


void PluginLib::deviceRegistration(IDeviceFactory * deviceFactory) {
    if ( ! soDeviceRegistration) {
        return;
    }
    this->soDeviceRegistration(deviceFactory);    
}


void PluginLib::mspRegistration(IMSPFactory * mspFactory) {
    if ( ! soMSPRegistration) {
        return;
    }
    this->soMSPRegistration(mspFactory);      
}



bool PluginLib::readEntityNames() {
    if ( ! this->isOpen()) {
        return false;
    }

    this->deviceNameVector.clear();
    this->mspNameVector.clear();

    if (this->soGetDeviceTypeNameVector != 0) {
        const vector<string> & nameVector = this->soGetDeviceTypeNameVector();
        vector<string>::const_iterator i = nameVector.begin();
        vector<string>::const_iterator e = nameVector.end();
        while (i != e) {
            TFDEBUG("  device found: " << *i)
            this->deviceNameVector.push_back(*i);
            ++i;
        }
    }

    if (this->soGetMSPTypeNameVector != 0) {
        const vector<string> & nameVector = this->soGetMSPTypeNameVector();
        vector<string>::const_iterator i = nameVector.begin();
        vector<string>::const_iterator e = nameVector.end();
        while (i != e) {
            TFDEBUG("  msp found: " << *i)
            this->mspNameVector.push_back(*i);
            ++i;
        }
    }

    return (this->deviceNameVector.size() != 0) || (this->mspNameVector.size() != 0);
}


const std::string & PluginLib::getPath() const {
    return this->path;
}


const std::vector<std::string> & PluginLib::getDeviceTypeNameVector() const {
    return this->deviceNameVector;
}


const std::vector<std::string> & PluginLib::getMSPTypeNameVector() const {
    return this->mspNameVector;
}


IDevice * PluginLib::newDeviceInstance(const std::string & deviceTypeName, const DeviceConfig & deviceConfig) {
    if ( ! this->soNewDeviceInstance) {
        return 0;
    }
    return this->soNewDeviceInstance(deviceTypeName, deviceConfig);
}


IMSP * PluginLib::newMSPInstance(const std::string & mspTypeName, const MSPConfig & mspConfig) {
    if ( ! this->soNewMSPInstance) {
        return 0;
    }
    return this->soNewMSPInstance(mspTypeName, mspConfig);
}


void PluginLib::deleteDeviceInstance(IDevice * device) {
    if ( ! this->soDeleteDeviceInstance) {
        return;
    }
    return this->soDeleteDeviceInstance(device);
}


void PluginLib::deleteMSPInstance(IMSP * msp) {
    if ( ! this->soDeleteMSPInstance) {
        return;
    }
    return this->soDeleteMSPInstance(msp);
}

}
