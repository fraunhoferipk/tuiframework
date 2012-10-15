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


#ifndef _tuiframework_TUIObjectInstance_h_
#define _tuiframework_TUIObjectInstance_h_

#include "ISerializable.h"
#include <string>
#include <map>

namespace tuiframework {

/**
 *  A Description of a TUI object instance.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class TUIObjectInstance : public ISerializable {
public:
    TUIObjectInstance();
    virtual ~TUIObjectInstance();

        /// Sets the name of the TUI object instance.
    void setName(const std::string & name);
        /// Returns the name of the TUI object instance.
    const std::string & getName() const;

        /// Sets the ID of the TUI object instance.
    void setID(int id);
        /// REturns the ID of the TUI object instance.
    int getID() const;

        /// Sets the TUI object type name.
    void setTypeName(const std::string & typeName);
        /// Returns the TUI object type name.
    const std::string & getTypeName() const;

        /// Sets the additional description.
    void setDescription(const std::string & description);
        /// Returns the additional description.
    const std::string & getDescription();

        /// Returns a mapping table, which consists of a channel name as key and channel number as value.
    const std::map<std::string, int> & getNameChannelNrMap() const;
        /// Returns a mapping table, which consists of a channel name as key and channel number as value.
    std::map<std::string, int> & getNameChannelNrMap();
        /// Sets a mapping table, which consists of a channel name as key and channel number as value.
    void setNameChannelNrMap(const std::map<std::string, int> & nameChannelNrMap);

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
        /// Name of the TUI object instance
    std::string name;
        /// ID of the TUI object instance
    int id;
        /// Name of the TUI object type
    std::string typeName;
        /// Additional description
    std::string description;

        /// A mapping table, which consists of a channel name as key and channel number as value.
    std::map<std::string, int> nameChannelNrMap;
};

}



#endif

