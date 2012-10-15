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


#ifndef _tuiframework_TUIObjectType_h_
#define _tuiframework_TUIObjectType_h_

#include "Port.h"
#include "ISerializable.h"

#include <string>
#include <map>

#include <iosfwd>

namespace tuiframework {

/**
 *  A Description of a TUI object type.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class TUIObjectType : public ISerializable {
public:
    TUIObjectType();
    virtual ~TUIObjectType();

        /// Returns the TUI object type name.
    const std::string & getName() const;
        /// Sets the TUI object type name.
    void setName(const std::string & name);

        /// Sets the additional description.
    void setDescription(const std::string & description);
        /// Returns the additional description.
    const std::string & getDescription();

        /// Returns a mapping table, which consists of a port name as key and a channel type as value.
    std::map<std::string, Port> & getPortMap();
        /// Returns a mapping table, which consists of a port name as key and a channel type as value.
    const std::map<std::string , Port> & getPortMap() const;
        /// Sets a mapping table, which consists of a port name as key and a channel type as value.
    void setPortMap(const std::map<std::string, Port> & portMap);

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
        /// Name of the TUI object type
    std::string name;
        /// Additional description
    std::string description;
        /// A mapping table, which consists of a port name as key and a port type as value.
    std::map<std::string, Port> portMap;
};

}

#endif


