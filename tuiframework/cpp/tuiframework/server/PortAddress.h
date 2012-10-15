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


#ifndef _tuiframework_PortAddress_h_
#define _tuiframework_PortAddress_h_

#include "../core/ISerializable.h"

#include <string>
#include <iosfwd>

namespace tuiframework {

/**
 *  PortAddress
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class PortAddress : public ISerializable {
public:
    enum OwnerType {
        DEVICE = 1,
        TUIOBJECT = 2,
        MSP = 3
    };

    PortAddress();
    PortAddress(OwnerType type, const std::string & name, const std::string & channelName);
    virtual ~PortAddress();

    OwnerType getOwnerType() const;
    void setOwnerType(OwnerType ownerType);

    const std::string & getName() const;
    void setName(const std::string & name);

    const std::string & getPortName() const;
    void setPortName(const std::string & portName);

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
        /// Type of the owner.
    int type;
        /// Name of the owner.
    std::string name;
        /// Name of the port.
    std::string portName;
};

}



#endif
