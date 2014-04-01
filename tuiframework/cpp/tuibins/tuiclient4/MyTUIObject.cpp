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


#include "MyTUIObject.h"

#include <iomanip> 

using namespace tuiframework;
using namespace std;

MyTUIObject::MyTUIObject() {
}


MyTUIObject::~MyTUIObject() {
}


void MyTUIObject::connect() {
    try {
        CONNECT(Vector4Event, "Vector4Receiver", "v1", MyTUIObject, this, &MyTUIObject::v1Received);
        CONNECT(Vector4Event, "Vector4Receiver", "v2", MyTUIObject, this, &MyTUIObject::v2Received);

    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void MyTUIObject::disconnect() {
    try {
        DISCONNECT(Vector4Event, "Vector4Receiver", "v1", MyTUIObject, this, &MyTUIObject::v1Received);
        DISCONNECT(Vector4Event, "Vector4Receiver", "v2", MyTUIObject, this, &MyTUIObject::v2Received);
    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void MyTUIObject::v1Received(const Vector4Event * e) {
    this->v1 = e->getPayload();
    this->print();
}


void MyTUIObject::v2Received(const Vector4Event * e) {
    this->v2 = e->getPayload();
    this->print();
    
    /*
    //std::cout << "(" << this->v1 << ")   (" << this->v2 << ")" << std::endl;
    std::cout << "pair<Vector4<double>, Vector4<double> >(Vector4<double>(";
    std::cout << this->v1[0] << ", " << this->v1[1] << ", " << this->v1[2] << "), Vector4<double>(";
    std::cout << this->v2[0] << ", " << this->v2[1] << ", " << this->v2[2] << "));" << std::endl;
*/    
}


void MyTUIObject::print() {
    {
        cout << "(";
        for (int x = 0; x < 3; ++x) {
            cout << setw(12) << fixed << setprecision(4) << this->v1[x];
        }
        cout << " )";
    }
    {
        cout << " -> (";
        for (int x = 0; x < 3; ++x) {
            cout << setw(12) << fixed << setprecision(4) << this->v2[x];
        }
        cout << " )" << endl;
    }
}

