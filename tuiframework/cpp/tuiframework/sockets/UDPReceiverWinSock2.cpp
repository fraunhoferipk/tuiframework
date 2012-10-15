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


#include "UDPReceiverWinSock2.h"

#include "../core/ISerializedDataSink.h"


#include "../core/IThreadMessageSink.h"

#include "../core/HostAddress.h"
#include "../core/TypeRegistration.h"

#define USE_TFDEBUG
#include "../logging/Logger.h"

#include <iostream>
#include <sstream>

#include <cstring> // GCC 4.3 related build problem

#define DEFAULT_PORT    9163

using namespace std;

namespace tuiframework {

UDPReceiverWinSock2::UDPReceiverWinSock2(ISerializedDataSink & serializedDataSink) :
		serializedDataSink(serializedDataSink),
		myPort(DEFAULT_PORT),
		threadMessageSink(0) {
}


UDPReceiverWinSock2::~UDPReceiverWinSock2() {
}


void UDPReceiverWinSock2::setMyPort(unsigned short int myPort) {
    this->myPort = myPort;
}

unsigned short int UDPReceiverWinSock2::getMyPort() {
    return this->myPort;
}

void UDPReceiverWinSock2::setThreadMessageSink(IThreadMessageSink * threadMessageSink) {
	this->threadMessageSink = threadMessageSink;
}


void UDPReceiverWinSock2::setHostAddressSink(IHostAddressSink * hostAddressSink) {
    this->hostAddressSink = hostAddressSink;
}

void UDPReceiverWinSock2::run() {
	// The default cancelability state of pthread's is deferred.

	pthread_cleanup_push(notifyCancellation, this);
	if (this->threadMessageSink) {
		this->threadMessageSink->threadCreated(this->tid);
	}

    WSAData wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        TFFATAL("WSAStartup failed:" << res);
    }

    this->connectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->connectSocket == INVALID_SOCKET) {
        TFFATAL("socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) failed");
        WSACleanup();
    	pthread_exit(0);
    }

    pthread_cleanup_push(closeSocket, this);


    SOCKADDR_IN my_sin;
    memset(&my_sin, 0, sizeof(my_sin));
    my_sin.sin_family = AF_INET;
    my_sin.sin_port = htons(this->myPort);
    my_sin.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(this->connectSocket, (struct sockaddr *)&my_sin, sizeof(my_sin)) == -1) {
        TFFATAL("bind(sfd, (struct sockaddr *)&my_sin, sizeof(my_sin)) failed");
        pthread_exit(0);
    }

    while (true) {
        char buffer[4096];
        struct sockaddr_in other_sin;
        int other_sin_size = (int)sizeof(other_sin);

        int receivedByteCount = recvfrom(this->connectSocket, (char *)buffer, 4096, 0, (struct sockaddr *)&other_sin, &other_sin_size);

        HostAddress ipAddress(ntohl(other_sin.sin_addr.s_addr), ntohs(other_sin.sin_port));

            //@@TODO: split in a seperate class
        stringstream ss;
        ss << HostEvent::EventTypeID() << " ";
        HostEvent::serializeHeader(ipAddress, true, ss);
        ss << " " << buffer;

        string str = ss.str();
        TFDEBUG(str);

        char * incomingSerializedData = new char[str.size() + 1];
        memcpy(incomingSerializedData, str.c_str(), str.size() + 1);

        this->serializedDataSink.push(pair<char *, int>(incomingSerializedData, receivedByteCount));
    }

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}


static void * runfunc(void * arg) {
	printf("UDPReceiverWinSock2 thread started\n");
	UDPReceiverWinSock2 * udpReceiverSocket = static_cast<UDPReceiverWinSock2 *>(arg);
	udpReceiverSocket->run();
	return 0;
}


void UDPReceiverWinSock2::create() {
	int rc = pthread_create(&this->tid, NULL, runfunc, this);
	if (rc) {
		TFFATAL("ERROR in thread creation");
	}
}


void UDPReceiverWinSock2::cancel() {
	pthread_cancel(this->tid);
}


void UDPReceiverWinSock2::notifyCancellation(void * arg) {
	UDPReceiverWinSock2 * udpReceiverSocket = static_cast<UDPReceiverWinSock2 *>(arg);
	if (udpReceiverSocket->threadMessageSink) {
		udpReceiverSocket->threadMessageSink->threadCanceled(udpReceiverSocket->tid);
	}
}


void UDPReceiverWinSock2::closeSocket(void * arg) {
	UDPReceiverWinSock2 * udpReceiverSocket = static_cast<UDPReceiverWinSock2 *>(arg);
    closesocket(udpReceiverSocket->connectSocket);
}

}
