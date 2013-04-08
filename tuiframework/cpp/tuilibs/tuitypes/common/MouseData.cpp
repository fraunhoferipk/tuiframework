#include "MouseData.h"

#include <iostream>

#define MOUSE_DATA_VERSION 1


MouseData::MouseData() :
	id(-1),
	deltaX(0),
	deltaY(0),
	deltaWheel(0),
	buttonCode(0)  {
	
}


MouseData::MouseData(int id) :
	id(id),
	deltaX(0),
	deltaY(0),
	deltaWheel(0),
	buttonCode(0) {
}


MouseData::~MouseData() {
}


void MouseData::setId(int id) {
	this->id = id;
}


void MouseData::setDeltaX(int deltaX) {
	this->deltaX = deltaX;
}


void MouseData::setDeltaY(int deltaY) {
	this->deltaY = deltaY;
}


void MouseData::setDeltaWheel(int deltaWheel) {
	this->deltaWheel = deltaWheel;
}


void MouseData::setButtonCode(unsigned int buttonCode) {
	this->buttonCode = buttonCode;
}


int MouseData::getId() const {
	return this->id;
}


int MouseData::getDeltaX() const {
	return this->deltaX;
}


int MouseData::getDeltaY() const {
	return this->deltaY;
}


int MouseData::getDeltaWheel() const {
	return this->deltaWheel;
}


bool MouseData::getButtonStateChange(CODE code) const {
	return this->buttonCode & static_cast<unsigned int>(code) ? true : false;
}


bool MouseData::hasChanged() const {
	return this->deltaX != 0 || this->deltaY != 0 || this->deltaWheel != 0 || this-> buttonCode != 0;
}


std::ostream & MouseData::serialize(std::ostream & os) const {
	os << MOUSE_DATA_VERSION << " " << this->id << " " << this->deltaX << " " << this->deltaY << " " << this->deltaWheel << " " << this->buttonCode;
	return os;
}


std::istream & MouseData::deSerialize(std::istream & is) {
	int version;
	is >> version;
	if (version != MOUSE_DATA_VERSION) {
		// @@TODO throw exception
	}
	is >> this->id >> this->deltaX >> this->deltaY >> this->deltaWheel >> this->buttonCode;
	return is;
}
