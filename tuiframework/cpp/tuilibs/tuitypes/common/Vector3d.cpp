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


#include "Vector3d.h"

Vector3d::Vector3d()
: x(0), y(0), z(0) {}

Vector3d::Vector3d(double x, double y, double z)
: x(x), y(y), z(z) {}

Vector3d::~Vector3d() {}

void Vector3d::setX( double a ) { this->x = a; }
void Vector3d::setY( double a ) { this->y = a; }
void Vector3d::setZ( double a ) { this->z = a; }

double Vector3d::getX() const { return this->x; }
double Vector3d::getY() const { return this->y; }
double Vector3d::getZ() const { return this->z; }

void Vector3d::add( Vector3d v ) {
    this->x += v.getX();
    this->y += v.getY();
    this->z += v.getZ();
}

void Vector3d::subtract( Vector3d v ) {
    this->x -= v.getX();
    this->y -= v.getY();
    this->z -= v.getZ();
}

Vector3d Vector3d::crossProduct( Vector3d v ) {
    return Vector3d(	this->y * v.z - v.y * this->z,
                        this->z * v.x - v.z * this->x,
                        this->x * v.y - v.x * this->y );
}

double Vector3d::dotProduct( Vector3d v ) {
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

double Vector3d::absoluteValue() {
    return sqrt( pow( this->x, 2 ) + pow( this->y, 2 ) + pow( this->z, 2 ) );
}

double Vector3d::distance( Vector3d v ) {
    // calculate displacement vector
    v.subtract( *this );
    // return absolute value of displacement vector
    return v.absoluteValue();
}

double Vector3d::distance( Vector3d a, Vector3d b, Vector3d c ) {
    // calculate 2 displacement vectors in plane
    b.subtract( a );
    c.subtract( a );
    // calculate normal vector
    Vector3d n = b.crossProduct( c );
    Vector3d p = *this;
    p.subtract( a );
    return n.dotProduct( p ) / n.absoluteValue();
}

void Vector3d::normalize() {
    double leng = this->x*this->x + this->y*this->y + this->z*this->z;
    leng = 1.0/std::sqrt(leng);

    this->x *= leng;
    this->y *= leng;
    this->z *= leng;
}

// for testing purposes
void Vector3d::printValues() {
    std::printf("(x,y,z) = (%.3f, %.3f, %.3f)", this->x, this->y, this->z);
}

std::ostream & Vector3d::serialize(std::ostream & os) const {
    os << this->x << " " << this->y << " " << this->z;
    return os;
}

std::istream & Vector3d::deSerialize(std::istream & is) {
    is >> this->x >> this->y >> this->z;
    return is;
}

