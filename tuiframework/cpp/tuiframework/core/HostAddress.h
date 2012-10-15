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


#ifndef _tuiframework_HostAddress_h_
#define _tuiframework_HostAddress_h_

#include "ISerializable.h"
#include <iosfwd>
#include <string>

namespace tuiframework {

/**
 *  Provides an Internet Protocol (IP) address and a port number.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class HostAddress : public ISerializable {
public:
    HostAddress();

        /// Constructs a host address with the delivered string.
        /// This constructor calls fromString(str).
    HostAddress(const std::string & str);
        /// Constructs a host address with the delivered ip address
        /// and port number
    HostAddress(unsigned int ipAddress, unsigned short portNr);
    virtual ~HostAddress();

        /// Sets the host address by the delivered string.
        /// The string must have the format n.n.n.n:p, where n
        /// has the range 0 - 255 and p has the range from 0 to 65535
    void fromString(const std::string & str);
        /// Returns a string representation the host address.
    std::string toString() const;

        /// Sets the IP address.
    void setIPAddress(unsigned int ipAddress);
        /// Returns the IP address.
    unsigned int getIPAddress() const;

        /// Sets the port number.
    void setPortNr(unsigned short portNr);
        /// Returns the port number.
    unsigned short getPortNr() const;

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

        /// Returns the address type ID.
    int getAddressTypeID() const;
        /// Returns the address type ID.
    static int AddressTypeID();

protected:
        /// IP address
    unsigned int ipAddress;
        /// Port number
    unsigned short portNr;
};

    /// Provides the less operator of two host addresses..
bool operator < (const HostAddress & a, const HostAddress & b);
    /// Provides the equal operator of two host addresses.
bool operator == (const HostAddress & a, const HostAddress & b);

}

#endif

