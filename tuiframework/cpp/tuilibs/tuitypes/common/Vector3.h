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

#ifndef _tuiframework_Vector3_h_
#define _tuiframework_Vector3_h_

#include <tuiframework/core/ISerializable.h>
#include <iostream>

template <typename T>
class Vector3 : public tuiframework::ISerializable {
public:
    Vector3() {
    }
    
    
    Vector3(const Vector3<T> & v) {
        this->setData(v.getData());
    }
    
    
    inline Vector3(T x, T y, T z) {
        this->data[0] = x;
        this->data[1] = y;
        this->data[2] = z;
    }
    
    
    ~Vector3() {
    }
    
    
    inline const T * getData() const {
        return this->data;
    }
    
    
    inline void setData(const T * data) {
        for (int i = 0; i < 3; ++i) {
            this->data[i] = data[i];
        }
    }
    
    
    inline const T & operator [](int i) const {
        return this->data[i];
    }
    
    
    inline T & operator [](int i) {
        return this->data[i];
    }
    
    
    inline static T dotProduct(const Vector3<T> & a, const Vector3<T> & b) {
        return
        a[0]*b[0] +
        a[1]*b[1] +
        a[2]*b[2];
    }
    
    
    inline static Vector3<T> crossProduct(const Vector3<T> & a, const Vector3<T> & b) {
        Vector3<T> c;
        c[0] = a[1]*b[2] - a[2]*b[1];
        c[1] = a[2]*b[0] - a[0]*b[2];
        c[2] = a[0]*b[1] - a[1]*b[0];
        return c;
    }
    
    
    virtual std::ostream & serialize(std::ostream & os) const {
        os << this->data[0];
        for (int i = 1; i < 3; ++i) {
            os << " " << this->data[i];
        }
        return os;
    }
    
    
    virtual std::istream & deSerialize(std::istream & is) {
        is >> this->data[0];
        for (int i = 1; i < 3; ++i) {
            is >> this->data[i];
        }
        return is;
    }
    
private:
    T data[3];
};


template <typename T>
inline Vector3<T> operator *(const Vector3<T> & a, const Vector3<T> & b) {
    return Vector3<T>::dotProduct(a, b);
}

#endif
