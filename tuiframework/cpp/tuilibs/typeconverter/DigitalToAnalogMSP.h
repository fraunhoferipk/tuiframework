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
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the TUIFramework. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _tuiframework_DigitalToAnalogMSP_h_
#define _tuiframework_DigitalToAnalogMSP_h_

#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/MSPType.h>
#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/core/EventDelegate.h>

#include <string>
#include <map>

namespace tuiframework {

/**
 *  DigitalToAnalogMSP
 *
 *  \author Oliver Belaifa
 *  \date 2013
 */ 
class DigitalToAnalogMSP : public IMSP {
public:
    static IMSP * createFunction(void * arg);
    static std::string getMSPTypeName();

    DigitalToAnalogMSP(const MSPConfig & config);
    virtual ~DigitalToAnalogMSP();

    virtual const std::string & getTypeName() const;
    virtual IEventSink * getEventSink(const std::string & name);
    virtual void registerEventSink(const std::string & name, IEventSink * eventSink);
    virtual const MSPType & getMSPType() const;

    void handleEvent(DigitalChangedEvent * e);

protected:
    MSPConfig config;
    MSPType type;

    EventDelegateNC<DigitalChangedEvent, DigitalToAnalogMSP> eventDelegate;

    IEventSink * out;
    
    float value0;
    float value1;
};

}

#endif