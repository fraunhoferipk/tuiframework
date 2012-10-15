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



#ifndef _tuiframework_Matrix4x4_h_
#define _tuiframework_Matrix4x4_h_

#include <tuiframework/core/ISerializable.h>
#include <iostream>

template <typename T>
class Matrix4x4 : public tuiframework::ISerializable {
public:
    Matrix4x4() { }
    ~Matrix4x4() { }
    
    const T * getData() const {
        return this->data;
    }


    void setData(const T * data) {
        for (int i = 0; i < 16; ++i) {
            this->data[i] = data[i];
        }
    }


    const T * const operator [](int i) const {
        return this->data + i*4;
    }


    T * operator [](int i) {
        return this->data + i*4;
    }


    static T product(const T * const row, const T * const column) {
        return
            row[0]*column[0] +
            row[1]*column[4] +
            row[2]*column[8] +
            row[3]*column[12];
    }


    static Matrix4x4<T> product(const Matrix4x4<T> & a, const Matrix4x4<T> & b) {
        Matrix4x4 c;
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                c[y][x] = product(a[y], &b[0][x]);
            }
        }
        return c;
    }


    static Matrix4x4<T> transposed(const Matrix4x4<T> & a) {
        Matrix4x4<T> b;
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                b[y][x] = a[x][y];
            }
        }
        return b;
    }


    virtual std::ostream & serialize(std::ostream & os) const {
        os << this->data[0];
        for (int i = 1; i < 16; ++i) {
            os << " " << this->data[i];
        }
        return os;
    }


    virtual std::istream & deSerialize(std::istream & is) {
        is >> this->data[0];
        for (int i = 1; i < 16; ++i) {
            is >> this->data[i];
        }
        return is;
    }

private:
        /// comlumn wise
    T data[16];
};


template <typename T>
Matrix4x4<T> operator *(const Matrix4x4<T> & a, const Matrix4x4<T> & b) {
    return Matrix4x4<T>::product(a, b);
}


#endif


