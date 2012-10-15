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



#ifndef _tuiframework_KinectJoint_h_
#define _tuiframework_KinectJoint_h_

#include <tuiframework/core/ISerializable.h>
#include <iostream>
#include <cstdio>
#include "Matrix4Data.h"
#include "Vector3d.h"

/**
* @brief Class for joint data from user skeleton as output by Kinect.
* @see tuidevices::KinectDevice
*/
class KinectJoint : public tuiframework::ISerializable {
public:
    enum SkeletonJoint
    {
    SKEL_HEAD			= 1,
    SKEL_NECK			= 2,
    SKEL_TORSO			= 3,
    SKEL_WAIST			= 4,

    SKEL_LEFT_COLLAR	= 5,
    SKEL_LEFT_SHOULDER	= 6,
    SKEL_LEFT_ELBOW		= 7,
    SKEL_LEFT_WRIST		= 8,
    SKEL_LEFT_HAND		= 9,
    SKEL_LEFT_FINGERTIP	=10,

    SKEL_RIGHT_COLLAR	=11,
    SKEL_RIGHT_SHOULDER	=12,
    SKEL_RIGHT_ELBOW	=13,
    SKEL_RIGHT_WRIST	=14,
    SKEL_RIGHT_HAND		=15,
    SKEL_RIGHT_FINGERTIP=16,

    SKEL_LEFT_HIP		=17,
    SKEL_LEFT_KNEE		=18,
    SKEL_LEFT_ANKLE		=19,
    SKEL_LEFT_FOOT		=20,

    SKEL_RIGHT_HIP		=21,
    SKEL_RIGHT_KNEE		=22,
    SKEL_RIGHT_ANKLE	=23,
    SKEL_RIGHT_FOOT		=24
    };

    /**
    * @brief Constructor, initializes with ( 1, 0, 0, ( 0, 0, 0 ) ).
    */
    KinectJoint();

    /**
    * @brief Constructor, initializes with passed values.
    */
    KinectJoint( int, int, double, double, double, double );

    /**
    * @brief Destructor.
    */
    virtual ~KinectJoint();
    
    int getJointId() const;
    int getUserId() const;
    void setConfidence (double);
    double getConfidence() const;
    Vector3d getPosition() const;
    Vector3d getTransformedPosition(const Matrix4Data &);

    /**
    * @brief prints a formatted output of vector values to console, for testing purposes
    */
    void printValues();

    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:

    /**
    * @brief Number, identifying which user the Joint belongs to.
    */
    int userId;

    /**
    * @brief Number, identifying which joint of the skeleton this is.
    * @see See tuidevices::KinectDevice for mapping of numbers to joints.
    */
    int jointId;

    /**
    * @brief States how reliable the joint was be tracked.
    */
    double confidence;

    /**
    * @brief 3-dimensional position of joint.
    */
    Vector3d position;
};

#endif

