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


#ifndef _tuiframework_MulticastListener_h_
#define _tuiframework_MulticastListener_h_

#include <pthread.h>

namespace tuiframework {

class ISerializedDataSink;
class IThreadMessageSink;
class IHostAddressSink;
/**
 *  Provides a multicast listener
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class MulticastListener {
public:
    MulticastListener(ISerializedDataSink & serializedDataSink);
    virtual ~MulticastListener();
    
        /// Sets the IP address of the multicast group.
    void setMCGroupIPAddress(unsigned int mcGroupIPAddress);
        /// Returns the IP address of the multicast group.
    unsigned int getMCGroupIPAddress() const;
        /// Sets the port number of the socket
    void setPort(unsigned short int port);
        /// Gets the port number of the socket
    unsigned short int getPort();
    
        /// Sets a sink for thread dependend notification messages.
    void setThreadMessageSink(IThreadMessageSink * threadMessageSink);
        /// Sets a sink for host addresses. Before pushing received data
        /// the host address of the sender will be pushed in the
        /// host address sink.
    void setHostAddressSink(IHostAddressSink * hostAddressSink);
        /// Starts the UDP receiver engine.
    void create();
        /// Stops the UDP receiver engine.
    void cancel();
        /// UDP receiver loop.
    void run();

protected:
        /// Clean up handler for sending a notification message about
        /// the cancellation of the UDP receiver engine.
    static void notifyCancellation(void * arg);
        /// Clean up handler which closes the socket.
    static void closeSocket(void * arg);
        /// Clean up handler which drops the multicast group membership.
    void dropMembership(void * arg);

protected:
        /// Socket file descriptor
    int sfd;
        /// Serialized data sink
    ISerializedDataSink & serializedDataSink;
        /// IP address of the multicast group
    unsigned int mcGroupIPAddress;
        /// Port number of the socket
    unsigned short int port;
        /// Thread ID
    pthread_t tid;
        /// Sink for thread dependend notification messages.
    IThreadMessageSink * threadMessageSink;
        /// Sink for host addresses.
    IHostAddressSink * hostAddressSink;
};

}

#endif
