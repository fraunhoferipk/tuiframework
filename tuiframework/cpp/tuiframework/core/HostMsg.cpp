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


#include "HostMsg.h"

#include <string.h>

using namespace std;

namespace tuiframework {

HostMsg::HostMsg() {
	data.first = 0;
	data.second = 0;
}


HostMsg::HostMsg(const HostAddress & hostAddress, const char * data, int size) {
    this->setHostAddress(hostAddress);
    this->setData(data, size);
}


HostMsg::~HostMsg() {
    delete []this->data.first;
}


void HostMsg::setHostAddress(const HostAddress & hostAddress) {
    this->hostAddress = hostAddress;
}


void HostMsg::setData(const char * data, int size) {
    if (this->data.first != 0) {
        delete []this->data.first;
    }
    this->data.first = new char[size];
    memcpy(this->data.first, data, size);
    this->data.second = size;
}


const HostAddress & HostMsg::getHostAddress() const {
    return this->hostAddress;
}


pair<const char *, int> HostMsg::getData() const {
    return pair<const char *, int>(data.first, data.second);
}


HostMsg * HostMsg::clone() const {
    HostMsg * hostMsg = new HostMsg();
    hostMsg->setHostAddress(this->hostAddress);
    hostMsg->setData(this->data.first, this->data.second);
    return hostMsg;
}

}
