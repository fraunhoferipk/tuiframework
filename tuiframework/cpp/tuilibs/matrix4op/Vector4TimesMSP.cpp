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


#include "Vector4TimesMSP.h"

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

IMSP * Vector4TimesMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Vector4TimesMSP(*config);
}

static std::string typeName = "Vector4Times";

const std::string & Vector4TimesMSP::getMSPTypeName() {
    return typeName;
}


Vector4TimesMSP::Vector4TimesMSP(const MSPConfig & config) :
    config(config),
    outAB(0),
    outABPacked(0) {
    
    this->eventDelegateA.setReceiver(this, &Vector4TimesMSP::handleA);
    this->eventDelegateB.setReceiver(this, &Vector4TimesMSP::handleB);
    this->eventDelegateBP.setReceiver(this, &Vector4TimesMSP::handleBP);
    
    try {
        this->initVector(this->config.getParameterGroup().getParameterGroup("A"), this->a);
        this->initVector(this->config.getParameterGroup().getParameterGroup("B"), this->b);
        
        TFINFO("initial vector A:" << this->a);
        TFINFO("initial vector B:" << this->a);
    }
    catch (Exception & e) {
        e.addErrorMessage("in Vector4TimesMSP.", __FILE__, __LINE__);
        TFERROR(e.getFormattedString());
    }
}


Vector4TimesMSP::~Vector4TimesMSP() {
}


const std::string & Vector4TimesMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Vector4TimesMSP::getEventSink(const std::string & name) {
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


void Vector4TimesMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outABTag) == 0) {
        this->outAB = eventSink;
    } else if (name.compare(outABPackedTag) == 0) {
        this->outABPacked = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Vector4TimesMSP::getMSPType() const {
    return this->type;
}


void Vector4TimesMSP::handleA(Vector4Event * e) {    
    this->a = e->getPayload();
    if (this->outAB) {
        this->outAB->push(new Vector4Event(-1, -1, Vector4<double>::times(this->a, this->b)));
    }
    
    delete e;
}


void Vector4TimesMSP::handleB(Vector4Event * e) {    
    this->b = e->getPayload();
    if (this->outAB) {
        this->outAB->push(new Vector4Event(-1, -1, Vector4<double>::times(this->a, this->b)));
    }
    
    delete e;
}


void Vector4TimesMSP::handleBP(PackedVector4Event * e) {    
    if (this->outABPacked) {
        const PackedType<Vector4<double> > & p = e->getPayload();
        const vector<pair<int, Vector4<double> > > & items = p.getItems();
        
        vector<pair<int, Vector4<double> > >::const_iterator i = items.begin();
        vector<pair<int, Vector4<double> > >::const_iterator e = items.end();

        PackedType<Vector4<double> > p2;
        vector<pair<int, Vector4<double> > > & items2 = p2.getItems();

        while (i != e) {
            items2.push_back(pair<int, Vector4<double> >((*i).first, Vector4<double>::times(this->a, (*i).second)));
            ++i;
        }
        
        this->outABPacked->push(new PackedVector4Event(-1, -1, p2));
    }
    
    delete e;
}


void Vector4TimesMSP::initVector(const ParameterGroup & parameterGroup, Vector4<double> & vec) {
    vec[0] = parameterGroup.getDouble("x");
    vec[1] = parameterGroup.getDouble("y");
    vec[2] = parameterGroup.getDouble("z");
    vec[3] = parameterGroup.getDouble("w");
}


}


