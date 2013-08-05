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



#ifndef _tuiframework_TrackerData_h_
#define _tuiframework_TrackerData_h_

#include <tuiframework/core/ISerializable.h>
#include <iosfwd>

class TrackerData : public tuiframework::ISerializable {
public:
    TrackerData();
    virtual ~TrackerData();
    
    double getPos(int index) const;
    void setPos(int index, double value);
    
    double getQuat(int index) const;
    void setQuat(int index, double value);
    
    void getPos(double pos[3]) const;
    void setPos(double pos[3]);
    
    void getQuat(double quat[4]) const;
    void setQuat(double quat[4]);
    
    void setPos(double p1, double p2, double p3);
    void setQuat(double q1, double q2, double q3, double q4);
    
    virtual std::ostream & serialize(std::ostream & os) const;
    virtual std::istream & deSerialize(std::istream & is);

protected:
    double pos[3];
    double quat[4];
};

#endif
