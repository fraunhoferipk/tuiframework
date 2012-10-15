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


#include "ServerStartupConfig.h"

#include <iostream>

namespace tuiframework {

ServerStartupConfig::ServerStartupConfig() : portNr(0) {
}


ServerStartupConfig::~ServerStartupConfig() {
}


int ServerStartupConfig::getPortNr() const {
    return this->portNr;
}


void ServerStartupConfig::setPortNr(int portNr) {
    this->portNr = portNr;
}


std::ostream & ServerStartupConfig::serialize(std::ostream & os) const {
    os << this->portNr;
    return os;
}


std::istream & ServerStartupConfig::deSerialize(std::istream & is) {
    is >> this->portNr;
    return is;
}

}
