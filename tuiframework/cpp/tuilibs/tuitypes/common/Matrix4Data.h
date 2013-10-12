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



#ifndef _tuiframework_Matrix4Data_h_
#define _tuiframework_Matrix4Data_h_

#include <tuiframework/core/ISerializable.h>
#include <iostream>

class Matrix4Data : public tuiframework::ISerializable {
public:
    Matrix4Data();
    virtual ~Matrix4Data();
    
    double & operator[](int index);
    const double & operator[](int index) const;
    const double * getData() const;    

    void setRow(int index, double r1, double r2, double r3, double r4);
    void setColumn(int index, double c1, double c2, double c3, double c4);
    void setData(const double * doubleArray);
    
    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
    /// column wise
    double mat[16];
};


#endif

