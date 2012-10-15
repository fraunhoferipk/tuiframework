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


#include "HostMsgDispatcher.h"

#include "HostAddress.h"
#include "IHostMsgSink.h"
#include "HostMsg.h"

using namespace std;

namespace tuiframework {

HostMsgDispatcher::HostMsgDispatcher(IHostMsgSink & hostMsgSink) :
        hostMsgSink(hostMsgSink) {
    pthread_mutex_init(&this->mutex, NULL);
}


HostMsgDispatcher::~HostMsgDispatcher() {
}


void HostMsgDispatcher::addDstAddress(const HostAddress & address) {
    pthread_mutex_lock(&this->mutex);
    if (this->dstAddressSet.find(address) == this->dstAddressSet.end()) {
        this->dstAddressSet.insert(address);
    }
    pthread_mutex_unlock(&this->mutex);
}


void HostMsgDispatcher::removeDstAddress(const HostAddress & address) {
    pthread_mutex_lock(&this->mutex);
    this->dstAddressSet.erase(address);
    pthread_mutex_unlock(&this->mutex);
}


void HostMsgDispatcher::push(const std::pair<char *, int> & sd) {
    pthread_mutex_lock(&this->mutex);
    
    set<HostAddress>::iterator i = this->dstAddressSet.begin();
    set<HostAddress>::iterator e = this->dstAddressSet.end();
    while (i != e) {
        HostMsg * msg = new HostMsg(*i, sd.first, sd.second);
        this->hostMsgSink.push(msg);
        ++i;
    }
    
    delete []sd.first;

    pthread_mutex_unlock(&this->mutex);
}





}
