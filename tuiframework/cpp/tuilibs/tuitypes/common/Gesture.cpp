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
