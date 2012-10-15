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


#ifndef _tuiframework_EPAddressTranslator_h_
#define _tuiframework_EPAddressTranslator_h_

#include "../core/IEventSink.h"

#include <map>
#include <string>

namespace tuiframework {
 /**
 *  EPAddressTranslator
 *
 *  \author Oliver Belaifa
 *  \date 2010-2012
 */
class EPAddressTranslator : public IEventSink {
public:
    EPAddressTranslator(int eID = -1, int pNr = -1, IEventSink * eventSink = 0);
    virtual ~EPAddressTranslator();

    void setEventSink(IEventSink * eventSink);

    void setEID(int id);
    int getEID() const;

    void setPNr(int nr);
    int getPNr() const;

    virtual void push(IEvent * event);

protected:
    IEventSink * eventSink;
    int pNr;
    int eID;
};

}

#endif
