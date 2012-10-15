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

#ifndef _tuiframework_TUIClientApp_h_
#define _tuiframework_TUIClientApp_h_

#include "TUIObjectStubContainer.h"

#include "../core/EventFactory.h"
#include "../core/EventChannelFactory.h"
#include "../core/EventQueue.h"
#include "../core/SerializedDataQueue.h"
#include "../core/EventSerializer.h"
#include "../core/EventDeserializer.h"
#include "../core/IEventSink.h"
#include "../core/HostMsgDispatcher.h"
#include "../core/HostMsgQueue.h"

#ifdef _WIN32
#include "../sockets/UDPSenderWinSock2.h"
#include "../sockets/UDPReceiverWinSock2.h"
#include "../sockets/MulticastListenerWinSock2.h"
#else
#include "../sockets/UDPSenderSocket.h"
#include "../sockets/UDPReceiverSocket.h"
#include "../sockets/MulticastListener.h"
#endif

#include "../core/TypeRegistration.h"
#include "../core/HostAddress.h"

#include <pthread.h>
#include <map>

namespace tuiframework {

class TUIObjectStub;

class IChannelEvent;
class INotificationSink;
class IChannel;
 /**
 *  This class implements a TUI Client Application.
 *  
 *  \author Oliver Belaifa
 *  \date 2010-2012
 *  
 */
class TUIClientApp : public IEventSink {
public:
    TUIClientApp();
    virtual ~TUIClientApp();

        /// connects a TUI Server by ip address and port number
        /// @param mySendPortNr port number used by the sender socket
        /// @param myReceivePortNr port number used by the receiver socket
        /// @param serverAddress ipaddress and port number of the TUI server
        /// @param systemNotificationSink event sink used for notification (e.g. connection established event)
        /// @param ownership true: tuiframework takes the ownership of eventprocessing, falls the caller keeps the ownership
    bool connectWithTUIServer(
            int mySendPortNr,
            int myReceivePortNr,
            const std::string & serverAddress,
            IEventSink * systemNotificationSink,
            bool ownership = true);

        /// disconnects the server connection
    bool disconnectFromTUIServer();
        /// returns the tui object descriptions which are defined by the tui server
    const AttachedObjects & getAttachedObjects() const;
        /// returns the container of tui object stubs
    TUIObjectStubContainer & getTUIObjectStubContainer();
        /// processes events which are in the event queue. This method should be called frequently
        /// if the caller kept the ownership
    void processEvents();
        /// returns the event factory of the client
    IEventFactory & getEventFactory();
        /// returns the event channel factory of the client
    IEventChannelFactory & getEventChannelFactory();

protected:
        /// initializes / executes all modules / threads
    void execute();
        /// frees / exits all modules / threads
    void exit();
        /// creates the tui client thread
    void create();
        /// cancels the tui client thread
    void cancel();
        /// processes the event loop
    void processOutputLoop();
        /// method for handly the attached objects event message
    void handleAttachedObjectsMsg(AttachedObjectsMsg * event);
        /// handles the multicast group invitation message sent by the server
    void handleMulticastGroupInvitationMsg(MulticastGroupInvitationMsg * event);
        /// implementation of the event sink push method, the entry of all
        /// events sent to the client
    virtual void push(IEvent * event);

protected:
        /// executes the output loop
    void executeOutputLoop();

protected:
    EventFactory eventFactory;
    EventChannelFactory eventChannelFactory;
    
    AttachedObjects attachedObjects;
    TUIObjectStubContainer stubContainer;

    pthread_t inputLoopThread;
    volatile bool inputLoopRunning;

    HostMsgQueue outHostMsgQueue;
    SerializedDataQueue inSerializedDataQueue;
    EventQueue outEventQueue;
    EventQueue inEventQueue;

#ifdef _WIN32
    UDPSenderWinSock2 udpSenderSocket;
    UDPReceiverWinSock2 udpReceiverSocket;
    MulticastListenerWinSock2 multicastListener;
#else
    UDPSenderSocket udpSenderSocket;
    UDPReceiverSocket udpReceiverSocket;
    MulticastListener multicastListener;
#endif

    HostMsgDispatcher outHostMsgDispatcher;
    EventSerializer eventSerializer;
    EventDeserializer eventDeserializer;

    pthread_t outputLoopThread;
    volatile bool outputLoopRunning;

    IEventSink * systemNotificationSink;
    bool connectedWithServer;
    bool created;
    
    bool multicastListenerActivated;

    int myReceivePortNr;
};

}

#endif

