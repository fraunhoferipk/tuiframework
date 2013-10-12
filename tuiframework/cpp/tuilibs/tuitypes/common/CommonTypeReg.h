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
#include "VectorList3d.h"
#include "Vector3d.h"
#include "MouseData.h"

#include <tuiframework/core/EPEventMsg.h>
#include <tuiframework/core/IEventFactory.h>
#include <tuiframework/core/IEventChannelFactory.h>

#include <string>

   
typedef tuiframework::EPEventMsg<bool, 11>                  DigitalChangedEvent;
typedef tuiframework::EPEventMsg<float, 12>                 AnalogChangedEvent;
typedef tuiframework::EPEventMsg<int, 13>                   IntegerChangedEvent;
typedef tuiframework::EPEventMsg<TrackerData, 14>           TrackerChangedEvent;
typedef tuiframework::EPEventMsg<Matrix4Data, 15>           Matrix4ChangedEvent;
typedef tuiframework::EPEventMsg<HapticAngle, 16>           HapticChangedEvent;
typedef tuiframework::EPEventMsg<Matrix4x4<double>, 17>     Matrix4x4ChangedEvent;
typedef tuiframework::EPEventMsg<KinectJoint, 18>           KinectEvent;
typedef tuiframework::EPEventMsg<Gesture, 19>               GestureEvent;
typedef tuiframework::EPEventMsg<Vector3d, 20>              Vector3dEvent;
typedef tuiframework::EPEventMsg<MouseData, 21>             MouseEvent;
typedef tuiframework::EPEventMsg<VectorList3d, 22>          VectorList3dEvent;


namespace CommonTypeReg {

void registerTypes(tuiframework::IEventFactory * eventFactory, tuiframework::IEventChannelFactory * eventChannelFactory);    

}

#endif
