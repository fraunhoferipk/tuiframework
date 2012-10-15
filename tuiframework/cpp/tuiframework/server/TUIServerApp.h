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


#ifndef _tuiframework_TUIServerApp_h_
#define _tuiframework_TUIServerApp_h_

#include "../core/ITUIServer.h"

#include "../core/EventFactory.h"
#include "DeviceFactory.h"
#include "MSPFactory.h"
#include "../core/EventQueue.h"
#include "../core/SerializedDataQueue.h"
#include "../core/EventDeserializer.h"
#include "../core/EventSerializer.h"
#include "../core/EventMsgRouter.h"
#include "../core/EventTerminator.h"
#include "../core/HostMsgDispatcher.h"
#include "../core/HostMsgQueue.h"

#ifdef _WIN32
#include "../sockets/UDPReceiverWinSock2.h"
#include "../sockets/UDPSenderWinSock2.h"
#else
#include "../sockets/UDPReceiverSocket.h"
#include "../sockets/UDPSenderSocket.h"
#endif

#include "../core/TUIObjectType.h"
#include "../core/TUIObjectInstance.h"

#include "../core/AttachedObjects.h"
#include "../core/GUIDEventTypeIDVector.h"

#include "../server/ServerConfig.h"
#include "../server/DeviceContainer.h"
#include "../server/MSPContainer.h"

#include "../core/TypeRegistration.h"

#include <pthread.h>
#include <map>

namespace tuiframework {

class IDevice;
/**
 *  TUIServerApp
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class TUIServerApp : public ITUIServer {
public:
    TUIServerApp(int senderPortNr, int receiverPortNr, bool usingMulticast = false,
                 const HostAddress & multicastGroup = HostAddress("225.0.0.47:22547"));
    virtual ~TUIServerApp();

    void init(const ServerConfig & serverConfig);

        // ITUIServer Impl
    virtual void tuiServerRegisterDevice(int entityID, IDevice & device);
    virtual void tuiServerDeregisterDevice(int entityID);

    virtual void tuiServerRegisterEventSink(int entityID, IEventSink & eventSink);
    virtual void tuiServerDeregisterEventSink(int entityID);

    virtual void tuiServerExecute();
    virtual void tuiServerExit();

    void handleSystemCmdMsg(HostEvent * msg);
    
    IEventFactory & getEventFactory();
    IDeviceFactory & getDeviceFactory();
    IMSPFactory & getMSPFactory();

protected:

    void createEntityGraph(const std::map<std::string, TUIObjectInstance> & tuiObjectInstanceMap,
            const std::vector<Connector> & connectorVector);

    static void * inputLoopThread_run(void * arg);
    void executeInputLoop();

protected:
    EventFactory eventFactory;
    DeviceFactory deviceFactory;
    MSPFactory mspFactory;
    DeviceContainer deviceContainer;
    MSPContainer mspContainer;
    AttachedObjects attachedObjects;
    GUIDEventTypeIDVector guidEventTypeIDVector;

    std::map<int, IDevice *> deviceMap;
    std::map<int, IEventSink *> eventSinkMap;

    pthread_t inputThread;
    volatile bool inputLoopRunning;

    HostMsgQueue outHostMsgQueue;
    SerializedDataQueue inSerializedDataQueue;
    EventQueue inEventQueue;
    EventQueue outEventQueue;

#ifdef _WIN32
    UDPSenderWinSock2 udpSenderSocket;
    UDPReceiverWinSock2 udpReceiverSocket;
#else
    UDPSenderSocket udpSenderSocket;
    UDPReceiverSocket udpReceiverSocket;
#endif
    EventDeserializer eventDeserializer;
    HostMsgDispatcher outHostMsgDispatcher;
    EventSerializer eventSerializer;

    pthread_t outputSerializedDataThread;

    int senderPortNr;
    int receiverPortNr;

    EventMsgRouter<EPAddress, IEventSink, EPAddress::CompareByDeviceID> tuiRouter;
    std::map<int, EventMsgRouter<EPAddress, IEventSink, EPAddress::CompareByChannelNr> *> deviceChannelRouterMap;
    std::map<int, EventMsgRouter<EPAddress, IEventSink, EPAddress::CompareByChannelNr> *> tuiChannelRouterMap;
    EventTerminator eventTerminator;

    std::vector<IEventSink *> addressTranslatorVector;
    
    bool usingMulticast;
    HostAddress multicastGroup;
    
    EventQueue mcEventQueue;
    EventSerializer mcEventSerializer;
    HostMsgDispatcher mcHostMsgDispatcher;
};

}

#endif

