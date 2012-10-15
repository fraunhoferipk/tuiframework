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


//#define USE_TFDEBUG
#include <tuiframework/core/TypeRegistration.h>

using namespace tuiframework;

/*
#include <tuiframework/client/ClientTypeReg.h>
#include <tuiframework/server/ServerTypeReg.h>

char digitalGUID[] = "51af203c-2c03-401d-8554-0506d07ac883";
char analogGUID[] = "f0d1bf7d-d5a3-4a73-a418-d8d02fc26097";
char doubleMatrix4x4GUID[] = "be46d70f-baa3-4f49-8254-fe26f8ac3910";
char gestureGUID[] = "9ab80f90-bc96-4fd4-a708-4f42412a0b2c";
char hapticAngleGUID[] = "8aae2fba-6411-478b-bb16-3d8a2d4ba233";
char kinectJointGUID[] = "80e104a8-30f8-4d91-affe-b06dc1fbbcb0";
char matrix4DataGUID[] = "2c41120c-534a-4754-80b7-4fa940405c0f";
char trackerDataGUID[] = "7f7de6c6-7b80-4cad-960a-a7f987de6db2";
char vector3dGUID[] = "434e37cd-b988-489d-aacd-c1a96fbe2276";
*/


namespace CommonTypeReg {

void registerTypes(IEventFactory * eventFactory, IEventChannelFactory * eventChannelFactory) {
    tuiframework::registerType<bool, 11>(eventFactory, eventChannelFactory, "DigitalChannel");
    tuiframework::registerType<float, 12>(eventFactory, eventChannelFactory, "AnalogChannel");
    tuiframework::registerType<TrackerData, 13>(eventFactory, eventChannelFactory, "TrackerChannel");
    tuiframework::registerType<Matrix4Data, 14>(eventFactory, eventChannelFactory, "Matrix4Channel");
    tuiframework::registerType<HapticAngle, 15>(eventFactory, eventChannelFactory, "HapticChannel");
    tuiframework::registerType<Matrix4x4<double>, 16>(eventFactory, eventChannelFactory, "Matrix4x4Channel");
    tuiframework::registerType<Gesture, 17>(eventFactory, eventChannelFactory, "GestureChannel");
    tuiframework::registerType<KinectJoint, 18>(eventFactory, eventChannelFactory, "KinectChannel");
    tuiframework::registerType<Vector3d, 19>(eventFactory, eventChannelFactory, "Vector3dChannel");    
}

}

/*
void clientRegisterTypes() {
    tuiframework::client::registerType<bool, digitalGUID>("DigitalChannel");
    tuiframework::client::registerType<float, analogGUID>("AnalogChannel");
    tuiframework::client::registerType<TrackerData, trackerDataGUID>("TrackerChannel");
    tuiframework::client::registerType<Matrix4Data, matrix4DataGUID>("Matrix4Channel");
    tuiframework::client::registerType<HapticAngle, hapticAngleGUID>("HapticChannel");
    tuiframework::client::registerType<Matrix4x4<double>, doubleMatrix4x4GUID>("Matrix4x4Channel");
    tuiframework::client::registerType<Gesture, gestureGUID>("GestureChannel");
    tuiframework::client::registerType<KinectJoint, kinectJointGUID>("KinectChannel");
    tuiframework::client::registerType<Vector3d, vector3dGUID>("Vector3dChannel");    
}


void serverRegisterTypes() {
    tuiframework::server::registerType<bool, digitalGUID>();
    tuiframework::server::registerType<float, analogGUID>();
    tuiframework::server::registerType<TrackerData, trackerDataGUID>();
    tuiframework::server::registerType<Matrix4Data, matrix4DataGUID>();
    tuiframework::server::registerType<HapticAngle, hapticAngleGUID>();
    tuiframework::server::registerType<Matrix4x4<double>, doubleMatrix4x4GUID>();
    tuiframework::server::registerType<Gesture, gestureGUID>();
    tuiframework::server::registerType<KinectJoint, kinectJointGUID>();
    tuiframework::server::registerType<Vector3d, vector3dGUID>();
}

*/