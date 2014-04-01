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


#include "EventSerializer.h"

#include "EventQueue.h"
#include "ISerializedDataSink.h"
#include "IThreadMessageSink.h"
#include "IEvent.h"

#include "../logging/Logger.h"

#include <sstream>
#include <string>
#include <cstring> // GCC 4.3 related build problem

using namespace std;

namespace tuiframework {

EventSerializer::EventSerializer(IEventSource & eventSource,
        ISerializedDataSink & serializedDataSink) :
        eventSource(eventSource),
        serializedDataSink(serializedDataSink),
        threadMessageSink(0) {

}


EventSerializer::~EventSerializer() {
}


void EventSerializer::setThreadMessageSink(IThreadMessageSink * threadMessageSink) {
    this->threadMessageSink = threadMessageSink;
}


static void * runfunc(void * arg) {
    TFDEBUG("EventSerializer thread started\n")
    EventSerializer * eventSerializer = static_cast<EventSerializer *>(arg);
    eventSerializer->run();
    return 0;
}


void EventSerializer::create() {
    int rc = pthread_create(&this->tid, NULL, runfunc, this);
    if (rc) {
        TFFATAL("ERROR in thread creation");
    }
}


void EventSerializer::cancel() {
    pthread_cancel(this->tid);
}


void EventSerializer::join() {
    pthread_join(this->tid, 0);
}


void EventSerializer::run() {
    pthread_cleanup_push(notifyCancellation, this);
    if (this->threadMessageSink) {
        this->threadMessageSink->threadCreated(this->tid);
    }

    pthread_cleanup_push(waitForDataCanceled, this);

    stringstream ss;
    while (true) {
        this->eventSource.waitForData();
        IEvent * event = this->eventSource.pop();
        if (event) {
            ss.str("");
                        
            ss << event->getEventTypeID() << " " << event;
            string str = ss.str();
            
            //cout << str << endl;
           
            char * buffer = new char[str.size() + 1];
            memcpy(buffer, str.c_str(), str.size() + 1);
            this->serializedDataSink.push(pair<char *, int>(buffer, str.size() + 1));
            
            delete event;
        }
    }
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}


void EventSerializer::waitForDataCanceled(void * arg) {
    EventSerializer * eventSerializer = static_cast<EventSerializer *>(arg);
    eventSerializer->eventSource.cleanupAfterWaitCanceled();
}


void EventSerializer::notifyCancellation(void * arg) {
    EventSerializer * eventSerializer = static_cast<EventSerializer *>(arg);
    if (eventSerializer->threadMessageSink) {
        eventSerializer->threadMessageSink->threadCanceled(eventSerializer->tid);
    }
}

}

