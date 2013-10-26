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


#include "Skeleton.h"

#include <iomanip> 

using namespace tuiframework;
using namespace std;

Skeleton::Skeleton() {
}


Skeleton::~Skeleton() {
}


void Skeleton::connect() {
    try {
        CONNECT(Matrix4Event, "SkeletonInstance", "CALIBRATION", Skeleton, this, &Skeleton::updateCALIBRATION);
        CONNECT(Matrix4Event, "SkeletonInstance", "Head", Skeleton, this, &Skeleton::updateHead);
        CONNECT(Matrix4Event, "SkeletonInstance", "Neck", Skeleton, this, &Skeleton::updateNeck);
        CONNECT(Matrix4Event, "SkeletonInstance", "Torso", Skeleton, this, &Skeleton::updateTorso);
        CONNECT(Matrix4Event, "SkeletonInstance", "Waist", Skeleton, this, &Skeleton::updateWaist);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Collar", Skeleton, this, &Skeleton::updateLeft_Collar);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Shoulder", Skeleton, this, &Skeleton::updateLeft_Shoulder);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Elbow", Skeleton, this, &Skeleton::updateLeft_Elbow);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Wrist", Skeleton, this, &Skeleton::updateLeft_Wrist);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Hand", Skeleton, this, &Skeleton::updateLeft_Hand);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Fingertip", Skeleton, this, &Skeleton::updateLeft_Fingertip);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Collar", Skeleton, this, &Skeleton::updateRight_Collar);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Shoulder", Skeleton, this, &Skeleton::updateRight_Shoulder);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Elbow", Skeleton, this, &Skeleton::updateRight_Elbow);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Wrist", Skeleton, this, &Skeleton::updateRight_Wrist);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Hand", Skeleton, this, &Skeleton::updateRight_Hand);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Fingertip", Skeleton, this, &Skeleton::updateRight_Fingertip);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Hip", Skeleton, this, &Skeleton::updateLeft_Hip);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Knee", Skeleton, this, &Skeleton::updateLeft_Knee);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Ankle", Skeleton, this, &Skeleton::updateLeft_Ankle);
        CONNECT(Matrix4Event, "SkeletonInstance", "left_Foot", Skeleton, this, &Skeleton::updateLeft_Foot);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Hip", Skeleton, this, &Skeleton::updateRight_Hip);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Knee", Skeleton, this, &Skeleton::updateRight_Knee);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Ankle", Skeleton, this, &Skeleton::updateRight_Ankle);
        CONNECT(Matrix4Event, "SkeletonInstance", "right_Foot", Skeleton, this, &Skeleton::updateRight_Foot);
    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void Skeleton::disconnect() {
    try {
        DISCONNECT(Matrix4Event, "SkeletonInstance", "CALIBRATION", Skeleton, this, &Skeleton::updateCALIBRATION);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "Head", Skeleton, this, &Skeleton::updateHead);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "Neck", Skeleton, this, &Skeleton::updateNeck);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "Torso", Skeleton, this, &Skeleton::updateTorso);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "Waist", Skeleton, this, &Skeleton::updateWaist);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Collar", Skeleton, this, &Skeleton::updateLeft_Collar);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Shoulder", Skeleton, this, &Skeleton::updateLeft_Shoulder);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Elbow", Skeleton, this, &Skeleton::updateLeft_Elbow);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Wrist", Skeleton, this, &Skeleton::updateLeft_Wrist);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Hand", Skeleton, this, &Skeleton::updateLeft_Hand);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Fingertip", Skeleton, this, &Skeleton::updateLeft_Fingertip);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Collar", Skeleton, this, &Skeleton::updateRight_Collar);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Shoulder", Skeleton, this, &Skeleton::updateRight_Shoulder);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Elbow", Skeleton, this, &Skeleton::updateRight_Elbow);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Wrist", Skeleton, this, &Skeleton::updateRight_Wrist);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Hand", Skeleton, this, &Skeleton::updateRight_Hand);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Fingertip", Skeleton, this, &Skeleton::updateRight_Fingertip);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Hip", Skeleton, this, &Skeleton::updateLeft_Hip);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Knee", Skeleton, this, &Skeleton::updateLeft_Knee);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Ankle", Skeleton, this, &Skeleton::updateLeft_Ankle);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "left_Foot", Skeleton, this, &Skeleton::updateLeft_Foot);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Hip", Skeleton, this, &Skeleton::updateRight_Hip);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Knee", Skeleton, this, &Skeleton::updateRight_Knee);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Ankle", Skeleton, this, &Skeleton::updateRight_Ankle);
        DISCONNECT(Matrix4Event, "SkeletonInstance", "right_Foot", Skeleton, this, &Skeleton::updateRight_Foot);
    } catch (const Exception & e) {
        cerr << "Exception" << endl;
        cerr << e.getFormattedString() << endl;
    }
}


