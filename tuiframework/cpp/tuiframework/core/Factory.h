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


#ifndef _tuiframework_Factory_h_
#define _tuiframework_Factory_h_

#include <map>

namespace tuiframework {

/**
 *  Represents a factory which allows producing products V
 *  by a key K using a pre registered create function.
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
template <class K, class V>
class Factory {
public:
    Factory() {
    }

    virtual ~Factory() {
    }

        /// Creates an instance of type V by a key K.
    V * createInstance(const K & key, void * arg = 0) {
        typename std::map<K, V * ( * ) (void *)>::iterator iter = this->registry.find(key);
        if (iter != this->registry.end()) {
            return (*iter).second(arg);
        }
        return 0;
    }

        /// Registers a create function
    void registerCreateFunction(const K & key, V * ( * createFunction) (void *)) {
        this->registry[key] = createFunction;
    }

protected:
        /// A mapping table, which consists of a key K and
        /// a create function pointer as value.
    std::map<K, V * ( * ) (void *)> registry;
};

}

#endif
