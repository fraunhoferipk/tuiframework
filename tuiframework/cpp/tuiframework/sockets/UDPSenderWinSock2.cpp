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


#include "UDPSenderWinSock2.h"

#define USE_TFDEBUG
#include "../logging/Logger.h"

#include "../core/IHostMsgSource.h"
#include "../core/HostMsg.h"
#include "../core/Exception.h"
#include "../core/IThreadMessageSink.h"

#include <map>
#include <vector>

#include <iostream>

#include <cstring> // GCC 4.3 related build problem

using namespace std;

namespace tuiframework {

UDPSenderWinSock2::UDPSenderWinSock2(IHostMsgSource & hostMsgSource) :
    hostMsgSource(hostMsgSource),
    myPort(0),
    threadMessageSink(0),
    connectSocket(INVALID_SOCKET) {
}


UDPSenderWinSock2::~UDPSenderWinSock2() {
}



void UDPSenderWinSock2::setMyPort(unsigned short myPort) {
    this->myPort = myPort;
}


void UDPSenderWinSock2::setThreadMessageSink(IThreadMessageSink * threadMessageSink) {
	this->threadMessageSink = threadMessageSink;
}


static void send(SOCKET connectSocket, vector<HostMsg *> & msgs, int size) {
	HostMsg * hostMsg = *msgs.begin();

	SOCKADDR_IN dst_sin;
	memset(&dst_sin, 0, sizeof(dst_sin));
    dst_sin.sin_family = AF_INET;
    dst_sin.sin_port = htons(hostMsg->getHostAddress().getPortNr());
    dst_sin.sin_addr.s_addr = htonl(hostMsg->getHostAddress().getIPAddress());

	int curIndex = 0;
	size += 4; // place for the frameCount

	char * data = new char[size];
	u_long frameCount = static_cast<u_long>(msgs.size());
	frameCount = htonl(frameCount);
	memcpy(&data[curIndex], &frameCount, 4);
	curIndex += 4;

	vector<HostMsg *>::iterator i = msgs.begin();
	vector<HostMsg *>::iterator e = msgs.end();
	while (i != e) {
		u_long frameSize = static_cast<u_long>((*i)->getData().second);
		frameSize = htonl(frameSize);
		memcpy(&data[curIndex], &frameSize, 4);
		curIndex += 4;

		memcpy(&data[curIndex], (*i)->getData().first, (*i)->getData().second);
		curIndex += (*i)->getData().second;
		delete (*i);
		++i;
	}
	sendto(connectSocket, data, size, 0, (struct sockaddr *)&dst_sin, sizeof(dst_sin));
}


void UDPSenderWinSock2::run() {
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

    if (this->myPort) {
        SOCKADDR_IN my_sin;
        memset(&my_sin, 0, sizeof(my_sin));
        my_sin.sin_family = AF_INET;
        my_sin.sin_port = htons(this->myPort);
        my_sin.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(this->connectSocket, (struct sockaddr *)&my_sin, sizeof(my_sin)) == -1) {
            TFFATAL("bind(sfd, (struct sockaddr *)&my_sin, sizeof(my_sin)) failed");
            pthread_exit(0);
        }
    }

