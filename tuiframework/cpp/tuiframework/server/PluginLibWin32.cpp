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

#include "PluginLibWin32.h"

#include "../core/Version.h"
#define USE_TFDEBUG
#include "../logging/Logger.h"

namespace tuiframework {

using namespace std;

PluginLib * PluginLib::loadPluginLib(const std::string & path) {
	TFDEBUG("Scanning: " << path)
    PluginLib * pluginLib = new PluginLib(path);
    if ( ! pluginLib->openDLL()) {
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
    if (pluginLib->isOpen()) {
        pluginLib->closeDLL();
    }
    delete pluginLib;
}


PluginLib::PluginLib(const string & path) :
    path(path),
    dllHandle(0),
    open(false),
    dllGetTFVersion(0),
    dllGetDLLVersion(0),
    dllTypeRegistration(0),
    dllDeviceRegistration(0),
    dllMSPRegistration(0),
    dllGetDeviceTypeNameVector(0),
    dllGetDeviceTypeVersion(0),
    dllNewDeviceInstance(0),
    dllDeleteDeviceInstance(0),
    dllGetMSPTypeNameVector(0),
    dllGetMSPTypeVersion(0),
    dllNewMSPInstance(0),
    dllDeleteMSPInstance(0) {
}


PluginLib::~PluginLib() {
    if (this->isOpen()) {
        this->closeDLL();
    }
}


bool PluginLib::openDLL() {
    if (this->dllHandle) {
        return false;
    }

    bool genRes = true;
    bool typeRes = true;
    bool devRes = true;
    bool mspRes = true;

    this->dllHandle = LoadLibrary(this->path.c_str());
    if ( ! this->dllHandle) {
        return false;
    }

	this->dllInitPlugin = reinterpret_cast<void (*)()>(GetProcAddress(this->dllHandle, "dllInitPlugin"));
    if ( ! this->dllInitPlugin) {
        //TFDEBUG(" dllInitPlugin not found")
        genRes = false;
    }

    this->dllGetDLLVersion = reinterpret_cast<std::string (*)()>(GetProcAddress(this->dllHandle, "dllGetDLLVersion"));
    if ( ! this->dllGetDLLVersion) {
		//TFDEBUG(" dllGetDLLVersion not found")
        genRes = false;
    }

	
    this->dllGetTFVersion = reinterpret_cast<std::string (*)()>(GetProcAddress(this->dllHandle, "dllGetTFVersion"));
    if ( ! this->dllGetTFVersion) {
		//TFDEBUG(" dllGetTFVersion not found")
        genRes = false;
    }
	
	if (this->dllGetTFVersion) {
        if (this->dllGetTFVersion().compare(TUIFRAMEWORK_VERSION)) {
            genRes = false;
            TFDEBUG(" dllGetTFVersion not equal!")
            TFDEBUG("   " << this->path.c_str() << " version: " << this->dllGetTFVersion());
            TFDEBUG("   tuiframework version: " << TUIFRAMEWORK_VERSION);
        }
    }

    this->dllTypeRegistration = reinterpret_cast<void (*)(IEventFactory * eventFactory, IEventChannelFactory * eventChannelFactory)>(GetProcAddress(this->dllHandle, "dllTypeRegistration"));
    if ( ! this->dllTypeRegistration) {
        //TFDEBUG(" dllTypeRegistration not found")
        typeRes = false;
	} else {
		TFDEBUG(" => dllTypeRegistration found")
	}
    
    this->dllDeviceRegistration = reinterpret_cast<void (*)(IDeviceFactory * deviceFactory)>(GetProcAddress(this->dllHandle, "dllDeviceRegistration"));
    if ( ! this->dllDeviceRegistration) {
        //TFDEBUG(" dllDeviceRegistration not found")
        devRes = false;
    }
    
    this->dllMSPRegistration = reinterpret_cast<void (*)(IMSPFactory * deviceFactory)>(GetProcAddress(this->dllHandle, "dllMSPRegistration"));
    if ( ! this->dllMSPRegistration) {
        //TFDEBUG(" dllMSPRegistration not found")
        mspRes = false;
    }

    this->dllGetDeviceTypeNameVector = reinterpret_cast<const std::vector<std::string> & (*)()>(GetProcAddress(this->dllHandle, "dllGetDeviceTypeNameVector"));
    if ( ! this->dllGetDeviceTypeNameVector) {
		//TFDEBUG(" dllGetDeviceTypeNameVector not found")
        devRes = false;
    }

    this->dllGetDeviceTypeVersion = reinterpret_cast<std::string (*)(const std::string &)>(GetProcAddress(this->dllHandle, "dllGetDeviceTypeVersion"));
    if ( ! this->dllGetDeviceTypeVersion) {
		//TFDEBUG(" dllGetDeviceTypeVersion not found")
        devRes = false;
    }

    this->dllNewDeviceInstance = reinterpret_cast<IDevice * (*)(const std::string &, const DeviceConfig &)>(GetProcAddress(this->dllHandle, "dllNewDeviceInstance"));
    if ( ! this->dllNewDeviceInstance) {
		//TFDEBUG(" dllNewDeviceInstance not found")
        devRes = false;
    }

    this->dllDeleteDeviceInstance = reinterpret_cast<void (*)(IDevice *)>(GetProcAddress(this->dllHandle, "dllDeleteDeviceInstance"));
    if ( ! this->dllDeleteDeviceInstance) {
		//TFDEBUG(" dllDeleteDeviceInstance not found")
        devRes = false;
    }

    this->dllGetMSPTypeNameVector = reinterpret_cast<const std::vector<std::string> & (*)()>(GetProcAddress(this->dllHandle, "dllGetMSPTypeNameVector"));
    if ( ! this->dllGetMSPTypeNameVector) {
		//TFDEBUG(" dllGetMSPTypeNameVector not found")
        mspRes = false;
    }

    this->dllGetMSPTypeVersion = reinterpret_cast<std::string (*)(const std::string &)>(GetProcAddress(this->dllHandle, "dllGetMSPTypeVersion"));
    if ( ! this->dllGetMSPTypeVersion) {
		//TFDEBUG(" dllGetMSPTypeVersion not found")
        mspRes = false;
    }

    this->dllNewMSPInstance = reinterpret_cast<IMSP * (*)(const std::string &, const MSPConfig &)>(GetProcAddress(this->dllHandle, "dllNewMSPInstance"));
    if ( ! this->dllNewMSPInstance) {
		//TFDEBUG(" dllNewMSPInstance not found")
        mspRes = false;
    }

    this->dllDeleteMSPInstance = reinterpret_cast<void (*)(IMSP *)>(GetProcAddress(this->dllHandle, "dllDeleteMSPInstance"));
    if ( ! this->dllDeleteMSPInstance) {
		//TFDEBUG(" dllDeleteMSPInstance not found")
        mspRes = false;
    }

    this->open = genRes && (typeRes || mspRes || devRes);
    
    if (this->open) {
        this->dllInitPlugin();
    }
        
    return this->open;
}


bool PluginLib::isOpen() const {
    return this->open;
}


bool PluginLib::closeDLL() {
    bool res = false;
    if (this->dllHandle) {
        res = FreeLibrary(this->dllHandle) == TRUE;
        this->dllHandle = 0;
    }

	this->dllGetDLLVersion = 0;
    this->dllGetTFVersion = 0;
    this->dllTypeRegistration = 0;
    this->dllDeviceRegistration = 0;
    this->dllMSPRegistration = 0;
    this->dllGetDeviceTypeNameVector = 0;
    this->dllGetDeviceTypeVersion = 0;
    this->dllNewDeviceInstance = 0;
    this->dllDeleteDeviceInstance = 0;
    this->dllGetMSPTypeNameVector = 0;
    this->dllGetMSPTypeVersion = 0;
    this->dllNewMSPInstance = 0;
    this->dllDeleteMSPInstance = 0;

    this->open = false;

    return res;
}



void PluginLib::typeRegistration(IEventFactory * eventFactory, IEventChannelFactory * eventChannelFactory) {
    if ( ! dllTypeRegistration) {
        return;
    }
    this->dllTypeRegistration(eventFactory, eventChannelFactory);
	//TFDEBUG("typeRegisteration called");
}


void PluginLib::deviceRegistration(IDeviceFactory * deviceFactory) {
    if ( ! dllDeviceRegistration) {
        return;
    }
    this->dllDeviceRegistration(deviceFactory);    
}


void PluginLib::mspRegistration(IMSPFactory * mspFactory) {
    if ( ! dllMSPRegistration) {
        return;
    }
    this->dllMSPRegistration(mspFactory);      
}


bool PluginLib::readEntityNames() {
    if ( ! this->isOpen()) {
        return false;
    }

    this->deviceNameVector.clear();
    this->mspNameVector.clear();

    if (this->dllGetDeviceTypeNameVector != 0) {
        const vector<string> & nameVector = this->dllGetDeviceTypeNameVector();
        vector<string>::const_iterator i = nameVector.begin();
        vector<string>::const_iterator e = nameVector.end();
        while (i != e) {
            this->deviceNameVector.push_back(*i);
			TFDEBUG("==> Device found: " << *i)
            ++i;
        }
    }

    if (this->dllGetMSPTypeNameVector != 0) {
        const vector<string> & nameVector = this->dllGetMSPTypeNameVector();
        vector<string>::const_iterator i = nameVector.begin();
        vector<string>::const_iterator e = nameVector.end();
        while (i != e) {
            this->mspNameVector.push_back(*i);
			TFDEBUG("==> MSP found: " << *i)
            ++i;
        }
    }

    return (this->deviceNameVector.size() != 0) || (this->mspNameVector.size() != 0);
}


const std::vector<std::string> & PluginLib::getDeviceTypeNameVector() const {
    return this->deviceNameVector;
}


const std::vector<std::string> & PluginLib::getMSPTypeNameVector() const {
    return this->mspNameVector;
}


IDevice * PluginLib::newDeviceInstance(const std::string & deviceTypeName, const DeviceConfig & deviceConfig) {
    if ( ! this->dllNewDeviceInstance) {
        return 0;
    }
    return this->dllNewDeviceInstance(deviceTypeName, deviceConfig);
}


IMSP * PluginLib::newMSPInstance(const std::string & mspTypeName, const MSPConfig & mspConfig) {
    if ( ! this->dllNewMSPInstance) {
        return 0;
    }
    return this->dllNewMSPInstance(mspTypeName, mspConfig);
}


void PluginLib::deleteDeviceInstance(IDevice * device) {
    if ( ! this->dllDeleteDeviceInstance) {
        return;
    }
    return this->dllDeleteDeviceInstance(device);
}


void PluginLib::deleteMSPInstance(IMSP * msp) {
    if ( ! this->dllDeleteMSPInstance) {
        return;
    }
    return this->dllDeleteMSPInstance(msp);
}

}
