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


#ifndef _tuiframework_Multiton_h_
#define _tuiframework_Multiton_h_

#include <map>

namespace tuiframework {

/**
 *  Provides a Multiton of class T.
 *
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template <class T>
class Multiton {
protected:
    Multiton() { }
    ~Multiton() {
        typename std::map<int, T *>::iterator i = instanceMap.begin();
        typename std::map<int, T *>::iterator e = instanceMap.end();
        while (i != e) {
            delete (*i).second;
        }
        instanceMap.clear();
    }

public:
        /// Returns the indexed instance of T.
    static T * getInstance(int index = 0) {
        T * t;
        typename std::map<int, T *>::iterator iter = instanceMap.find(index);
        if (iter == instanceMap.end()) {
            t = new T();
            instanceMap.insert(typename std::map<int, T *>::value_type(index, t));
        } else {
            t = (*iter).second;
        }
        return t;
    }

protected:
        /// Map of instances of T
    static std::map<int, T *> instanceMap;
};

}

#endif

