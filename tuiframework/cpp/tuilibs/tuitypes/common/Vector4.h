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

#ifndef _tuiframework_Vector4_h_
#define _tuiframework_Vector4_h_

#include <tuiframework/core/ISerializable.h>
#include <iostream>

template <typename T>
class Vector4 : public tuiframework::ISerializable {
public:
    Vector4() {
        this->data[0] = 0;
        this->data[1] = 0;
        this->data[2] = 0;
        this->data[3] = 1.0;
    }
    
    Vector4(const Vector4<T> & v) {
        this->setData(v.getData());
    }
    
    inline Vector4(T x, T y, T z) {
        this->data[0] = x;
        this->data[1] = y;
        this->data[2] = z;
        this->data[3] = 1.0;
    }
    
    inline Vector4(T x, T y, T z, T w) {
        this->data[0] = x;
        this->data[1] = y;
        this->data[2] = z;
        this->data[3] = w;
    }
    
    ~Vector4() {
    }
    
    inline const T * getData() const {
        return this->data;
    }
    
    inline void setData(const T * data) {
        for (int i = 0; i < 4; ++i) {
            this->data[i] = data[i];
        }
    }
    
    inline void set(T x, T y, T z, T w) {
        this->data[0] = x;
        this->data[1] = y;
        this->data[2] = z;
        this->data[3] = w;
    }
    
    inline const T & operator [](int i) const {
        return this->data[i];
    }
    
    inline T & operator [](int i) {
        return this->data[i];
    }
    
    inline static T dotProduct(const Vector4<T> & a, const Vector4<T> & b) {
        return (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])*a[3]*b[3];
    }
    
    inline static Vector4<T> scale(const T & a, const Vector4<T> & b) {
        return Vector4<T>(a*b[0], a*b[1], a*b[2]);
    }
    
    inline void scale(T factor) {
        for (int i = 0; i < 3; ++i) {
            this->data[i] *= factor;
        }
    }
    
    inline static Vector4<T> crossProduct(const Vector4<T> & a, const Vector4<T> & b) {
        Vector4<T> c;
        c[0] = a[1]*b[2] - a[2]*b[1];
        c[1] = a[2]*b[0] - a[0]*b[2];
        c[2] = a[0]*b[1] - a[1]*b[0];
        c[3] = a[3]*b[3];
        return c;
    }
    
    inline static Vector4<T> times(const Vector4<T> & a, const Vector4<T> & b) {
        Vector4<T> c;
        for (int i = 0; i < 4; ++i) {
            c.data[i] = a.data[i]*b.data[i];
        }
        return c;
    }

    inline Vector4<T> & operator +=(const Vector4<T> & a) {
        for (int i = 0; i < 3; ++i) {
            this->data[i] += a.data[i]*a[3]/this->data[3];
        }
        return *this;
    }
    
    inline Vector4<T> & operator -=(const Vector4<T> & a) {
        for (int i = 0; i < 3; ++i) {
            this->data[i] -= a.data[i]*a[3]/this->data[3];
        }
        return *this;
    }
    
    inline T getAbs() const {
        return static_cast<T>(sqrt(this->dotProduct(*this, *this)));
    }
    
    inline void scaleToAbs(T factor) {
        T a = getAbs();
        if (a < 0.000000001) {
            return;
        }
        
        T f = factor/a;
        for (int i = 0; i < 3; ++i) {
            this->data[i] *= f;
        }
    }
    
    inline void normalize() {
        for (int i = 0; i < 3; ++i) {
            this->data[i] *= this->data[3];
        }
        this->data[3] = 1;
    }
    
    virtual std::ostream & serialize(std::ostream & os) const {
        os << this->data[0];
        for (int i = 1; i < 4; ++i) {
            os << " " << this->data[i];
        }
        return os;
    }
    
    virtual std::istream & deSerialize(std::istream & is) {
        is >> this->data[0];
        for (int i = 1; i < 4; ++i) {
            is >> this->data[i];
        }
        return is;
    }
    
protected:
    T data[4];
};


template <typename T>
inline T operator *(const Vector4<T> & a, const Vector4<T> & b) {
    return Vector4<T>::dotProduct(a, b);
}


template <typename T>
inline Vector4<T> operator *(const T & a, const Vector4<T> & b) {
    return Vector4<T>::scale(a, b);
}


template <typename T>
inline Vector4<T> operator *(const Vector4<T> & b, const T & a) {
    return Vector4<T>::scale(a, b);
}


template <typename T>
inline Vector4<T> operator +(const Vector4<T> & a, const Vector4<T> & b) {
    Vector4<T> c;
    c[0] = a[0] + b[0];
    c[1] = a[1] + b[1];
    c[2] = a[2] + b[2];
    return c;
}


template <typename T>
inline Vector4<T> operator -(const Vector4<T> & a, const Vector4<T> & b) {
    Vector4<T> c;
    c[0] = a[0] - b[0];
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];
    return c;
}



#endif
