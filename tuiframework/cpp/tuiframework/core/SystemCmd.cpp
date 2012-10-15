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


#include "SystemCmd.h"

#include <iostream>

namespace tuiframework {

const char SystemCmd::requestConnection = 1;
const char SystemCmd::removeConnection = 2;

SystemCmd::SystemCmd() :
    cmd(0),
    portNr(0) {
}


SystemCmd::SystemCmd(int cmd, unsigned short portNr) :
    cmd(cmd),
    portNr(portNr) {
}


SystemCmd::~SystemCmd() {
}


void SystemCmd::setCmd(char cmd) {
    this->cmd = cmd;
}


char SystemCmd::getCmd() const {
    return this->cmd;
}


void SystemCmd::setPortNr(unsigned short portNr) {
    this->portNr = portNr;
}


unsigned short SystemCmd::getPortNr() const {
    return this->portNr;
}


std::ostream & SystemCmd::serialize(std::ostream & os) const {
    os << cmd << " " << portNr;
    return os;
}


std::istream & SystemCmd::deSerialize(std::istream & is) {
    is >> cmd >> portNr;
    return is;
}

}

