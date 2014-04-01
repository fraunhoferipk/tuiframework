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


#ifndef _tuiframework_UDPReceiverSocket_h_
#define _tuiframework_UDPReceiverSocket_h_

#include <pthread.h>

namespace tuiframework {

class ISerializedDataSink;
class IThreadMessageSink;
class IHostAddressSink;
/**
 *  Provides a User Datagram Protocol (UDP) receiver engine.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class UDPReceiverSocket {
public:
    UDPReceiverSocket(ISerializedDataSink & serializedDataSink);
    virtual ~UDPReceiverSocket();

        /// Sets the port number of the socket
    void setMyPort(unsigned short int myPort);
        /// Gets the port number of the socket
    unsigned short int getMyPort();
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
    
    void join();
        /// UDP receiver loop.
    void run();
    
    void setHostEventPrefix(bool hostEventPrefix);

protected:
        /// Clean up handler for sending a notification message about
        /// the cancellation of the UDP receiver engine.
    static void notifyCancellation(void * arg);
        /// Clean up handler which closes the socket.
    static void closeSocket(void * arg);

protected:
        /// Socket file descriptor
	int sfd;
        /// Serialized data sink
    ISerializedDataSink & serializedDataSink;
        /// Port number of the socket
    unsigned short int myPort;
        /// Thread ID
    pthread_t tid;
        /// Sink for thread dependend notification messages.
    IThreadMessageSink * threadMessageSink;
        /// Sink for host addresses.
    IHostAddressSink * hostAddressSink;
    
    bool hostEventPrefix;
};

}

#endif
