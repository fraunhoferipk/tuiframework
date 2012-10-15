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


#ifndef _tuiframework_TUIEventHandler_h_
#define _tuiframework_TUIEventHandler_h_

#include "../core/IEventChannel.h"
#include "../core/IEventSink.h"
#include "../core/EventDelegate.h"
#include "../core/Exception.h"

#include <map>
#include <string>
#include <list>

namespace tuiframework {

/**
 *  Provides a connection management of source event channel to destination method pointer.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class TUIEventHandler {
protected:
    /**
     *  Base class of FunctorMap which is needed
     *  for the usage in containers.
     *
     *  \author Oliver Belaifa
     *  \date 2010-2012
     */
    class IFunctorMap {
    public:
        virtual ~IFunctorMap() { }
    };

    /**
     *  Manages the functor signatures.
     *
     *  \author Oliver Belaifa
     *  \date 2010-2012
     */
    template <class E, class R>
    class FunctorMap : public IFunctorMap {
    protected:
            /// Method pointer
        typedef void (R:: * MethodPointer)(const E *);

            /// A list of method pointer and event sink pairs.
            // must use stl list, because method pointer doesn't have a < operator
        typedef std::list<std::pair<MethodPointer, IEventSink *> > MethodPointerEventSinkList;

            /// A mapping table, which consists of an instance pointer as key and
            /// a list of method pointer and event sink pairs as value.
        typedef std::map<R *, MethodPointerEventSinkList> FunctorMapType;

    public:
        FunctorMap() {
        }

        virtual ~FunctorMap() {
        }

            /// Returns the iterator which has the method pointer mp.
        typename MethodPointerEventSinkList::iterator find(MethodPointerEventSinkList & l1, MethodPointer mp) {
            typename MethodPointerEventSinkList::iterator i = l1.begin();
            while (i != l1.end() && (*i).first != mp) {
                ++i;
            }
            return i;
        }

            /// Adds the method pointer and stores its signature and returns
            /// the created event delegate.
        IEventSink * add(R * receiver, void (R:: * method)(const E *)) {
            IEventSink * eventDelegate = 0;
                // Is a receiver entry already available ?
            typename FunctorMapType::iterator iter2 = this->functorEventSinkMap.find(receiver);
            if (iter2 == this->functorEventSinkMap.end()) {
                    // If not, then create an entry.
                iter2 = this->functorEventSinkMap.insert(typename FunctorMapType::value_type(receiver, MethodPointerEventSinkList())).first;
            }
                // Create an event delegate and store it in the
                // list of method pointer and event sink pairs within the receiver entry.
            typename MethodPointerEventSinkList::iterator iter1 = find((*iter2).second, method);
            if (iter1 == (*iter2).second.end()) {
                eventDelegate = new EventDelegate<E, R>(receiver, method);
                (*iter2).second.push_back(std::pair<MethodPointer, IEventSink *>(method, eventDelegate));
            }

            return eventDelegate;
        }

            /// Removes the method pointer with the corresponding signature and
            /// returns its event delegate.
        IEventSink * remove(R * receiver, void (R:: * method)(const E *)) {
            IEventSink * eventDelegate = 0;
                // Find the receiver.
            typename FunctorMapType::iterator iter2 = this->functorEventSinkMap.find(receiver);
            if (iter2 != this->functorEventSinkMap.end()) {
                    // Find the iterator which has the method pointer.
                typename MethodPointerEventSinkList::iterator iter1 = find((*iter2).second, method);
                if (iter1 != (*iter2).second.end()) {
                    eventDelegate = (*iter1).second;
                    // Remove the iterator from the list of method pointer and event sink pairs.
                    (*iter2).second.erase(iter1);
                        // Does the receiver map have any entries?
                    if ((*iter2).second.size() == 0) {
                            // No, remove it.
                        this->functorEventSinkMap.erase(iter2);
                    }
                }
            }
            return eventDelegate;
        }

    protected:
            /// A mapping table, which consists of an instance pointer as key and
            /// a list of method pointer and event sink pairs as value.
        FunctorMapType functorEventSinkMap;
    };

        /// A mapping table, which consists of an event channel as key and
        /// a functor map as value.
    typedef std::map<IEventChannel *, IFunctorMap *> EventChannelFunctorMap;

public:
    TUIEventHandler();
    virtual ~TUIEventHandler();

        /// Connects the source event channel with a functor.
    template <class E, class R>
    void channelConnect(IEventChannel * source, R * receiver, void (R:: * method)(const E *)) {

        if (source == 0) {
            throw Exception("channelConnect failed (delivered IEventChannel is 0).", __FILE__, __LINE__);
        }

        if (source->getChannelTypeID() != E::EventTypeID()) {
            throw Exception("channelConnect failed (the type of the delivered IEventChannel differs from the EventTypeID", __FILE__, __LINE__);
        }

        EventChannelFunctorMap::iterator iter3 = this->eventChannelFunctorMap.find(source);
        if (iter3 == this->eventChannelFunctorMap.end()) {
            iter3 = this->eventChannelFunctorMap.insert(EventChannelFunctorMap::value_type(source, new FunctorMap<E, R>())).first;
        }
        FunctorMap<E, R> * tuiFunctorMap = static_cast<FunctorMap<E, R> *>((*iter3).second);
            // Connect and store the signature
        IEventSink * eventSink = tuiFunctorMap->add(receiver, method);
        if (eventSink) {
            source->addEventSink(eventSink);
        }
    }

        /// Disconnects the source event channel from a functor.
    template <class E, class R>
    void channelDisconnect(IEventChannel * source, R * receiver, void (R:: * method)(const E *)) {
        EventChannelFunctorMap::iterator iter3 = this->eventChannelFunctorMap.find(source);
        if (iter3 != this->eventChannelFunctorMap.end()) {
                // Find the connection by the signature and remove it
            FunctorMap<E, R> * tuiFunctorMap = static_cast<FunctorMap<E, R> *>((*iter3).second);
            IEventSink * eventSink = tuiFunctorMap->remove(receiver, method);
            if (eventSink) {
                source->removeEventSink(eventSink);
                delete eventSink;
            } else {
                throw Exception("channelDisconnect failed (IEventSink not found with the delivered Signature).", __FILE__, __LINE__);
            }
        } else {
                throw Exception("channel Disconnect failed (IEventChannel not found with the delivered signature).", __FILE__, __LINE__);
        }
    }
protected:
        /// A mapping table, which consists of an event channel as key and
        /// a functor map as value.
    EventChannelFunctorMap eventChannelFunctorMap;
};

}

#endif

