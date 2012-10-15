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


#ifndef _tuiframework_SerializedDataQueue_h_
#define _tuiframework_SerializedDataQueue_h_

#include "ISerializedDataSink.h"
#include "ISerializedDataSource.h"

#include "Queue.h"

#include <utility>//std::pair

namespace tuiframework {

class IEvent;

/**
 *  A synchronized FIFO queue for serialized data.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class SerializedDataQueue : public Queue<std::pair<char *, int > >, public ISerializedDataSink, public ISerializedDataSource {
public:
    SerializedDataQueue() :
        Queue<std::pair<char *, int> >(std::pair<char *, int>((char*)0, 0)) {
    }

    virtual ~SerializedDataQueue() {
    }

    virtual void push(const std::pair<char *, int > & sd) {
        Queue<std::pair<char *, int > >::push(sd);
    }


    virtual void waitForData() {
        Queue<std::pair<char *, int > >::waitForData();
    }


    virtual void cleanupAfterWaitCanceled() {
    	Queue<std::pair<char *, int > >::cleanupAfterWaitCanceled();
    }


    virtual std::pair<char *, int> pop() {
        return Queue<std::pair<char *, int > >::pop();
    }
};

}

#endif

