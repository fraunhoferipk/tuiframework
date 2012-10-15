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


#ifndef _tuiframework_AttachedObjects_h_
#define _tuiframework_AttachedObjects_h_

#include "../core/TUIObjectType.h"
#include "../core/TUIObjectInstance.h"

#include <vector>

namespace tuiframework {

/**
 *  Information about the attached device and TUI objects.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class AttachedObjects : public ISerializable {
public:
    AttachedObjects();
    virtual ~AttachedObjects();

        /// Sets a table of TUI object type description objects.
    void setTUIObjectTypeVector(const std::vector<TUIObjectType> & tuiObjectTypeVector);
        /// Returns a table of TUI object type description objects.
    const std::vector<TUIObjectType> & getTUIObjectTypeVector() const;

        /// Sets a table of TUI object instance description objects.
    void setTUIObjectInstanceVector(const std::vector<TUIObjectInstance> & tuiObjectInstanceVector);
        /// Returns a table of TUI object instance description objects.
    const std::vector<TUIObjectInstance> & getTUIObjectInstanceVector() const;

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
        /// TUI object type description object table
    std::vector<TUIObjectType> tuiObjectTypeVector;
        /// TUI object instance description object table
    std::vector<TUIObjectInstance> tuiObjectInstanceVector;
};


}

#endif
