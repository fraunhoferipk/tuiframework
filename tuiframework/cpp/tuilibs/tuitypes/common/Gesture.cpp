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



#include "Gesture.h"

Gesture::Gesture()
: m_GestureType(NoGesture), m_HandSide(NoHand) {}

Gesture::Gesture( GestureType type, HandSide hand, float x, float y, float z)
: m_GestureType(type), m_HandSide(hand), m_posX(x), m_posY(y), m_posZ(z){}

Gesture::~Gesture() {}

int Gesture::getGestureType() const{
    return this->m_GestureType;
}

int Gesture::getHandSide() const{
        return this->m_HandSide;
}

float Gesture::getPosX() const{
    return this->m_posX;
}

float Gesture::getPosY() const{
    return this->m_posY;
}

float Gesture::getPosZ() const{
    return this->m_posZ;
}

std::ostream & Gesture::serialize(std::ostream & os) const {
    os << (int)(this->m_GestureType) << " " << (int)(this->m_HandSide) << " " << this->m_posX << " " << this->m_posY << " " << this->m_posZ;
    return os;
}

std::istream & Gesture::deSerialize(std::istream & is) {
    long gestureType,handSide;

    is >> gestureType >> handSide >> this->m_posX >> this->m_posY >> this->m_posZ;
    this->m_GestureType = (GestureType) gestureType;
    this->m_HandSide = (HandSide) handSide;
    return is;
}
