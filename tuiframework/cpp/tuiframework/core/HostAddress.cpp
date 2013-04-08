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


#include "HostAddress.h"
#include "Exception.h"

#include <istream>
#include <ostream>
#include <sstream>

#include <algorithm> // GCC 4.3 related build problem

using namespace std;

namespace tuiframework {

HostAddress::HostAddress() :
	ipAddress(0),
	portNr(0) {
}


HostAddress::HostAddress(const std::string & str) {
    this->fromString(str);
}


HostAddress::HostAddress(unsigned int ipAddress, unsigned short portNr) :
    ipAddress(ipAddress),
    portNr(portNr) {
}


HostAddress::~HostAddress() {
}


int HostAddress::getAddressTypeID() const {
    return HostAddress::AddressTypeID();
}


int HostAddress::AddressTypeID() {
    return 2;
}



void HostAddress::fromString(const string & str) {
    // remove leading and trailling spaces
    string str2 = str;
    replace(str2.begin(), str2.end(), '.', ' ');
    replace(str2.begin(), str2.end(), ':', ' ');
    size_t x1 = str2.find_first_not_of(" ");
    size_t x2 = str2.find_last_not_of(" ");

    if (x1 == string::npos || x2 == string::npos) {
        throw Exception("HostAddress parse error3");
    }

    str2 = str2.substr(x1, x2 - x1 + 1);

    stringstream ss;
    ss.str(str2);

    int i = 0;
    unsigned int n;
    this->ipAddress = 0;
    do {
        this->ipAddress <<= 8;
        ss >> n;
        // error checking
        if (n > 255 || ss.fail() || (i < 3 && ss.eof())) {
            throw Exception("HostAddress parse error");
        }
        this->ipAddress |= n;
        i++;
    } while (i < 4);

    ss >> this->portNr;

    if (!ss.eof())
    {
        throw Exception("HostAddress parse error2");
    }
}


string HostAddress::toString() const {

    stringstream ss;
    for (int i = 0; i < 4; ++i) {
        ss << ((this->ipAddress >> (24 - 8*i)) & 255);
        if (i < 3) {
            ss << ".";
        }
    }
    ss << ":" << this->portNr;
    return ss.str();
}


void HostAddress::setIPAddress(unsigned int ipAddress) {
    this->ipAddress = ipAddress;
}


unsigned int HostAddress::getIPAddress() const {
    return this->ipAddress;
}


void HostAddress::setPortNr(unsigned short portNr) {
    this->portNr = portNr;
}


unsigned short HostAddress::getPortNr() const {
    return this->portNr;
}


std::ostream & HostAddress::serialize(std::ostream & os) const {
    os << this->toString();
    return os;
}


std::istream & HostAddress::deSerialize(std::istream & is) {
    string str;
    is >> str;
    this->fromString(str);
    return is;
}


bool operator < (const HostAddress & a, const HostAddress & b) {
    if (a.getIPAddress() < b.getIPAddress()) {
        return true;
    }

    if (a.getIPAddress() > b.getIPAddress()) {
        return false;
    }

    if (a.getPortNr() < b.getPortNr()) {
        return true;
    }

    return false;
}


bool operator == (const HostAddress & a, const HostAddress & b) {
    return a.getIPAddress() == b.getIPAddress() && a.getPortNr() == b.getPortNr();
}


} // namespace tuiframework

