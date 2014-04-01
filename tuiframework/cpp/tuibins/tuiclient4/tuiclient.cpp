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


#include "Init.h"

#include <tuiframework/client/client.h>
#include <tuitypes/common/CommonTypeReg.h>

#include <iostream>
#include <stdlib.h>
#include <cstdio>

using namespace tuiframework;
using namespace std;


int main(int argc, char* argv[]) {
    
    Matrix4<double> a;
    a.setRow(0, -0.844886, 0.038559, -0.533554, 0);
    a.setRow(1, 0.196147, 0.950262, -0.241927, 0);
    a.setRow(2, 0.497688, -0.309056, -0.810427, 0);
    a.setRow(3, 804.377, -719.626, -775.713, 1);
    
    Matrix4<double> b;
    b.setRow(0, 0.884075, -0.0320818, 0.510251, 0);
    b.setRow(1, 0.0781636, 0.997974, -0.227442, 0);
    b.setRow(2, -0.510415, 0.372416, 0.811975, 0);
    b.setRow(3, 860.67, -721.87, -772.464, 1);
    
    Matrix4<double> ainv;
    a.getInverse(ainv);
    //cout << ainv << endl;
    /*
    Matrix4<double> c = ainv*b;
    cout << c << endl;
    */
    Matrix4<double> d = b*ainv;
    cout << d << endl;
    
    Matrix4<double> a2;
    a2.setRow(0, -0.816012, 0.063607, 0.574524, 0);
    a2.setRow(1, -0.082659, 0.970872, -0.224891, 0);
    a2.setRow(2, -0.572094, -0.231003, -0.786986, 0);
    a2.setRow(3, -418.667, -719.608, -736.742, 1);

    Matrix4<double> a2inv;
    a2.getInverse(a2inv);
   // cout << a2inv << endl;
    
    Matrix4<double> b2;
    b2.setRow(0, 0.902015, -0.0556646, -0.482022, 0);
    b2.setRow(1, 0.00238735, 0.894371, -0.229272, 0);
    b2.setRow(2, 0.488171, 0.374459, 0.76482, 0);
    b2.setRow(3, -403.955, -690.101, -717.646, 1);
    
    Matrix4<double> d2 = b2*a2inv;
    cout << d2 << endl;
    
    Matrix4<double> a3;
    a3.setRow(0, -0.997648, 0.048466, 0.048473, 0);
    a3.setRow(1, 0.038117, 0.979991, -0.195358, 0);
    a3.setRow(2, -0.056972, -0.193051, -0.979533, 0);
    a3.setRow(3, -78.874, -721.05, -730.586, 1);
    
    Matrix4<double> a3inv;
    a3.getInverse(a3inv);
    
    Matrix4<double> b3;
    b3.setRow(0, 1.04049, 0.00121223, 0.0140528, 0);
    b3.setRow(1, -0.0104498, 0.961271, -0.220335, 0);
    b3.setRow(2, 0.0593426, 0.362561, 0.890188, 0);
    b3.setRow(3, -104.067 ,-700.747, -656.883, 1);
    
    Matrix4<double> d3 = b3*a3inv;
    cout << d3 << endl;
    
    Matrix4<double> e3 = a3*d3;
    cout << e3 << endl;
    
    Matrix4<double> f3 = d3*a2;
    cout << f3 << endl;
    
    if (argc != 4) {
        cout << "Usage tuiclient1 <sender port> <receiver port> <ipaddress:port>" << endl;
        return 1;
    }

    Init init;
    initTypeRegistration(getEventFactory());
    CommonTypeReg::registerTypes(&getEventFactory(), &getEventChannelFactory());

    getchar();

    connectWithTUIServer(atoi(argv[1]), atoi(argv[2]), argv[3], &init);
    return 0;
}
