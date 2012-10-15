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



#ifndef _GESTURE_H_
#define _GESTURE_H_

#include <tuiframework/core/ISerializable.h>
#include <iostream>
#include <cstdio>

class Gesture : public tuiframework::ISerializable {
public:
    enum GestureType {
        Wave = 0,
        Click = 1,
        NoGesture = 3,
        Grab = 4,
        NoMoreGrab = 5
    };

    enum HandSide {
        LeftHand = 0,
        RightHand = 1,
        NoHand = 2
    };

    Gesture();

    Gesture(GestureType, HandSide, float, float, float);
    virtual ~Gesture();
    
    int getGestureType() const;
    int getHandSide() const;

    float getPosX() const;
    float getPosY() const;
    float getPosZ() const;

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
    GestureType m_GestureType;
    HandSide m_HandSide;
    float m_posX, m_posY, m_posZ;
};

#endif

