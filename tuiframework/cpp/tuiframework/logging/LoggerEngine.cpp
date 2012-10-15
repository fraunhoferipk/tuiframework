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


#include "LoggerEngine.h"

#include "../core/ISerializedDataSource.h"
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

#define DEFAULT_PORT    9163

using namespace std;

namespace tuiframework {

LoggerEngine::LoggerEngine(ISerializedDataSource & serializedDataSource) :
    serializedDataSource(serializedDataSource),
    threadMessageSink(0) {
}


LoggerEngine::~LoggerEngine() {
}


void LoggerEngine::setThreadMessageSink(IThreadMessageSink * threadMessageSink) {
    this->threadMessageSink = threadMessageSink;
}


void LoggerEngine::run() {
    // The default cancelability state of pthread's is deferred.

    pthread_cleanup_push(notifyCancellation, this);
    if (this->threadMessageSink) {
        this->threadMessageSink->threadCreated(this->tid);
    }

    pthread_cleanup_push(waitForDataCanceled, this);

    while (true) {
        this->serializedDataSource.waitForData();
        pair<char *, int> outgoingSerializedData = this->serializedDataSource.pop();
        if (outgoingSerializedData.first != 0) {

            // LOGGING
            cout << outgoingSerializedData.first << endl;

            delete outgoingSerializedData.first;
        }
    }

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}


static void * runfunc(void * arg) {
    printf("LoggerEngine thread started\n");
    LoggerEngine * engine = static_cast<LoggerEngine *>(arg);
    engine->run();
    return 0;
}


void LoggerEngine::create() {
    int rc = pthread_create(&this->tid, NULL, runfunc, this);
    if (rc) {
        TFERROR("ERROR in thread creation");
    }
}


void LoggerEngine::cancel() {
    pthread_cancel(this->tid);
}

// Static methods for clean up

void LoggerEngine::notifyCancellation(void * arg) {
    LoggerEngine * engine = static_cast<LoggerEngine *>(arg);
    if (engine->threadMessageSink) {
        engine->threadMessageSink->threadCanceled(engine->tid);
    }
}


void LoggerEngine::closeSocket(void * arg) {
    LoggerEngine * udpSenderSocket = static_cast<LoggerEngine *>(arg);
    close(engine->sfd);
}


void LoggerEngine::waitForDataCanceled(void * arg) {
    LoggerEngine * engine = static_cast<LoggerEngine *>(arg);
    engine->serializedDataSource.cleanupAfterWaitCanceled();
}


}



