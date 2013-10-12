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



#ifndef _tuiframework_VectorList3d_h_
#define _tuiframework_VectorList3d_h_

#include <tuiframework/core/ISerializable.h>
#include <vector>
#include "Vector3d.h"

/**
* @brief Vector class for 3 double values.
*
* Can be used as a position vector or a displacement vector.
*
* @see Used in KinectJoint
*/
class VectorList3d : public tuiframework::ISerializable {
public:
    /**
    * @brief Constructor, initializes with ( 0, 0, 0 ).
    */
    VectorList3d();
    
    

    /**
    * @brief Constructor, initializes with passed values.
    */
    VectorList3d(const std::vector<Vector3d>& v);

    /**
    * @brief Destructor.
    */
    virtual ~VectorList3d();


    const std::vector<Vector3d>& getVectorList() const;

    /**
    * @returns number of Vector3ds in data structure
    */
    double size();


    /**
    * @brief prints a formatted output of vector values to console, for testing purposes
    */
    void printValues();
    
    /**
    * @computes difference between pairs of vectors, and places results in m_directionVectorList
    */
    void computeDirections();
    
    /**
    * @returns average of angle changes (a metric between 0 to 1: 0 is smooth, 1 is rough)
    */
    double getAverageAngleChange();

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:

    std::vector<Vector3d> m_vectorList;
    std::vector<Vector3d> m_directionVectorList; // direction vector between pairs of vectors

};


#endif
