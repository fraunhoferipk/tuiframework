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


#ifndef _tuiframework_TUIObjectDescriptor_h_
#define _tuiframework_TUIObjectDescriptor_h_

#include "TUIObjectInstance.h"
#include "TUIObjectType.h"
#include "ISerializable.h"

#include <iosfwd>

namespace tuiframework {

/**
 *  A Description of a TUI object.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class TUIObjectDescriptor : public ISerializable {
public:
    TUIObjectDescriptor();
    virtual ~TUIObjectDescriptor();

        /// Returns the TUI object instance description.
    const TUIObjectInstance & getTUIObjectInstance() const;
        /// Sets the TUI object instance description.
    void setTUIObjectInstance(const TUIObjectInstance & tuiObjectInstance);

        /// Returns the TUI object type description;
    const TUIObjectType & getTUIObjectType() const;
        /// Sets the TUI object type description;
    void setTUIObjectType(const TUIObjectType & tuiObjectType);

        /// Returns a mapping table, which consists of a channel name as key and channel number as value.
    const std::map<std::string, int> & getNameChannelNrMap() const;
        /// Returns a mapping table, which consists of a channel name as key and channel number as value.
    std::map<std::string, int> & getNameChannelNrMap();
        /// Sets a mapping table, which consists of a channel name as key and channel number as value.
    void setNameChannelNrMap(const std::map<std::string, int> & nameChannelNrMap);

    std::ostream & serialize(std::ostream & os) const;
    std::istream & deSerialize(std::istream & is);

protected:
        /// TUI object instance description
    TUIObjectInstance tuiObjectInstance;
        /// TUI object type description
    TUIObjectType tuiObjectType;
        /// A mapping table, which consists of a channel name as key and channel number as value.
    std::map<std::string, int> nameChannelNrMap;
};

}

#endif
