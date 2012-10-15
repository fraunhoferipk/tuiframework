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

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#endif

#ifndef _WIN32
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
 
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}

#endif


using namespace tuiframework;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Usage tuiclient2 <my sender port> <my receiver port> <1. tuiserver ipaddress:port> <2. tuiserver ipaddress:port>" << endl;
        return 1;
    }

    Init init;
    initTypeRegistration(getEventFactory(1));
    initTypeRegistration(getEventFactory(2));
    CommonTypeReg::registerTypes(&getEventFactory(1), &getEventChannelFactory(1));
    CommonTypeReg::registerTypes(&getEventFactory(2), &getEventChannelFactory(2));

    getchar();

    connectWithTUIServer(atoi(argv[1]), atoi(argv[2]), argv[3], &init, false, 1);
    connectWithTUIServer(atoi(argv[1]) + 10, atoi(argv[2]) + 10, argv[4], &init, false, 2);
    
    while ( ! kbhit()) {
        TUIClientAppProvider::getInstance(1)->processEvents();
        TUIClientAppProvider::getInstance(2)->processEvents();

#ifdef _WIN32
		Sleep(10);
#else
        usleep(100);
#endif
    }
    return 0;
}
