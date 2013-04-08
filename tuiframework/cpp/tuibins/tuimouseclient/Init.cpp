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


#include "Init.h"

#include <iostream>

using namespace tuiframework;
using namespace std;

Init::Init() {
}


Init::~Init() {
}


void Init::push(IEvent * event) {
    if (event->getEventTypeID() == SystemMsg::EventTypeID()) {
        SystemMsg * msg = static_cast<SystemMsg *>(event);
        if (msg->getPayload() == CONNECTION_ESTABLISHED) {
            cout << "CONNECTION ESTABLISHED" << endl;
			this->tuiMouse.connect();
        }
    }
}