    SOCKADDR_IN dst_sin;
    pthread_cleanup_push(waitForDataCanceled, this);
	/*
	/// {
	map<pair<int, int>, vector<HostMsg *> > hostMsgMap;
	HostMsg * hostMsg;
	while (true) {
		this->hostMsgSource.waitForData();
		HostMsg * hostMsg;
		do {
			hostMsg = this->hostMsgSource.pop();
			if (hostMsg != 0) {
				pair<int, int> haddr(hostMsg->getHostAddress().getIPAddress(), hostMsg->getHostAddress().getPortNr());
				string str = hostMsg->getHostAddress().toString();
				map<pair<int, int>, vector<HostMsg *> >::iterator iter = hostMsgMap.find(haddr);
				if (iter == hostMsgMap.end()) {
					iter = hostMsgMap.insert(map<pair<int, int>, vector<HostMsg *> >::value_type(haddr, vector<HostMsg *>())).first;
				}
				(*iter).second.push_back(hostMsg);
			}
		}
		while (hostMsg != 0);

		if (hostMsgMap.size()) {
			vector<HostMsg *> v;
			int size = 0; // initial DWORD (number of pakets)
			map<pair<int, int>, vector<HostMsg *> >::iterator i = hostMsgMap.begin();
			map<pair<int, int>, vector<HostMsg *> >::iterator e = hostMsgMap.end();
			while (i != e) {
				vector<HostMsg *>::iterator i2 = (*i).second.begin();
				vector<HostMsg *>::iterator e2 = (*i).second.end();
				while (i2 != e2) {
					if (size != 4 && size + (*i2)->getData().second + 4 > 4096 - 4) {
						send(this->connectSocket, v, size);
						v.clear(); // hostmsg already deleted
						size = 0;
					} else {
						v.push_back(*i2);
						size += (*i2)->getData().second + 4; // data size + paket size
					}
					++i2;
				}
				send(this->connectSocket, v, size);
				v.clear(); // hostmsg already deleted
				size = 0;
				++i;
			}

			hostMsgMap.clear();
		}
	}
	/// }
	*/

    while (true) {
        this->hostMsgSource.waitForData();
        HostMsg * hostMsg = this->hostMsgSource.pop();
        if (hostMsg != 0) {

            memset(&dst_sin, 0, sizeof(dst_sin));
            dst_sin.sin_family = AF_INET;
            dst_sin.sin_port = htons(hostMsg->getHostAddress().getPortNr());
            dst_sin.sin_addr.s_addr = htonl(hostMsg->getHostAddress().getIPAddress());
            sendto(this->connectSocket, hostMsg->getData().first, hostMsg->getData().second, 0, (struct sockaddr *)&dst_sin, sizeof(dst_sin));
            //TFDEBUG(hostMsg->getHostAddress() << "  " << hostMsg->getData().first);

            delete hostMsg;
        }
    }
   
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}


void UDPSenderWinSock2::cleanup() {
	pthread_join(this->tid, 0);
	this->hostMsgSource.cleanupAfterWaitCanceled();
	closesocket(this->connectSocket);
	WSACleanup();
}


static void * runfunc(void * arg) {
	printf("UDPSenderWinSock2 thread started\n");
	UDPSenderWinSock2 * udpSenderSocket = static_cast<UDPSenderWinSock2 *>(arg);
	udpSenderSocket->run();
	return 0;
}


void UDPSenderWinSock2::create() {
	int rc = pthread_create(&this->tid, NULL, runfunc, this);
	if (rc) {
		TFERROR("ERROR in thread creation");
	}
}


void UDPSenderWinSock2::cancel() {
	pthread_cancel(this->tid);
}

// Static methods for clean up

void UDPSenderWinSock2::notifyCancellation(void * arg) {
	UDPSenderWinSock2 * udpSenderSocket = static_cast<UDPSenderWinSock2 *>(arg);
	if (udpSenderSocket->threadMessageSink) {
		udpSenderSocket->threadMessageSink->threadCanceled(udpSenderSocket->tid);
	}
}


void UDPSenderWinSock2::closeSocket(void * arg) {
    UDPSenderWinSock2 * udpSenderSocket = static_cast<UDPSenderWinSock2 *>(arg);
    closesocket(udpSenderSocket->connectSocket);
    WSACleanup();
}


void UDPSenderWinSock2::waitForDataCanceled(void * arg) {
	UDPSenderWinSock2 * udpSenderSocket = static_cast<UDPSenderWinSock2 *>(arg);
	udpSenderSocket->hostMsgSource.cleanupAfterWaitCanceled();
}

}



