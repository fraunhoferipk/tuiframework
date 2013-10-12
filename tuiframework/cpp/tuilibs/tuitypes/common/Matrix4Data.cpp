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



#include "Matrix4Data.h"



Matrix4Data::Matrix4Data() {
    for (int i = 0; i < 16; ++i) {
        mat[i] = i % 5 == 0 ? 1 : 0;
    }
}

Matrix4Data::~Matrix4Data() {
}

double& Matrix4Data::operator[](int index) {
    return this->mat[index];
}

const double& Matrix4Data::operator[](int index) const {
    return this->mat[index];
}

const double * Matrix4Data::getData() const {
    return &this->mat[0];
}

/// index from 0 to 3
void Matrix4Data::setRow(int index, double r1, double r2,
        double r3, double r4) {
    this->mat[index * 4] = r1;
    this->mat[index * 4 + 1] = r2;
    this->mat[index * 4 + 2] = r3;
    this->mat[index * 4 + 3] = r4;
}

/// index from 0 to 3
void Matrix4Data::setColumn(int index, double c1, double c2,
        double c3, double c4) {
    this->mat[index] = c1;
    this->mat[index + 4] = c2;
    this->mat[index + 8] = c3;
    this->mat[index + 12] = c4;
}


void Matrix4Data::setData(const double * doubleArray) {
    for (int i = 0; i < 16; ++i) {
        this->mat[i] = doubleArray[i];
    }
}


std::ostream & Matrix4Data::serialize(std::ostream & os) const {
    os << this->mat[0];
    for (int i = 1; i < 16; ++i) {
        os << " " << this->mat[i];
    }
    return os;
}


std::istream & Matrix4Data::deSerialize(std::istream & is) {
    is >> this->mat[0];
    for (int i = 1; i < 16; ++i) {
        is >> this->mat[i];
    }
    return is;
}

