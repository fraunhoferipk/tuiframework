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



#ifdef WIN32
#include "windows.h"
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <fstream>
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#endif

#include <stdio.h>
#include <memory.h>
#include <iostream>

#ifndef WIN32
#include <tuiframework/core/Exception.h>
using namespace tuiframework;
#endif

#include "TTYChannel.h"

using namespace std;

bool TTYChannel::open(const std::string & name, int baudrate, bool blocking)
{

#ifdef WIN32

    cerr << "opening com port...";

    DWORD flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING;
    if (!blocking) flags |= FILE_FLAG_OVERLAPPED;	// ???? stimmt das �berhaupt ????

    hComm = CreateFile(name.c_str(),  
                    GENERIC_READ | GENERIC_WRITE, 
                    0, 
                    0, 
                    OPEN_EXISTING,
                    flags,
                    NULL);
    if (hComm == INVALID_HANDLE_VALUE)
    {
        // error opening port; abort
        cerr << "could not open comm port!\n";
        return false;
    }

    cerr << "ok\n";

    cerr << "setting dcb...";

    DCB dcb;
    memset(&dcb,0,sizeof(dcb));
    dcb.DCBlength=sizeof(dcb);
    
    char fromatString[256];
    sprintf(fromatString, "baud=%d parity=N data=8 stop=1", baudrate);

    if (!BuildCommDCB(fromatString, &dcb))
    {
        cerr << "could not build Comm Port config: " << GetLastError() << "\n";
        close();
        return false;
    }

    if (!SetCommState(hComm, &dcb))
    {
        cerr << "could not set Comm Port config" << GetLastError() << "\n";
        close();
        return false;
    }

    cerr << "ok\n";

#else	
    unsigned int flags = O_RDWR | O_NOCTTY;
    if (!blocking) {
        flags |= O_NONBLOCK;
    }
    this->tty_fd = ::open(name.c_str(), flags);

    if (this->tty_fd < 0) {
        throw Exception("could not open \"" + name + "\"", __FILE__, __LINE__);
    }
        // save current port settings
    tcgetattr(this->tty_fd, &this->oldtio);

    memset(&this->newtio, 0, sizeof(this->newtio));
    this->newtio.c_cflag = baudrate | CRTSCTS | CS8 | CLOCAL | CREAD;
    this->newtio.c_iflag = IGNPAR;
    this->newtio.c_oflag = 0;

        // set input mode (non-canonical, no echo,...)
    this->newtio.c_lflag = 0;
        // inter-character timer unused
    this->newtio.c_cc[VTIME] = 0;
        // blocking read until 1 chars received
    this->newtio.c_cc[VMIN] = 1;

    tcflush(this->tty_fd, TCIFLUSH);
    tcsetattr(this->tty_fd, TCSANOW, &this->newtio);
#endif

    return true;
}


void TTYChannel::flushInput() {
#ifdef WIN32
#else
    tcflush(this->tty_fd, TCIFLUSH);
#endif
}


void TTYChannel::flushOutput() {
#ifdef WIN32
#else
    tcflush(this->tty_fd, TCOFLUSH);
#endif

}


void TTYChannel::close() {
#ifdef WIN32
    if (!CloseHandle(hComm))
    {
        cerr << "could not close comm port handle" << GetLastError() << "\n";
    }
#else
    // restore previous port settings
        if (tty_fd > 0) 
        {
            tcsetattr(this->tty_fd, TCSANOW, &this->oldtio);
        ::close(this->tty_fd);
    }
#endif

}

// reads single byte
int TTYChannel::readByte() {
    unsigned char val;
#ifdef WIN32
    DWORD count;
    ReadFile(hComm, &val, 1, &count, NULL);
    if (count <= 0) return -1;
#else
    if (read(this->tty_fd, &val, 1) == -1) {
        throw Exception("read(...) failed.", __FILE__, __LINE__);
    }
#endif

    return val;
}

// reads avalable (up to maxlen) bytes from stream, returns number of read bytes
int TTYChannel::readBytes(char * buf, int maxlen) {

#ifdef WIN32
    DWORD count;
    ReadFile(hComm, buf, maxlen, &count, NULL);
#else
    int count = read(this->tty_fd, buf, maxlen);
    if (count == -1) {
        throw Exception("read(...) failed.", __FILE__, __LINE__);
    }
#endif

    return count;
}


    // reads given number of bytes, blocks until all bytes read
void TTYChannel::readNumBytes(char * buf, int n) {
    
    int i = 0;
    do {
        
#ifdef WIN32
        DWORD count;
        ReadFile(hComm, buf, n, &count, NULL);
#else
        int count = read(this->tty_fd, &buf[i], n - i);
        if (count == -1) {
            throw Exception("read(...) failed.", __FILE__, __LINE__);
        }
#endif
        i += count;
    } while (i < n);
}


void TTYChannel::writeByte(char val) {
#ifdef WIN32
    DWORD count;
    WriteFile(hComm, &val, 1, &count, NULL);
#else
    if (write(this->tty_fd, &val, 1) == -1) {
        throw Exception("write(...) failed", __FILE__, __LINE__);
    }
#endif
}

void TTYChannel::writeBytes(const char * buf, int n) {
#ifdef WIN32
    DWORD count;
    WriteFile(hComm, buf, n, &count, NULL);
#else
    if (write(this->tty_fd, buf, n) == -1) {
        throw Exception("write(...) failed", __FILE__, __LINE__);
    }
#endif
}


int TTYChannel::availableBytes(void)
{
#ifdef WIN32
    struct _COMSTAT status;
    unsigned long   etat;

    if (hComm!=INVALID_HANDLE_VALUE)
    {
        ClearCommError(hComm, &etat, &status);
        return (int)status.cbInQue;
    }
    else return 0;
#else
    int fd;
    int bytes;

    ioctl(this->tty_fd, FIONREAD, &bytes);
    return bytes;
#endif
}
