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


#include "EventDeserializer.h"

#include "IEventSink.h"
#include "IEvent.h"
#include "ISerializedDataSource.h"
#include "IThreadMessageSink.h"

#include "EventFactory.h"
#include "HostAddress.h"

#include "../logging/Logger.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cstring> // GCC 4.3 related build problem

using namespace std;

namespace tuiframework {

EventDeserializer::EventDeserializer(ISerializedDataSource & serializedDataSource,
        IEventSink & eventSink,
        IEventFactory & eventFactory) :
        serializedDataSource(serializedDataSource),
        eventSink(eventSink),
        eventFactory(eventFactory),
        threadMessageSink(0) {
}


EventDeserializer::~EventDeserializer() {
}


void EventDeserializer::setThreadMessageSink(IThreadMessageSink * threadMessageSink) {
    this->threadMessageSink = threadMessageSink;
}


static void * runfunc(void * arg) {
    TFDEBUG("EventDeserializer thread started\n")
    EventDeserializer * eventSerializer = static_cast<EventDeserializer *>(arg);
    eventSerializer->run();
    return 0;
}


void EventDeserializer::create() {
    int rc = pthread_create(&this->tid, NULL, runfunc, this);
    if (rc) {
        TFFATAL("ERROR in thread creation")
    }
}


void EventDeserializer::cancel() {
    pthread_cancel(this->tid);
}


void EventDeserializer::run() {
    pthread_cleanup_push(notifyCancellation, this);
    if (this->threadMessageSink) {
        this->threadMessageSink->threadCreated(this->tid);
    }

    pthread_cleanup_push(waitForDataCanceled, this);

    stringstream ss;
    while (true) {
       this->serializedDataSource.waitForData();
       pair<char *, int> serializedData = this->serializedDataSource.pop();
       if (serializedData.first != 0) {
           ss.clear();
           ss.str(serializedData.first);
           delete serializedData.first;

           int eventType;
           ss >> eventType;
           //TFDEBUG("EventType = " << eventType)

           IEvent * event = eventFactory.createInstance(eventType, &eventFactory);

           if (event) {
               ss >> event;
             //  cout << event << endl;
               this->eventSink.push(event);
           }
       }
    }

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}


void EventDeserializer::waitForDataCanceled(void * arg) {
    TFDEBUG("")
    EventDeserializer * eventSerializer = static_cast<EventDeserializer *>(arg);
    eventSerializer->serializedDataSource.cleanupAfterWaitCanceled();
}


void EventDeserializer::notifyCancellation(void * arg) {
    EventDeserializer * eventSerializer = static_cast<EventDeserializer *>(arg);
    if (eventSerializer->threadMessageSink) {
        eventSerializer->threadMessageSink->threadCanceled(eventSerializer->tid);
    }
}

}

