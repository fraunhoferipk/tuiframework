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


#ifndef _tuiframework_MSPInstance_h_
#define _tuiframework_MSPInstance_h_

#include "../core/ISerializable.h"
#include "ParameterGroup.h"
#include <string>
#include <map>

namespace tuiframework {

/**
 *  A Description of a multi stream processor instance.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class MSPInstance : public ISerializable {
public:
    MSPInstance();
    virtual ~MSPInstance();

        /// Sets the name of the multi stream processor instance.
    void setName(const std::string & name);
        /// Returns the name of the multi stream processor instance.
    const std::string & getName() const;

        /// Sets the multi stream processor type name.
    void setTypeName(const std::string & typeName);
        /// Returns the multi stream processor type name.
    const std::string & getTypeName() const;

        /// Sets the additional description.
    void setDescription(const std::string & description);
        /// Returns the additional description.
    const std::string & getDescription() const;

    void setParameterGroup(const ParameterGroup & parameterGroup);
    const ParameterGroup & getParameterGroup() const;

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
        /// Name of the multi stream processor instance
    std::string name;
        /// Name of the multi stream processor type
    std::string typeName;
        /// Additional description
    std::string description;

    ParameterGroup parameterGroup;
};

}

#endif

