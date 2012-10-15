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


#ifndef _tuiframework_GUIDEventTypeIDMap_h_
#define _tuiframework_GUIDEventTypeIDMap_h_

#include <vector>
#include <string>
#include <utility>

#include "ISerializable.h"

namespace tuiframework {
 /**
 *  GUIDEventTypeIDVector
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */    
class GUIDEventTypeIDVector : public ISerializable {
public:
    GUIDEventTypeIDVector();
    virtual ~GUIDEventTypeIDVector();
    
    void setGUIDEventTypeIDVector(const std::vector<std::pair<std::string, int> > & guidEventTypeIDVector);
    const std::vector<std::pair<std::string, int> > & getGUIDEventTypeIDVector() const;
    
    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);
    
protected:
    std::vector<std::pair<std::string, int> > guidEventTypeIDVector;
};
    
}

#endif

