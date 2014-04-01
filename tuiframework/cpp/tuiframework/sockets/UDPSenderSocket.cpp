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


#include "UDPSenderSocket.h"

#include "../core/IHostMsgSource.h"
#include "../core/HostMsg.h"
#include "../core/Exception.h"
#include "../logging/Logger.h"
#include "../core/IThreadMessageSink.h"

#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <cstring> // GCC 4.3 related build problem


using namespace std;

namespace tuiframework {

UDPSenderSocket::UDPSenderSocket(IHostMsgSource & hostMsgSource) :
    hostMsgSource(hostMsgSource),
    myPort(0),
    threadMessageSink(0) {
}


UDPSenderSocket::~UDPSenderSocket() {
}



void UDPSenderSocket::setMyPort(unsigned short myPort) {
    this->myPort = myPort;
}


void UDPSenderSocket::setThreadMessageSink(IThreadMessageSink * threadMessageSink) {
    this->threadMessageSink = threadMessageSink;
}


void UDPSenderSocket::run() {
    // The default cancelability state of pthread's is deferred.

    pthread_cleanup_push(notifyCancellation, this);
    if (this->threadMessageSink) {
        this->threadMessageSink->threadCreated(this->tid);
    }

    this->sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sfd == -1) {
        TFFATAL("socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) failed");
        pthread_exit(0);
    }

    pthread_cleanup_push(closeSocket, this);

    const int one = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0) {
        TFFATAL("setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) failed");
        pthread_exit(0);
    }

    if (this->myPort) {
        struct sockaddr_in my_sin;
        memset(&my_sin, 0, sizeof(my_sin));
        my_sin.sin_family = AF_INET;
        my_sin.sin_port = htons(this->myPort);
        my_sin.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(sfd, (struct sockaddr *)&my_sin, sizeof(my_sin)) == -1) {
            TFFATAL("bind(sfd, (struct sockaddr *)&my_sin, sizeof(my_sin)) failed");
            pthread_exit(0);
        }
    }

    struct sockaddr_in dst_sin;
    pthread_cleanup_push(waitForDataCanceled, this);

    while (true) {
        this->hostMsgSource.waitForData();
        HostMsg * hostMsg = this->hostMsgSource.pop();
        if (hostMsg != 0) {

            memset(&dst_sin, 0, sizeof(dst_sin));
            dst_sin.sin_family = AF_INET;
            dst_sin.sin_port = htons(hostMsg->getHostAddress().getPortNr());
            dst_sin.sin_addr.s_addr = htonl(hostMsg->getHostAddress().getIPAddress());
            sendto(sfd, hostMsg->getData().first, hostMsg->getData().second, 0, (struct sockaddr *)&dst_sin, sizeof(dst_sin));

            //TFDEBUG(hostMsg->getHostAddress().toString() << " " << hostMsg->getData().first);
            //std::cout << "sender socket: " << hostMsg->getHostAddress().toString() << std::endl;

            delete hostMsg;
        }
    }

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}


static void * runfunc(void * arg) {
    printf("UDPSenderSocket thread started\n");
    UDPSenderSocket * udpSenderSocket = static_cast<UDPSenderSocket *>(arg);
    udpSenderSocket->run();
    return 0;
}


void UDPSenderSocket::create() {
    int rc = pthread_create(&this->tid, NULL, runfunc, this);
    if (rc) {
        TFERROR("ERROR in thread creation");
    }
}


void UDPSenderSocket::cancel() {
    pthread_cancel(this->tid);
}


void UDPSenderSocket::join() {
    pthread_join(this->tid, 0);
}

// Static methods for clean up

void UDPSenderSocket::notifyCancellation(void * arg) {
    UDPSenderSocket * udpSenderSocket = static_cast<UDPSenderSocket *>(arg);
    if (udpSenderSocket->threadMessageSink) {
        udpSenderSocket->threadMessageSink->threadCanceled(udpSenderSocket->tid);
    }
}


void UDPSenderSocket::closeSocket(void * arg) {
    UDPSenderSocket * udpSenderSocket = static_cast<UDPSenderSocket *>(arg);
    close(udpSenderSocket->sfd);
}


void UDPSenderSocket::waitForDataCanceled(void * arg) {
    UDPSenderSocket * udpSenderSocket = static_cast<UDPSenderSocket *>(arg);
    udpSenderSocket->hostMsgSource.cleanupAfterWaitCanceled();
}

}



