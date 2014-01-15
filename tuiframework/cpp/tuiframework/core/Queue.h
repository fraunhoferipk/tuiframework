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


#ifndef _tuiframework_Queue_h_
#define _tuiframework_Queue_h_

#include "IEventSink.h"

#include <pthread.h>

#include <queue>
#include <iostream>

namespace tuiframework {

class IEvent;

/**
 *  A synchronized FIFO queue.
 *
 *  Queue is designed for multithreaded environments and
 *  offers a conditional wait for data.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template <class T>
class Queue {
public:
        /// Constructs a new Queue.
        /// The delivered initial value is returned by pop()
        /// for the case no data is available
    Queue<T>(T initialValue = T()) :
        initialValue(initialValue) {
        pthread_mutex_init(&this->mutex, NULL);
        pthread_cond_init(&this->condition, NULL);
    }

    virtual ~Queue<T>() {
        pthread_cond_destroy(&this->condition);
        pthread_mutex_destroy(&this->mutex);
    }

        /// This method blocks until data is available by using a conditional wait
        /// and thus is a cancellation point.
    void waitForData() {
        pthread_mutex_lock(&this->mutex);
        if ( ! this->q.size()) {
            pthread_cond_wait(&this->condition, &this->mutex);
        }
        pthread_mutex_unlock(&this->mutex);
    }

        /// If the running thread is requested to cancel.
        /// cleanupAfterWaitCanceled should be called to unlock the mutex
        /// which is set after the conditional wait is canceled.
    void cleanupAfterWaitCanceled() {
        pthread_cond_signal(&this->condition);
        pthread_mutex_unlock(&this->mutex);
        /*
        pthread_cond_destroy(&this->condition);
        pthread_mutex_destroy(&this->mutex);
        pthread_mutex_init(&this->mutex, NULL);
        pthread_cond_init(&this->condition, NULL);
        */
    }

        /// Data of type T is pushed in the Queue.
        /// One thread which waits for data is unlocked and can collect the data.
    void push(const T & t) {
        pthread_mutex_lock(&this->mutex);
        this->q.push(t);
        pthread_mutex_unlock(&this->mutex);
        pthread_cond_signal(&this->condition);
        // pthread_cond_broadcast(&this->condition);
        
    }

        /// Data of type T is popped.
        /// If Queue is empty the initial value of T, which was delivered
        /// to the Constructor is returned.
    T pop() {
        pthread_mutex_lock(&this->mutex);
        T  t(initialValue);
        if (this->q.size()) {
            t = this->q.front();
            this->q.pop();
        }
        pthread_mutex_unlock(&this->mutex);
        return t;
    }

        /// Returns the number of elements stored in the Queue
    int size() const {
        pthread_mutex_lock(const_cast<pthread_mutex_t *>(&this->mutex));
        int s = static_cast<int>(this->q.size()); 
        pthread_mutex_unlock(const_cast<pthread_mutex_t *>(&this->mutex));
        return s;
    }

protected:
        /// Initial value
    T initialValue;
        /// Queue uses a std:queue to store the data
    std::queue<T> q;
        /// Mutex for synchronizing
    pthread_mutex_t mutex;
        /// Conditional variable
    pthread_cond_t condition;
};

}

#endif

