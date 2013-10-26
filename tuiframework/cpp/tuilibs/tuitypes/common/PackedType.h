/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    designed for creating applications with a tangible user interface (TUI).
    
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


#ifndef _tuiframework_PackedType_h_
#define _tuiframework_PackedType_h_

#include <tuiframework/core/ISerializable.h>
 
#include <vector>

template <class T>
class PackedType : public tuiframework::ISerializable {
public:
    PackedType() {
    }
    
    ~PackedType() {
    }
    
    
    std::vector<std::pair<int, T> > & getItems() {
        return this->items;
    }
    
    
    const std::vector<std::pair<int, T> > & getItems() const {
        return this->items;
    }
    
    
    virtual std::ostream & serialize(std::ostream & os) const {
        os << this->items.size();

        typename std::vector<std::pair<int, T> >::const_iterator i = this->items.begin();
        typename std::vector<std::pair<int, T> >::const_iterator e = this->items.end();
        while (i != e) {
            os << " " << (*i).first;
            os << " " << (*i).second;
            ++i;
        }
        
        return os;
    }
    
    
    virtual std::istream & deSerialize(std::istream & is) {
        this->items.clear();
        
        int count;
        is >> count;
        for (int i = 0; i < count; ++i) {
            int first;
            T second;
            is >> first;
            is >> second;
            this->items.push_back(std::pair<int, T>(first, second));
        }
        
        return is;
    }
    
protected:    
    std::vector<std::pair<int, T> > items; 
};

#endif

