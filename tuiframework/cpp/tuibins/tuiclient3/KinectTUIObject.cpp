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


#include "KinectTUIObject.h"

using namespace tuiframework;
using namespace std;

KinectTUIObject::KinectTUIObject() {
}


KinectTUIObject::~KinectTUIObject() {
}


void KinectTUIObject::connect() {
    try {
        CONNECT(Matrix4ChangedEvent, "KinectTUIObjectInstance", "HeadTransf",
                KinectTUIObject, this, &KinectTUIObject::headTransfChanged);
        CONNECT(Matrix4ChangedEvent, "KinectTUIObjectInstance", "RightHandTransf",
                KinectTUIObject, this, &KinectTUIObject::rightHandTransfChanged);
        CONNECT(Matrix4ChangedEvent, "KinectTUIObjectInstance", "LeftHandTransf",
                KinectTUIObject, this, &KinectTUIObject::leftHandTransfChanged);

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void KinectTUIObject::disconnect() {
    try {
        DISCONNECT(Matrix4ChangedEvent, "KinectTUIObjectInstance", "Head",
                   KinectTUIObject, this, &KinectTUIObject::headTransfChanged);
        DISCONNECT(Matrix4ChangedEvent, "KinectTUIObjectInstance", "RightHand",
                   KinectTUIObject, this, &KinectTUIObject::rightHandTransfChanged);
        DISCONNECT(Matrix4ChangedEvent, "KinectTUIObjectInstance", "LeftHand",
                   KinectTUIObject, this, &KinectTUIObject::leftHandTransfChanged);

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void KinectTUIObject::headTransfChanged(const Matrix4ChangedEvent * e) {
	cout << "KinectTUIObject: headTransfChanged: " << e->getPayload() << endl;
}


void KinectTUIObject::rightHandTransfChanged(const Matrix4ChangedEvent * e) {
	cout << "KinectTUIObject: rightHandTransfChanged: " << e->getPayload() << endl;
}


void KinectTUIObject::leftHandTransfChanged(const Matrix4ChangedEvent * e) {
	cout << "KinectTUIObject: leftHandTransfChanged: " << e->getPayload() << endl;
}

