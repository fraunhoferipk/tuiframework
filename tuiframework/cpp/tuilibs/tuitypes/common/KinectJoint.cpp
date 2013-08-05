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



#include "KinectJoint.h"

KinectJoint::KinectJoint()
: userId(1), jointId(0), confidence(0), position() {}

KinectJoint::KinectJoint( int user, int joint, double x, double y, double z, double c)
: userId(user), jointId(joint), confidence(c), position(x, y, z) {}

KinectJoint::~KinectJoint() {}

int KinectJoint::getUserId() const{
    return this->userId;
}

int KinectJoint::getJointId() const{
    return this->jointId;
}

void KinectJoint::setConfidence(double a) {
    this->confidence = a;
}

double KinectJoint::getConfidence() const{
    return this->confidence;
}

Vector3d KinectJoint::getPosition() const {
    return this->position;
}

Vector3d KinectJoint::getTransformedPosition(const Matrix4Data & mat){
    //double x = this->getPosition().getX() + mat[12];
    //double y = this->getPosition().getY()* mat[5] - this->getPosition().getZ()*mat[9] + mat[13];
    //double z = this->getPosition().getY()* mat[6] + this->getPosition().getZ()*mat[10] + mat[14];
    
    double x_ = this->getPosition().getX();
    double y_ = this->getPosition().getY();
    double z_ = this->getPosition().getZ();
    
    double x =  mat[0]*x_ + mat[4]*y_ + mat[8]*z_ - mat[12];
    double y = mat[1]*x_ + mat[5]*y_ + mat[9]*z_  - mat[13];
    double z = mat[2]*x_ + mat[6]*y_ + mat[10]*z_  - mat[14];
    
    Vector3d vec(x,y,z);
    return vec;
}


// for testing purposes
void KinectJoint::printValues() {
    this->position.printValues();
    std::printf("; confidence = %.2f\n", this->confidence);
}

std::ostream & KinectJoint::serialize(std::ostream & os) const {
    os << this->userId << " " << this->jointId << " " << this->position.getX() << " " << this->position.getY() << " " << this->position.getZ() << " " << this->confidence;
    return os;
}

std::istream & KinectJoint::deSerialize(std::istream & is) {
    double x, y, z;
    is >> this->userId >> this->jointId >> x >> y >> z >> this->confidence;
    this->position.setX(x);
    this->position.setY(y);
    this->position.setZ(z);
    return is;
}