void Skeleton::updateCALIBRATION(const Matrix4Event * e) {
    this->print("CALIBRATION", e);
}


void Skeleton::updateHead(const Matrix4Event * e) {
    this->print("Head", e);
}


void Skeleton::updateNeck(const Matrix4Event * e) {
    this->print("Neck", e);
}


void Skeleton::updateTorso(const Matrix4Event * e) {
    this->print("Torso", e);
}


void Skeleton::updateWaist(const Matrix4Event * e) {
    this->print("Waist", e);
}


void Skeleton::updateLeft_Collar(const Matrix4Event * e) {
    this->print("Collar", e);
}


void Skeleton::updateLeft_Shoulder(const Matrix4Event * e) {
    this->print("Shoulder", e);
}


void Skeleton::updateLeft_Elbow(const Matrix4Event * e) {
    this->print("Elbow", e);
}


void Skeleton::updateLeft_Wrist(const Matrix4Event * e) {
    this->print("Wrist", e);
}


void Skeleton::updateLeft_Hand(const Matrix4Event * e) {
    this->print("Hand", e);
}


void Skeleton::updateLeft_Fingertip(const Matrix4Event * e) {
    this->print("Fingertip", e);
}


void Skeleton::updateRight_Collar(const Matrix4Event * e) {
    this->print("Collar", e);
}


void Skeleton::updateRight_Shoulder(const Matrix4Event * e) {
    this->print("Shoulder", e);
}


void Skeleton::updateRight_Elbow(const Matrix4Event * e) {
    this->print("Elbow", e);
}


void Skeleton::updateRight_Wrist(const Matrix4Event * e) {
    this->print("Wrist", e);
}


void Skeleton::updateRight_Hand(const Matrix4Event * e) {
    this->print("Hand", e);
}


void Skeleton::updateRight_Fingertip(const Matrix4Event * e) {
    this->print("Fingertip", e);
}


void Skeleton::updateLeft_Hip(const Matrix4Event * e) {
    this->print("Hip", e);
}


void Skeleton::updateLeft_Knee(const Matrix4Event * e) {
    this->print("Knee", e);
}


void Skeleton::updateLeft_Ankle(const Matrix4Event * e) {
    this->print("Ankle", e);
}


void Skeleton::updateLeft_Foot(const Matrix4Event * e) {
    this->print("Foot", e);
}


void Skeleton::updateRight_Hip(const Matrix4Event * e) {
    this->print("Hip", e);
}


void Skeleton::updateRight_Knee(const Matrix4Event * e) {
    this->print("Knee", e);
}


void Skeleton::updateRight_Ankle(const Matrix4Event * e) {
    this->print("Ankle", e);
}


void Skeleton::updateRight_Foot(const Matrix4Event * e) {
    this->print("Foot", e);
}


void Skeleton::print(const char * name, const Matrix4Event * e) {  
    std::cout << name << ":" << std::endl;
    const Matrix4<double> & m = e->getPayload();
    for (int y = 0; y < 4; ++y) {
        cout << "(";
        for (int x = 0; x < 4; ++x) {
            cout << setw(12) << fixed << setprecision(4) << m[y][x];
        }
        cout << " )" << endl;
    }
}


