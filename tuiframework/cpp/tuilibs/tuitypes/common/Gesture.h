#ifndef _TUIFRAMEWORK_GESTURE_H_
#define _TUIFRAMEWORK_GESTURE_H_

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
        NoMoreGrab = 5,
        LeftHandOverShoulder = 6,
        NoLeftHandOverShoulder = 7
    };

    enum HandSide {
        LeftHand = 0,
        RightHand = 1,
        NoHand = 2,
        BothHands = 3
    };

	Gesture();
	Gesture(GestureType, HandSide, float, float, float);

	virtual ~Gesture();

    virtual int getGestureType() const;
    virtual int getHandSide() const;

    virtual float getPosX() const;
    virtual float getPosY() const;
    virtual float getPosZ() const;

	virtual std::ostream & serialize(std::ostream & os) const;
	virtual std::istream & deSerialize(std::istream & is);

protected:
    GestureType m_GestureType;
    HandSide m_HandSide;
    float m_posX, m_posY, m_posZ;

};


#endif

