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



#include "CommonTypeReg.h"

#include <tuiframework/core/TypeRegistration.h>

using namespace tuiframework;

namespace CommonTypeReg {

void registerTypes(IEventFactory * eventFactory, IEventChannelFactory * eventChannelFactory) {
    tuiframework::registerType<bool, 11>(eventFactory, eventChannelFactory, "DigitalChannel");
    tuiframework::registerType<float, 12>(eventFactory, eventChannelFactory, "AnalogChannel");
    tuiframework::registerType<int, 13>(eventFactory, eventChannelFactory, "IntegerChannel");
    tuiframework::registerType<TrackerData, 14>(eventFactory, eventChannelFactory, "TrackerChannel");
    tuiframework::registerType<Matrix4Data, 15>(eventFactory, eventChannelFactory, "Matrix4Channel");
    tuiframework::registerType<HapticAngle, 16>(eventFactory, eventChannelFactory, "HapticChannel");
    
    tuiframework::registerType<Gesture, 18>(eventFactory, eventChannelFactory, "GestureChannel");
    tuiframework::registerType<KinectJoint, 19>(eventFactory, eventChannelFactory, "KinectChannel");
    tuiframework::registerType<Vector3d, 20>(eventFactory, eventChannelFactory, "Vector3dChannel");
    tuiframework::registerType<MouseData, 21>(eventFactory, eventChannelFactory, "MouseChannel");
    tuiframework::registerType<VectorList3d, 22>(eventFactory, eventChannelFactory, "VectorList3dChannel");
    
    tuiframework::registerType<Vector4<double>, 23>(eventFactory, eventChannelFactory, "Vector4");
    tuiframework::registerType<PackedType<Vector4<double> >, 24>(eventFactory, eventChannelFactory, "PackedVector4");
    tuiframework::registerType<Matrix4<double>, 25>(eventFactory, eventChannelFactory, "Matrix4");
    tuiframework::registerType<PackedType<Matrix4<double> >, 26>(eventFactory, eventChannelFactory, "PackedMatrix4");
}

}
