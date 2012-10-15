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


#include "SimpleFilter.h"

namespace tuiframework {

    SimpleFilter::SimpleFilter( int i, double n, double f )
    : nearThreshold( n ), farThreshold( f ), previous( new Vector3d[i] ), difference( new Vector3d[i] ) {}

    SimpleFilter::~SimpleFilter() {
        delete this->previous;
        delete this->difference;
    }

    bool SimpleFilter::process( int i, Vector3d v ) {
        Vector3d current = v;
        // calculate difference
        v.subtract( this->previous[i] );
        // sum up difference
        this->difference[i].add( v );
        // reset difference ( each value on its own ) if new vector is near previous
        if ( v.getX() < nearThreshold && v.getX() > -nearThreshold ) { this->difference[i].setX( 0 ); }
        if ( v.getY() < nearThreshold && v.getY() > -nearThreshold ) { this->difference[i].setY( 0 ); }
        if ( v.getZ() < nearThreshold && v.getZ() > -nearThreshold ) { this->difference[i].setZ( 0 ); }
        // test whether limit is reached
        if (	this->difference[i].getX() >= farThreshold || this->difference[i].getX() <= -farThreshold ||
                this->difference[i].getY() >= farThreshold || this->difference[i].getY() <= -farThreshold ||
                this->difference[i].getZ() >= farThreshold || this->difference[i].getZ() <= -farThreshold 	) {
            // set new previous vector
            this->previous[i] = current;
            // reset difference
            this->difference[i] = Vector3d();
            // trigger transmission
            return true;
        }
        // no transmission triggered
        return false;
    }

}
