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



#ifndef _CommonTUITypesPlugin_h_
#define _CommonTUITypesPlugin_h_

#include <string>

namespace tuiframework {
    class IEventFactory;
    class IEventChannelFactory;
}

extern "C" {
    void soInitPlugin();
    void soTypeRegistration(tuiframework::IEventFactory * eventFactory, tuiframework::IEventChannelFactory * eventChannelFactory);
    
    std::string soGetTFVersion();
    std::string soGetSOVersion();
}



#endif