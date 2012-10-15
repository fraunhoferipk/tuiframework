/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    designed for creating applications with a tangible user interface (TUI).
    
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

#include <tuiframework/client/client.h>
#include <tuitypes/common/CommonTypeReg.h>

#include <iostream>
#include <stdlib.h>
#include <cstdio>

using namespace tuiframework;
using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage tuiclient1 <sender port> <receiver port> <ipaddress:port>" << endl;
        return 1;
    }

    Init init;
    initTypeRegistration(getEventFactory());
    CommonTypeReg::registerTypes(&getEventFactory(), &getEventChannelFactory());

	getchar();

    connectWithTUIServer(atoi(argv[1]), atoi(argv[2]), argv[3], &init);
    return 0;
}
