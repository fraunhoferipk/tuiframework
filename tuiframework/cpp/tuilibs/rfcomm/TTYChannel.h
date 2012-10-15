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


#ifndef TTYCHANNEL_H
#define TTYCHANNEL_H

#include <string>

#ifdef WIN32
#define TTYCHANNEL_DEFAULT_BAUD 9400
#else
#define TTYCHANNEL_DEFAULT_BAUD B9600
#include <termios.h>
#endif

class TTYChannel
{

public:
    bool open(const std::string & name, int baudrate = TTYCHANNEL_DEFAULT_BAUD, bool blocking = true);
    void flushInput(void);
    void flushOutput(void);
    void close(void);
    int readByte(void);
    int readBytes(char * buf, int maxlen);
    void readNumBytes(char * buf, int n);
    void writeByte(const char val);
    void writeBytes(const char * buf, int n);
    int availableBytes(void);

private:

#ifdef WIN32
    HANDLE hComm;
#else
    struct termios oldtio, newtio;
    int tty_fd;
#endif

};


#endif
