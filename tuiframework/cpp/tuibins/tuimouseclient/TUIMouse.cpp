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


#include "TUIMouse.h"

using namespace tuiframework;
using namespace std;

TUIMouse::TUIMouse() {
}


TUIMouse::~TUIMouse() {
}


void TUIMouse::connect() {
    try {
        CONNECT(DigitalChangedEvent, "TUIMouse1", "LeftButton",
                TUIMouse, this, &TUIMouse::leftButtonChanged);
        CONNECT(DigitalChangedEvent, "TUIMouse1", "RightButton",
                TUIMouse, this, &TUIMouse::rightButtonChanged);
        CONNECT(DigitalChangedEvent, "TUIMouse1", "MiddleButton",
                TUIMouse, this, &TUIMouse::middleButtonChanged);
        CONNECT(DigitalChangedEvent, "TUIMouse1", "CustomButton1",
                TUIMouse, this, &TUIMouse::customButton1Changed);
        CONNECT(DigitalChangedEvent, "TUIMouse1", "CustomButton2",
                TUIMouse, this, &TUIMouse::customButton2Changed);
        CONNECT(IntegerChangedEvent, "TUIMouse1", "DeltaX",
                TUIMouse, this, &TUIMouse::deltaXChanged);
        CONNECT(IntegerChangedEvent, "TUIMouse1", "DeltaY",
                TUIMouse, this, &TUIMouse::deltaYChanged);
        CONNECT(IntegerChangedEvent, "TUIMouse1", "DeltaWheel",
                TUIMouse, this, &TUIMouse::deltaWheelChanged);

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void TUIMouse::disconnect() {
    try {
        DISCONNECT(DigitalChangedEvent, "TUIMouse1", "LeftButton",
                TUIMouse, this, &TUIMouse::leftButtonChanged);
        DISCONNECT(DigitalChangedEvent, "TUIMouse1", "RightButton",
                TUIMouse, this, &TUIMouse::rightButtonChanged);
        DISCONNECT(DigitalChangedEvent, "TUIMouse1", "MiddleButton",
                TUIMouse, this, &TUIMouse::middleButtonChanged);
        DISCONNECT(DigitalChangedEvent, "TUIMouse1", "CustomButton1",
                TUIMouse, this, &TUIMouse::customButton1Changed);
        DISCONNECT(DigitalChangedEvent, "TUIMouse1", "CustomButton2",
                TUIMouse, this, &TUIMouse::customButton2Changed);
        DISCONNECT(IntegerChangedEvent, "TUIMouse1", "DeltaX",
                TUIMouse, this, &TUIMouse::deltaXChanged);
        DISCONNECT(IntegerChangedEvent, "TUIMouse1", "DeltaY",
                TUIMouse, this, &TUIMouse::deltaYChanged);
        DISCONNECT(IntegerChangedEvent, "TUIMouse1", "DeltaWheel",
                TUIMouse, this, &TUIMouse::deltaWheelChanged);

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void TUIMouse::leftButtonChanged(const DigitalChangedEvent * e) {
	cout << "TUIMouse: LeftButton: " << e->getPayload() << endl;
}


void TUIMouse::rightButtonChanged(const DigitalChangedEvent * e) {
	cout << "TUIMouse: RightButton: " << e->getPayload() << endl;
}


void TUIMouse::middleButtonChanged(const DigitalChangedEvent * e) {
	cout << "TUIMouse: MiddleButton: " << e->getPayload() << endl;
}


void TUIMouse::customButton1Changed(const DigitalChangedEvent * e) {
	cout << "TUIMouse: CustomButton1: " << e->getPayload() << endl;
}


void TUIMouse::customButton2Changed(const DigitalChangedEvent * e) {
	cout << "TUIMouse: CustomButton2: " << e->getPayload() << endl;
}


void TUIMouse::deltaXChanged(const IntegerChangedEvent * e) {
	cout << "TUIMouse: DX: " << e->getPayload() << endl;
}


void TUIMouse::deltaYChanged(const IntegerChangedEvent * e) {
	cout << "TUIMouse: DY: " << e->getPayload() << endl;
}


void TUIMouse::deltaWheelChanged(const IntegerChangedEvent * e) {
	cout << "TUIMouse: DWheel: " << e->getPayload() << endl;
}

