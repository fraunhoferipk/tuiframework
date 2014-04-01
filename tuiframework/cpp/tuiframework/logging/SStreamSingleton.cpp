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


#include "SStreamSingleton.h"

#include <pthread.h>

using namespace std;

namespace tuiframework {

map<int, stringstream *> SStreamSingleton::ssMap;


SStreamSingleton::SStreamSingleton() {
}


SStreamSingleton::~SStreamSingleton() {
    map<int, stringstream *>::iterator i = ssMap.begin();
    map<int, stringstream *>::iterator e = ssMap.end();
    while (i != e) {
        delete (*i).second;
        ++i;
    }
}


stringstream & SStreamSingleton::getSS() {
    pthread_t tid = pthread_self();
#ifdef _WIN32
    long int key = reinterpret_cast<long int>(tid.p);
#else
    long int key = static_cast<long int>((long int) tid);
#endif
    map<int, stringstream *>::iterator iter = ssMap.find(key);

    if (iter == ssMap.end()) {
        stringstream * ss = new stringstream();
        ssMap.insert(map<int, stringstream *>::value_type(key, ss));
        return *ss;
    }

    return *(*iter).second;
}


}

