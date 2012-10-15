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



#ifndef _CommonTypeReg_h_
#define _CommonTypeReg_h_

#include "Matrix4x4.h"
#include "Gesture.h"
#include "HapticAngle.h"
#include "KinectJoint.h"
#include "Matrix4Data.h"

#include "TrackerData.h"
#include "Vector3d.h"

#include <tuiframework/core/EPEventMsg.h>
#include <tuiframework/core/IEventFactory.h>
#include <tuiframework/core/IEventChannelFactory.h>

#include <string>

   
typedef tuiframework::EPEventMsg<bool, 11>                  DigitalChangedEvent;
typedef tuiframework::EPEventMsg<float, 12>                 AnalogChangedEvent;
typedef tuiframework::EPEventMsg<TrackerData, 13>           TrackerChangedEvent;
typedef tuiframework::EPEventMsg<Matrix4Data, 14>           Matrix4ChangedEvent;
typedef tuiframework::EPEventMsg<HapticAngle, 15>           HapticChangedEvent;
typedef tuiframework::EPEventMsg<Matrix4x4<double>, 16>     Matrix4x4ChangedEvent;
typedef tuiframework::EPEventMsg<KinectJoint, 17>           KinectEvent;
typedef tuiframework::EPEventMsg<Gesture, 18>               GestureEvent;
typedef tuiframework::EPEventMsg<Vector3d, 19>              Vector3dEvent;


namespace CommonTypeReg {

void registerTypes(tuiframework::IEventFactory * eventFactory, tuiframework::IEventChannelFactory * eventChannelFactory);    
//void clientRegisterTypes();
//void serverRegisterTypes();

}

#endif