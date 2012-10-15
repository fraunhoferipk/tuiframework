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



#ifndef _tuidevices_PhidgetsDev_h_
#define _tuidevices_PhidgetsDev_h_

#include <tuiframework/core/EventQueue.h>
#include <tuiframework/core/IDevice.h>
#include <tuiframework/core/IEventSink.h>
#include <tuiframework/server/DeviceDescriptor.h>

#include <phidget21.h>

#include <string>

#ifndef _WIN32
#define __stdcall
#define __cdecl
#endif

namespace tuiframework {
    class ITUIServer;
}

namespace tuidevices {

class PhidgetsDev : public tuiframework::IDevice, public tuiframework::IEventSink {
public:
    static IDevice * __cdecl createFunction(void * arg);
    static std::string __cdecl getDeviceName();

    PhidgetsDev(const tuiframework::DeviceConfig & deviceConfig);
    virtual ~PhidgetsDev();

    tuiframework::IEventSink * getEventSink() const;
    int getEntityID() const;

        // IDevice
    virtual void deviceConnect(tuiframework::ITUIServer & tuiServer);
    virtual void deviceDisconnect();
    virtual bool deviceExecute();
    virtual void deviceStop();
    virtual void deviceSetEventSink(tuiframework::IEventSink * eventSink);
    virtual const tuiframework::DeviceDescriptor & getDeviceDescriptor() const;

        // IEventSink
    virtual void push(tuiframework::IEvent * event);
    
	int getAOutOffset() const;
    int getDOutOffset() const;
    int getDInOffset() const;

protected:
    void displayProperties();

protected:
    tuiframework::DeviceDescriptor deviceDescriptor;
    int entityID;

    tuiframework::ITUIServer * tuiServer;

    tuiframework::IEventSink * eventSink;
    CPhidgetInterfaceKitHandle ifKit;
    
    int aoutOffset;
    int doutOffset;
    int dinOffset;
};

}

#endif
