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


#ifndef _tuiframework_Vector4TimesMSP_h_
#define _tuiframework_Vector4TimesMSP_h_

#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/MSPType.h>
#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/core/EventDelegate.h>

#include <string>
#include <map>

namespace tuiframework {
/**
 *  Vector4TimesMSP
 *
 *  \author Oliver Belaifa
 *  \date 2013
 */ 
class Vector4TimesMSP : public IMSP {
public:
    static IMSP * createFunction(void * arg);
    static const std::string & getMSPTypeName();

    Vector4TimesMSP(const MSPConfig & config);
    virtual ~Vector4TimesMSP();

    virtual const std::string & getTypeName() const;
    virtual IEventSink * getEventSink(const std::string & name);
    virtual void registerEventSink(const std::string & name, IEventSink * eventSink);
    virtual const MSPType & getMSPType() const;

    void handleA(Vector4Event * e);
    void handleB(Vector4Event * e);
    void handleBP(PackedVector4Event * e);
    
protected:
    void initVector(const ParameterGroup & parameterGroup, Vector4<double> & vec);
    
protected:
    MSPConfig config;
    MSPType type;

    EventDelegateNC<Vector4Event, Vector4TimesMSP> eventDelegateA;
    EventDelegateNC<Vector4Event, Vector4TimesMSP> eventDelegateB;
    
    EventDelegateNC<PackedVector4Event, Vector4TimesMSP> eventDelegateBP;
    
    Vector4<double> a;
    Vector4<double> b;

    IEventSink * outAB;
    IEventSink * outABPacked;
};

}

#endif