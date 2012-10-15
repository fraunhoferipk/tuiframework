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



#ifndef _tuiframework_Vector3d_h_
#define _tuiframework_Vector3d_h_

#include <tuiframework/core/ISerializable.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <cmath>

/**
* @brief Vector class for 3 double values.
*
* Can be used as a position vector or a displacement vector.
*
* @see Used in KinectJoint
*/
class Vector3d : public tuiframework::ISerializable {
public:
    /**
    * @brief Constructor, initializes with ( 0, 0, 0 ).
    */
    Vector3d();

    /**
    * @brief Constructor, initializes with passed values.
    */
    Vector3d( double x, double y, double z );

    /**
    * @brief Destructor.
    */
    virtual ~Vector3d();

    void setX( double a );
    void setY( double a );
    void setZ( double a );

    double getX() const;
    double getY() const;
    double getZ() const;

    /**
    * @brief Adds another vector to this vector.
    * @param v vector to be added
    */
    void add( Vector3d v );

    /**
    * @brief Subtracts another vector from this vector.
    * @param v vector to be subtracted
    */
    void subtract( Vector3d v );

    /**
    * @brief Calculates cross product ( vector product, Kreuzprodukt ) of this and another vector.
    * @param v vector to calculate cross product with
    * @return cross product
    */
    Vector3d crossProduct( Vector3d v );

    /**
    * @brief Calculates dot product ( scalar product, Skalarprodukt ) of this and another vector.
    * @param v vector to calculate dot product with
    * @return dot product
    */
    double dotProduct( Vector3d v);

    /**
    * @brief Calculates absolute value ( modulus, Betrag ) of this vector.
    * @return absolute value
    */
    double absoluteValue();

    /**
    * @brief Calculates distance between two points given as vectors.
    *
    * Uses this vector as position vector for point to calculate distance to point passed as vector via parameter with.
    *
    * @param v point to calculate distance with
    * @return distance, always positive
    */
    double distance( Vector3d v );

    /**
    * @brief Calculates distance between a point and a plane given by 3 points.
    *
    * Uses this vector as position vector for point to calculate distance to a plane with.
    * Plane is given by 3 points passed via parameters.
    * Sign of result depends on order of parameters.
    *
    * @param a position vector of point A of plane
    * @param b position vector of point B of plane
    * @param c position vector of point C of plane
    * @return distance, can be negative, depending on direction of normal vector of plane
    */
    double distance( Vector3d a, Vector3d b, Vector3d c );

    void normalize();

    /**
    * @brief prints a formatted output of vector values to console, for testing purposes
    */
    void printValues();

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:

    double x;
    double y;
    double z;

};


#endif
