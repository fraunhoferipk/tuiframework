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


#include "Matrix4TransfMSP.h"

#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/server/ParameterGroup.h>
#include <tuiframework/core/Exception.h>
#include <tuiframework/logging/Logger.h>

static const char * inTag = "in";
static const char * outTag = "out";

using namespace std;

namespace tuiframework {

IMSP * Matrix4TransfMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Matrix4TransfMSP(*config);
}

static std::string typeName = "Matrix4TransfMSP";

const std::string & Matrix4TransfMSP::getMSPTypeName() {
    return typeName;
}


Matrix4TransfMSP::Matrix4TransfMSP(const MSPConfig & config) :
    config(config),
    out(0),
    left(true) {
    
    this->maxtrix4ChangedEventDelegate.setReceiver(this, &Matrix4TransfMSP::handleMatrix4ChangedEvent);
    
    try {
        const ParameterGroup & parameterGroup = this->config.getParameterGroup().getParameterGroup("transf");
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
    catch (Exception & e) {
        e.addErrorMessage("in Matrix4TransfMSP.", __FILE__, __LINE__);
        TFERROR(e.getFormattedString());
    }
}


Matrix4TransfMSP::~Matrix4TransfMSP() {
}


const std::string & Matrix4TransfMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Matrix4TransfMSP::getEventSink(const std::string & name) {
    if (name.compare(inTag) == 0) {
        return &maxtrix4ChangedEventDelegate;
    }
    return 0;
}


void Matrix4TransfMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if ( ! name.compare(outTag)) {
        this->out = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Matrix4TransfMSP::getMSPType() const {
    return this->type;
}


void Matrix4TransfMSP::handleMatrix4ChangedEvent(Matrix4ChangedEvent * e) {
    if ( ! this->out) {
        return;
    }
    
    const Matrix4Data & mdIn = e->getPayload();
    
    Matrix4Data mdOut;
    Matrix4x4<double> a;
    a.setData(mdIn.getData());
    if (this->left) {
       TFINFO("a = " << a)
       TFINFO("transfMastrix = " << this->transfMatrix)
       Matrix4x4<double> b = Matrix4x4<double>::product(this->transfMatrix, a);
       TFINFO("b = " << b)
       mdOut.setData(b.getData());
    } else {
       mdOut.setData(Matrix4x4<double>::product(this->transfMatrix, a).getData());
    }
    TFINFO("mdOut = " << mdOut)
    this->out->push(new Matrix4ChangedEvent(-1, -1, mdOut));
    
    delete e;
}


}
