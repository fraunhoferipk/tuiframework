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


#ifndef _tuiframework_UDPSenderSocket_h_
#define _tuiframework_UDPSenderSocket_h_

#include "../core/HostAddress.h"

#include <pthread.h>
#include <set>

namespace tuiframework {

class IHostMsgSource;
class IThreadMessageSink;

/**
 *  Provides a User Datagram Protocol (UDP) sender engine.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class UDPSenderSocket {
public:
    UDPSenderSocket(IHostMsgSource & hostMsgSource);
    virtual ~UDPSenderSocket();

        /// Sets the port number of the socket.
        /// If myPort is set to 0 then a random port number is chosen.
    void setMyPort(unsigned short myPort);
        /// Sets a sink for thread dependend notification messages.
    void setThreadMessageSink(IThreadMessageSink * threadMessageSink);

        /// Starts the UDP sender engine.
    void create();
        /// Stops the UDP sender engine.
    void cancel();
    
    void join();
        /// UDP sender loop.
    void run();

protected:
        /// Clean up handler for sending a notification message about
        /// the cancellation of the UDP sender engine.
    static void notifyCancellation(void * arg);
        /// Clean up handler which closes the socket.
    static void closeSocket(void * arg);
        /// Clean up handler which notifies the serialized data source about
        /// the cancellation during waiting for data.
    static void waitForDataCanceled(void * arg);

protected:
        /// host message source
    IHostMsgSource & hostMsgSource;

        /// Port number of the socket
    unsigned short myPort;
        /// Socket file descriptor
    int sfd;
        /// Thread ID
    pthread_t tid;
        /// Sink for thread dependend notification messages.
    IThreadMessageSink * threadMessageSink;
};

}

#endif

