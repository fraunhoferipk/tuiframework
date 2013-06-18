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

#ifndef _tuiframework_SimpleFilter_h_
#define _tuiframework_SimpleFilter_h_

#include <tuilibs/tuitypes/common/CommonTypeReg.h>

/**
 * @brief A filter reducing the output of a Vector3d channel, for example KinectJoint positions.
 *
 * <ul><li>
 * To reduce output, vectors with small differences should not be send. Therefore the limit <tt>farThreshold</tt> is set.
 * A new vector is transmitted only if the difference to the last transmitted vector is greater than the threshold.
 * </li><li>
 * Differences which are small, but steady should be transmitted nevertheless. Therefore differences are added up.
 * When the sum reaches the threshold a new vector is transmitted.
 * </li><li>
 * But if differences occur only casually, they should not sum up.
 * Therefore the sum is resetted if the last transmitted vector occurs again.
 * In which scope a vector is acknowledge to be the same, is limited by <tt>nearThreshold</tt>.
 * </li><li>
 * A whole new vector will be transmitted if one of its values triggers a new transmission.
 * </li><li>
 * One filter can handle several channels and memorizes the previous vector and the sum for each of them.
 * </li></ul>
 */
class SimpleFilter {

public:

	/**
	 * @brief Constructor for a filter
	 *
	 * When used with the Kinect, the following values are recommended:<br />
	 * <tt>nearThreshold = 0.005</tt>,
	 * <tt>farThreshold = 0.05</tt>.
	 *
	 * @param i number of channels to filter
	 * @param n nearThreshold, see description
	 * @param f farThreshold, see description
	 */
	SimpleFilter( int i, double n, double f );

	/**
	 * @brief Destructor
	 */
	virtual ~SimpleFilter();

	/**
	 * @brief Determines if a vector should be transmitted
	 *
	 * @param i number of the channel
	 * @param v vector proposed for transmission
	 * @return true or false, whether vector has to be transmitted
	 */
	bool process( int i, Vector3d v );

protected:

	/**
	 * @brief limits in which scope two values are treated like being the same
	 */
	double nearThreshold;

	/**
	 * @brief limit from which two values are apart enough to trigger transmission
	 */
	double farThreshold;

	/**
	 * @brief last transmitted vector for comparison
	 */
	Vector3d* previous;

	/**
	 * @brief summarized difference
	 */
	Vector3d* difference;

};

#endif
