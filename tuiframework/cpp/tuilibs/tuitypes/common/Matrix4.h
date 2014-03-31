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

#ifndef _tuiframework_Matrix4_h_
#define _tuiframework_Matrix4_h_

#include "Vector4.h"
#include <tuiframework/core/ISerializable.h>
#include <iostream>

template <typename T>
class Matrix4 : public tuiframework::ISerializable {
public:
    Matrix4() { }
    ~Matrix4() { }
    
    inline const T * getData() const {
        return this->data;
    }
    
    
    inline void setData(const T * data) {
        for (int i = 0; i < 16; ++i) {
            this->data[i] = data[i];
        }
    }
    
    
    inline const T * const operator [](int i) const {
        return this->data + i*4;
    }
    
    
    inline T * operator [](int i) {
        return this->data + i*4;
    }
    
    
    inline void setIdentity() {
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                (*this)[y][x] = x == y ? 1 : 0;
            }
        }
    }
    
    
    inline void setZero() {
        for (int i = 0; i < 16; ++i) {
            this->data[i] = 0;
        }
    }
    
        /// index from 0 to 3
    inline void setRow(int index, T r1, T r2, T r3, T r4) {
        this->data[index*4] = r1;
        this->data[index*4 + 1] = r2;
        this->data[index*4 + 2] = r3;
        this->data[index*4 + 3] = r4;
    }


        /// index from 0 to 3
    inline void setColumn(int index, T c1, T c2, T c3, T c4) {
        this->data[index] = c1;
        this->data[index + 4] = c2;
        this->data[index + 8] = c3;
        this->data[index + 12] = c4;
    }
    
        /// index from 0 to 3
    inline T getColumnAbs(int index, int dim) const {
        T a = 0;
        for (int i = 0; i < dim; ++i) {
            a += this->data[index + 4*i]*this->data[index + 4*i];
        }
        return static_cast<T>(sqrt(a));
    }
    
        /// index from 0 to 3, dim from 1 to 4
    inline T getRowAbs(int index, int dim) const {
        T a = 0;
        for (int i = 0; i < dim; ++i) {
            a += this->data[index*4 + i]*this->data[index*4 + i];
        }
        return static_cast<T>(sqrt(a));
    }
    
        /// index from 0 to 3, dim from 1 to 4
    inline void scaleColumnAbs(int index, int dim, T factor) {
        
        T a = getColumnAbs(index, dim);
        if (a < 0.000000001) {
            return;
        }
        
        T f = factor/a;
        for (int i = 0; i < dim; ++i) {
            this->data[index + 4*i] *= f;
        }
    }
    
        /// index from 0 to 3, dim from 1 to 4
    inline void scaleRowAbs(int index, int dim, T factor) {
        
        T a = getRowAbs(index, dim);
        if (a < 0.000000001) {
            return;
        }
        
        T f = factor/a;
        for (int i = 0; i < dim; ++i) {
            this->data[index*4 + i] *= f;
        }
    }    
    
    
    inline T getCofactor(int x, int y) const {
        int x1 = (x + 1) & 3;
        int x2 = (x + 2) & 3;
        int x3 = (x + 3) & 3;
        int y1 = (y + 1) & 3;
        int y2 = (y + 2) & 3;
        int y3 = (y + 3) & 3;
               
        T c = 
        (*this)[y1][x1]*(*this)[y2][x2]*(*this)[y3][x3] +
        (*this)[y1][x2]*(*this)[y2][x3]*(*this)[y3][x1] +
        (*this)[y1][x3]*(*this)[y2][x1]*(*this)[y3][x2] -
        (*this)[y1][x1]*(*this)[y2][x3]*(*this)[y3][x2] -
        (*this)[y1][x2]*(*this)[y2][x1]*(*this)[y3][x3] -
        (*this)[y1][x3]*(*this)[y2][x2]*(*this)[y3][x1];
    
        if ((x + y) & 1) {
            c *= -1;
        }
        
        return c;
    }
    
    
    inline Matrix4<T> getAdjoint() const {
        Matrix4<T> adjoint;
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                    // the adjoint matrix is the transposed cofactor matrix
                adjoint[x][y] = getCofactor(x, y);
            }
        }
        return adjoint;
    }
    
    
    inline T getDeterminant() const {
        Matrix4<T> adjoint = this->getAdjoint();
        T det = 0;
        int x = 0;
        for (int y = 0; y < 4; ++y) {
            det += (*this)[y][0]*adjoint[0][y];
        }
        return det;
    }
    
    
    inline bool getInverse(Matrix4<T> & out) const {
        Matrix4<T> adjoint = this->getAdjoint();
        T det = 0;
        int x = 0;
        for (int y = 0; y < 4; ++y) {
            det += (*this)[y][0]*adjoint[0][y];
        }
        
            // if the determinant is zero the matrix is not invertible
        if (det > -0.000000001 && det < 0.000000001) {
            return false;
        }
        
        T invDet = 1/det;
        
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                out[y][x] = invDet*adjoint[y][x];
            }
        }
        
        return true;
    }
    
    
    inline static T product(const T * const row, const T * const column) {
        return
        row[0]*column[0] +
        row[1]*column[4] +
        row[2]*column[8] +
        row[3]*column[12];
    }
    
    
    inline static T scalarProduct(const T * const vec1, const T * const vec2) {
        return
        vec1[0]*vec2[0] +
        vec1[1]*vec2[1] +
        vec1[2]*vec2[2] +
        vec1[3]*vec2[3];
    }
    
    
    inline static Matrix4<T> product(const Matrix4<T> & a, const Matrix4<T> & b) {
        Matrix4<T> c;
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                c[y][x] = product(a[y], &b[0][x]);
            }
        }
        return c;
    }

    
    inline static Vector4<T> product(const Vector4<T> & v, const Matrix4<T> & m) {
        Vector4<T> c;
        for (int x = 0; x < 4; ++x) {
            c[x] = product(&v[0], &m[0][x]);
        }
        return c;
    }
    
    
    inline static Vector4<T> product(const Matrix4<T> & m, const Vector4<T> & v) {
        Vector4<T> c;
        for (int y = 0; y < 4; ++y) {
            c[y] = scalarProduct(m[y], &v[0]);
        }
        return c;
    }
    
     
    inline static Matrix4<T> times(const Matrix4<T> & a, const Matrix4<T> & b) {
        Matrix4<T> c;
        for (int i = 0; i < 16; ++i) {
            c.data[i] = a.data[i]*b.data[i];
        }
        return c;
    }
    
    
    inline static Matrix4<T> transposed(const Matrix4<T> & a) {
        Matrix4<T> b;
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
    
    
    inline Matrix4<T> & operator +=(const Matrix4<T> & a) {
        for (int i = 0; i < 16; ++i) {
            this->data[i] += a.data[i];
        }
        return *this;
    }
    
    
    inline Matrix4<T> & operator -=(const Matrix4<T> & a) {
        for (int i = 0; i < 16; ++i) {
            this->data[i] -= a.data[i];
        }
        return *this;
    }
    
private:
    /// comlumn wise
    T data[16];
};


template <typename T>
inline Matrix4<T> operator *(const Matrix4<T> & a, const Matrix4<T> & b) {
    return Matrix4<T>::product(a, b);
}


template <typename T>
inline Vector4<T> operator *(const Vector4<T> & v, const Matrix4<T> & m) {
    return Matrix4<T>::product(v, m);
}


template <typename T>
inline Vector4<T> operator *(const Matrix4<T> & m, const Vector4<T> & v) {
    return Matrix4<T>::product(m, v);
}

#endif


