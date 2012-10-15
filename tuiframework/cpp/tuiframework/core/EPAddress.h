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


#ifndef _tuiframework_EPAddress_
#define _tuiframework_EPAddress_

#include "ISerializable.h"

namespace tuiframework {


/**
 *  Specifies an entity port address.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class EPAddress : public ISerializable {
public:
        /// Contains an operator which does a comparison between two EPAddress by the device ID.
    struct CompareByDeviceID {
        bool operator()(const EPAddress & devChAddr1, const EPAddress & devChAddr2) const {
            return devChAddr1.getEntityID() < devChAddr2.getEntityID();
        }
    };

        /// Contains an operator which does a comparison between two EPAddress by the channel number.
    struct CompareByChannelNr {
        bool operator()(const EPAddress & devChAddr1, const EPAddress & devChAddr2) const {
            return devChAddr1.getPortNr() < devChAddr2.getPortNr();
        }
    };

public:
    EPAddress(int entityID = -1, int portNr = -1);
    ~EPAddress();

    int getAddressTypeID() const;
    static int AddressTypeID();

    int getEntityID() const;
    int getPortNr() const;

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
    int entityID;
    int portNr;
};

}

#endif

