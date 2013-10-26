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

#ifndef _tuidevices_MotionCaptureStubDev_h_
#define _tuidevices_MotionCaptureStubDev_h_

#include <tuiframework/core/IDevice.h>
#include <tuiframework/core/SerializedDataQueue.h>
#ifdef _WIN32
#include <tuiframework/sockets/UDPReceiverWinSock2.h>
#else
#include <tuiframework/sockets/UDPReceiverSocket.h>
#endif
#include <tuiframework/server/DeviceDescriptor.h>

#include <pthread.h>

#include <map>
#include <cstdio>

namespace tuiframework {
    class ITUIServer;
}

namespace tuidevices {

/**
 *  MotionCaptureStubDev
 *
 *  \author Oliver Belaifa
 *  \date 2013
 */    
class MotionCaptureStubDev : public tuiframework::IDevice {
public:
    static IDevice * createFunction(void * arg);
    static std::string getDeviceName();

    MotionCaptureStubDev(const tuiframework::DeviceConfig & deviceConfig);
    virtual ~MotionCaptureStubDev();

        // IDevice
    virtual void deviceConnect(tuiframework::ITUIServer & tuiServer);
    virtual void deviceDisconnect();
    virtual bool deviceExecute();
    virtual void deviceStop();
    virtual void deviceSetEventSink(tuiframework::IEventSink * eventSink);
    virtual const tuiframework::DeviceDescriptor & getDeviceDescriptor() const;

protected:
    static void * inputLoopThread_run(void * arg);
    void executeInputLoop();

protected:
    tuiframework::DeviceDescriptor deviceDescriptor;

    int entityID;
    tuiframework::ITUIServer * tuiServer;
    tuiframework::IEventSink * eventSink;

    volatile bool inputLoopRunning;
    pthread_t inputLoopThread;

    int portNr;

    tuiframework::SerializedDataQueue queue;
#ifdef _WIN32
    tuiframework::UDPReceiverWinSock2 receiver;
#else
    tuiframework::UDPReceiverSocket receiver;
#endif
};

}

#endif
