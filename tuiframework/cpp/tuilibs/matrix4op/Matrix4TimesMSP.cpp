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


#include "Matrix4TimesMSP.h"

#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/server/ParameterGroup.h>
#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inATag = "A";
static const char * inBTag = "B";
static const char * inBPackedTag = "BP";
static const char * outABTag = "A*B";
static const char * outABPackedTag = "A*BP";

using namespace std;

namespace tuiframework {

IMSP * Matrix4TimesMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Matrix4TimesMSP(*config);
}

static std::string typeName = "Matrix4Times";

const std::string & Matrix4TimesMSP::getMSPTypeName() {
    return typeName;
}


Matrix4TimesMSP::Matrix4TimesMSP(const MSPConfig & config) :
    config(config),
    outAB(0),
    outABPacked(0) {
    
    this->eventDelegateA.setReceiver(this, &Matrix4TimesMSP::handleA);
    this->eventDelegateB.setReceiver(this, &Matrix4TimesMSP::handleB);
    this->eventDelegateBP.setReceiver(this, &Matrix4TimesMSP::handleBP);
    
    try {
        this->initMatrix(this->config.getParameterGroup().getParameterGroup("A"), this->a);
        this->initMatrix(this->config.getParameterGroup().getParameterGroup("B"), this->b);
        
        TFINFO("initial matrix A:" << this->a);
        TFINFO("initial matrix B:" << this->a);
    }
    catch (Exception & e) {
        e.addErrorMessage("in Matrix4TimesMSP.", __FILE__, __LINE__);
        TFERROR(e.getFormattedString());
    }
}


Matrix4TimesMSP::~Matrix4TimesMSP() {
}


const std::string & Matrix4TimesMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Matrix4TimesMSP::getEventSink(const std::string & name) {
    if (name.compare(inATag) == 0) {
        return &this->eventDelegateA;
    } else if (name.compare(inBTag) == 0) {
        return &this->eventDelegateB;
    } else if (name.compare(inBPackedTag) == 0) {
        return &this->eventDelegateBP;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Matrix4TimesMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outABTag) == 0) {
        this->outAB = eventSink;
    } else if (name.compare(outABPackedTag) == 0) {
        this->outABPacked = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Matrix4TimesMSP::getMSPType() const {
    return this->type;
}


void Matrix4TimesMSP::handleA(Matrix4Event * e) {    
    this->a = e->getPayload();
    if (this->outAB) {
        this->outAB->push(new Matrix4Event(-1, -1, this->a*this->b));
    }
    
    delete e;
}


void Matrix4TimesMSP::handleB(Matrix4Event * e) {    
    this->b = e->getPayload();
    if (this->outAB) {
        this->outAB->push(new Matrix4Event(-1, -1, Matrix4<double>::times(this->a, this->b)));
    }
    
    delete e;
}


void Matrix4TimesMSP::handleBP(PackedMatrix4Event * e) {    
    if (this->outABPacked) {
        const PackedType<Matrix4<double> > & p = e->getPayload();
        const vector<pair<int, Matrix4<double> > > & items = p.getItems();
        
        vector<pair<int, Matrix4<double> > >::const_iterator i = items.begin();
        vector<pair<int, Matrix4<double> > >::const_iterator e = items.end();

        PackedType<Matrix4<double> > p2;
        vector<pair<int, Matrix4<double> > > & items2 = p2.getItems();

        while (i != e) {
            items2.push_back(pair<int, Matrix4<double> >((*i).first, Matrix4<double>::times(this->a, (*i).second)));
            ++i;
        }
        
        this->outABPacked->push(new PackedMatrix4Event(-1, -1, p2));
    }
    
    delete e;
}


void Matrix4TimesMSP::initMatrix(const ParameterGroup & parameterGroup, Matrix4<double> & mat) {
    mat[0][0] = parameterGroup.getParameterGroup("row1").getDouble("col1");
    mat[0][1] = parameterGroup.getParameterGroup("row1").getDouble("col2");
    mat[0][2] = parameterGroup.getParameterGroup("row1").getDouble("col3");
    mat[0][3] = parameterGroup.getParameterGroup("row1").getDouble("col4");
    mat[1][0] = parameterGroup.getParameterGroup("row2").getDouble("col1");
    mat[1][1] = parameterGroup.getParameterGroup("row2").getDouble("col2");
    mat[1][2] = parameterGroup.getParameterGroup("row2").getDouble("col3");
    mat[1][3] = parameterGroup.getParameterGroup("row2").getDouble("col4");
    mat[2][0] = parameterGroup.getParameterGroup("row3").getDouble("col1");
    mat[2][1] = parameterGroup.getParameterGroup("row3").getDouble("col2");
    mat[2][2] = parameterGroup.getParameterGroup("row3").getDouble("col3");
    mat[2][3] = parameterGroup.getParameterGroup("row3").getDouble("col4");
    mat[3][0] = parameterGroup.getParameterGroup("row4").getDouble("col1");
    mat[3][1] = parameterGroup.getParameterGroup("row4").getDouble("col2");
    mat[3][2] = parameterGroup.getParameterGroup("row4").getDouble("col3");
    mat[3][3] = parameterGroup.getParameterGroup("row4").getDouble("col4");
}


}


