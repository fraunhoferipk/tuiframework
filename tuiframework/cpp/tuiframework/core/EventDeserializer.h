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


#ifndef _tuiframework_EventDeserializer_h_
#define _tuiframework_EventDeserializer_h_

#include <pthread.h>

namespace tuiframework {

class IEventSink;
class ISerializedDataSource;
class IEventFactory;
class IThreadMessageSink;

/**
 *  Provides an event deserialization engine.
 *  Deserializes events from a serialized data source and
 *  pushes them to an event sink.
 *  Creates an own thread.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class EventDeserializer {
public:
    EventDeserializer(ISerializedDataSource & serializedDataSource, IEventSink & eventSink, IEventFactory & eventFactory);
    virtual ~EventDeserializer();

        /// Sets a sink for thread dependend notification messages.
    void setThreadMessageSink(IThreadMessageSink * threadMessageSink);

        /// Starts the event deserialization engine.
    void create();
        /// Cancels the event deserialization engine.
    void cancel();
        /// Event deserialization loop.
    void run();

protected:
        /// Clean up handler which notifies the serialized data source about
        /// the cancellation during waiting for data.
    static void waitForDataCanceled(void * arg);
        /// Clean up handler for sending a notification message about
        /// the cancellation of the deserialization engine.
    static void notifyCancellation(void * arg);

protected:
    ISerializedDataSource & serializedDataSource;
    IEventSink & eventSink;
    IEventFactory & eventFactory;
        /// Thread ID
    pthread_t tid;
        /// Sink for thread dependend notification messages.
    IThreadMessageSink * threadMessageSink;
};

}

#endif
