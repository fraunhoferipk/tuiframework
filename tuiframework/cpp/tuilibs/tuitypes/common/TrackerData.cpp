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

#include "TrackerData.h"

#include <iostream>

using namespace std;


TrackerData::TrackerData() {
}


TrackerData::~TrackerData() {
}


double TrackerData::getPos(int index) const {
    return this->pos[index];
}


void TrackerData::setPos(int index, double value) {
    this->pos[index] = value;
}


double TrackerData::getQuat(int index) const {
    return this->quat[index];
}


void TrackerData::setQuat(int index, double value) {
    this->quat[index] = value;
}


void TrackerData::getPos(double pos[3]) const {
    pos[0] = this->pos[0];
    pos[1] = this->pos[1];
    pos[2] = this->pos[2];
}


void TrackerData::setPos(double pos[3]) {
    this->pos[0] = pos[0];
    this->pos[1] = pos[1];
    this->pos[2] = pos[2];
}


void TrackerData::getQuat(double quat[4]) const {
    quat[0] = this->quat[0];
    quat[1] = this->quat[1];
    quat[2] = this->quat[2];
    quat[3] = this->quat[3];
}


void TrackerData::setQuat(double quat[4]) {
    this->quat[0] = quat[0];
    this->quat[1] = quat[1];
    this->quat[2] = quat[2];
    this->quat[3] = quat[3];
}


void TrackerData::setPos(double p1, double p2, double p3) {
    this->pos[0] = p1;
    this->pos[1] = p2;
    this->pos[2] = p3;
}


void TrackerData::setQuat(double q1, double q2, double q3, double q4) {
    this->quat[0] = q1;
    this->quat[1] = q2;
    this->quat[2] = q3;
    this->quat[3] = q4;
}


std::ostream & TrackerData::serialize(std::ostream & os) const {
    os << this->pos[0] << " " << this->pos[1] << " " << this->pos[2] << " ";
    os << this->quat[0] << " " << this->quat[1] << " " << this->quat[2] << " " << this->quat[3];
    return os;
}


std::istream & TrackerData::deSerialize(std::istream & is) {
    is >> this->pos[0] >> this->pos[1] >> this->pos[2];
    is >> this->quat[0] >> this->quat[1] >> this->quat[2] >> this->quat[3];
    return is;
}


