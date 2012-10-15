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



#include "Matrix4x4TransfUnit.h"
#include <tuiframework/logging/Logger.h>

#include <sstream>

using namespace std;

namespace tuiframework {


Matrix4x4TransfUnit::Matrix4x4TransfUnit() :
        eventSink(0) {
}


Matrix4x4TransfUnit::~Matrix4x4TransfUnit() {
}


void Matrix4x4TransfUnit::setParameterGroup(const ParameterGroup & parameterGroup) {

    this->transfMatrix[0][0] = parameterGroup.getParameterGroup("row1").getDouble("col1");
    this->transfMatrix[0][1] = parameterGroup.getParameterGroup("row1").getDouble("col2");
    this->transfMatrix[0][2] = parameterGroup.getParameterGroup("row1").getDouble("col3");
    this->transfMatrix[0][3] = parameterGroup.getParameterGroup("row1").getDouble("col4");
    this->transfMatrix[1][0] = parameterGroup.getParameterGroup("row2").getDouble("col1");
    this->transfMatrix[1][1] = parameterGroup.getParameterGroup("row2").getDouble("col2");
    this->transfMatrix[1][2] = parameterGroup.getParameterGroup("row2").getDouble("col3");
    this->transfMatrix[1][3] = parameterGroup.getParameterGroup("row2").getDouble("col4");
    this->transfMatrix[2][0] = parameterGroup.getParameterGroup("row3").getDouble("col1");
    this->transfMatrix[2][1] = parameterGroup.getParameterGroup("row3").getDouble("col2");
    this->transfMatrix[2][2] = parameterGroup.getParameterGroup("row3").getDouble("col3");
    this->transfMatrix[2][3] = parameterGroup.getParameterGroup("row3").getDouble("col4");
    this->transfMatrix[3][0] = parameterGroup.getParameterGroup("row4").getDouble("col1");
    this->transfMatrix[3][1] = parameterGroup.getParameterGroup("row4").getDouble("col2");
    this->transfMatrix[3][2] = parameterGroup.getParameterGroup("row4").getDouble("col3");
    this->transfMatrix[3][3] = parameterGroup.getParameterGroup("row4").getDouble("col4");

    TFINFO("Transformation matrix: " << this->transfMatrix);
}


void Matrix4x4TransfUnit::setEventSink(IEventSink * eventSink) {
    this->eventSink = eventSink;
}


void Matrix4x4TransfUnit::push(IEvent * event) {
    Matrix4x4ChangedEvent * e = static_cast<Matrix4x4ChangedEvent *>(event);
    if (this->right) {
        e->setPayload(this->transfMatrix*e->getPayload());
    } else {
        e->setPayload(e->getPayload()*this->transfMatrix);
    }
    this->eventSink->push(event);
}


}
