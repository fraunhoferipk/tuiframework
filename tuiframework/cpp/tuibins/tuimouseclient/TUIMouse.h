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


#ifndef _TUIMouse_h_
#define _TUIMouse_h_

#include <tuiframework/client/client.h>
#include <tuitypes/common/CommonTypeReg.h>

class TUIMouse {
public:
    TUIMouse();
    virtual ~TUIMouse(void);

    void connect();
    void disconnect();

	void leftButtonChanged(const DigitalChangedEvent * e);
	void rightButtonChanged(const DigitalChangedEvent * e);
	void middleButtonChanged(const DigitalChangedEvent * e);
	void customButton1Changed(const DigitalChangedEvent * e);
	void customButton2Changed(const DigitalChangedEvent * e);

	void deltaXChanged(const IntegerChangedEvent * e);
	void deltaYChanged(const IntegerChangedEvent * e);
	void deltaWheelChanged(const IntegerChangedEvent * e);

};

#endif
