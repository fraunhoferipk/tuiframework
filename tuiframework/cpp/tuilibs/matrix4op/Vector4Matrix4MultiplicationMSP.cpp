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


#include "Vector4Matrix4MultiplicationMSP.h"

#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/server/ParameterGroup.h>
#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inVTag = "V";
static const char * inVPackedTag = "VP";
static const char * inMTag = "M";
static const char * inMPackedTag = "MP";
static const char * outVMTag = "V*M";
static const char * outMVTag = "M*V";
static const char * outVPackedMTag = "VP*M";
static const char * outMVPackedTag = "M*VP";
static const char * outMPackedVTag = "MP*V";
static const char * outVMPackedTag = "V*MP";

using namespace std;

namespace tuiframework {

IMSP * Vector4Matrix4MultiplicationMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Vector4Matrix4MultiplicationMSP(*config);
}

static std::string typeName = "Vector4Matrix4Multiplication";

const std::string & Vector4Matrix4MultiplicationMSP::getMSPTypeName() {
    return typeName;
}


Vector4Matrix4MultiplicationMSP::Vector4Matrix4MultiplicationMSP(const MSPConfig & config) :
    config(config),
    outVM(0),
    outMV(0),
    outVPackedM(0),
    outMVPacked(0),
    outMPackedV(0),
    outVMPacked(0) {
    
    this->eventDelegateV.setReceiver(this, &Vector4Matrix4MultiplicationMSP::handleV);
    this->eventDelegateM.setReceiver(this, &Vector4Matrix4MultiplicationMSP::handleM);
    this->eventDelegateVP.setReceiver(this, &Vector4Matrix4MultiplicationMSP::handleVP);
    this->eventDelegateMP.setReceiver(this, &Vector4Matrix4MultiplicationMSP::handleMP);
    
    try {
        this->initVector(this->config.getParameterGroup().getParameterGroup("V"), this->v);
        this->initMatrix(this->config.getParameterGroup().getParameterGroup("M"), this->m);
        TFINFO("initial vector V:" << this->v);
        TFINFO("initial matrix M:" << this->m);
        this->triggerM = this->config.getParameterGroup().getParameterGroup("multiply").getInt("triggerM") != 0;
        this->triggerV = this->config.getParameterGroup().getParameterGroup("multiply").getInt("triggerV") != 0;
        TFINFO("triggerM = " << this->triggerM);
        TFINFO("triggerV = " << this->triggerV);        
    }
    catch (Exception & e) {
        e.addErrorMessage("in Vector4Matrix4MultiplicationMSP.", __FILE__, __LINE__);
        TFERROR(e.getFormattedString());
    }
}


Vector4Matrix4MultiplicationMSP::~Vector4Matrix4MultiplicationMSP() {
}


const std::string & Vector4Matrix4MultiplicationMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Vector4Matrix4MultiplicationMSP::getEventSink(const std::string & name) {
    if (name.compare(inVTag) == 0) {
        return &this->eventDelegateV;
    } else if (name.compare(inMTag) == 0) {
        return &this->eventDelegateM;
    } else if (name.compare(inVPackedTag) == 0) {
        return &this->eventDelegateVP;
    } else if (name.compare(inMPackedTag) == 0) {
        return &this->eventDelegateMP;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Vector4Matrix4MultiplicationMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outVMTag) == 0) {
        this->outVM = eventSink;
    } else if (name.compare(outMVTag) == 0) {
        this->outMV = eventSink;
    } else if (name.compare(outVPackedMTag) == 0) {
        this->outVPackedM = eventSink;
    } else if (name.compare(outMVPackedTag) == 0) {
        this->outMVPacked = eventSink;
    } else if (name.compare(outMPackedVTag) == 0) {
        this->outMPackedV = eventSink;
    } else if (name.compare(outVMPackedTag) == 0) {
        this->outVMPacked = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Vector4Matrix4MultiplicationMSP::getMSPType() const {
    return this->type;
}


void Vector4Matrix4MultiplicationMSP::handleV(Vector4Event * e) {    
    this->v = e->getPayload();
    if (this->outVM && this->triggerV) {
        this->outVM->push(new Vector4Event(-1, -1, this->v*this->m));
    }
    if (this->outMV && this->triggerV) {
        this->outMV->push(new Vector4Event(-1, -1, this->m*this->v));
    }
    delete e;
}


void Vector4Matrix4MultiplicationMSP::handleM(Matrix4Event * e) {    
    this->m = e->getPayload();
    if (this->outVM && this->triggerM) {
        this->outVM->push(new Vector4Event(-1, -1, this->v*this->m));
    }
    if (this->outMV && this->triggerM) {
        this->outMV->push(new Vector4Event(-1, -1, this->m*this->v));
    }
    delete e;
}


void Vector4Matrix4MultiplicationMSP::handleVP(PackedVector4Event * e) {

    if (this->outVPackedM || this->outMPackedV) {
        const PackedType<Vector4<double> > & p = e->getPayload();
        const vector<pair<int, Vector4<double> > > & items = p.getItems();
        
        vector<pair<int, Vector4<double> > >::const_iterator i;
        vector<pair<int, Vector4<double> > >::const_iterator e;

        PackedType<Vector4<double> > p2;
        vector<pair<int, Vector4<double> > > & items2 = p2.getItems();
        if (this->outVPackedM) {
            i = items.begin();
            e = items.end();
            while (i != e) {
                items2.push_back(pair<int, Vector4<double> >((*i).first, (*i).second*this->m));
                ++i;
            }
            this->outVPackedM->push(new PackedVector4Event(-1, -1, p2));
            items2.clear();
        }

        if (this->outMPackedV) {
            i = items.begin();
            e = items.end();
            while (i != e) {
                items2.push_back(pair<int, Vector4<double> >((*i).first, this->m*(*i).second));
                ++i;
            }
            this->outMPackedV->push(new PackedVector4Event(-1, -1, p2));
        }
        
    }

    delete e;
}


void Vector4Matrix4MultiplicationMSP::handleMP(PackedMatrix4Event * e) {
    
      if (this->outVPackedM || this->outMPackedV) {
        const PackedType<Matrix4<double> > & p = e->getPayload();
        const vector<pair<int, Matrix4<double> > > & items = p.getItems();
        
        vector<pair<int, Matrix4<double> > >::const_iterator i = items.begin();
        vector<pair<int, Matrix4<double> > >::const_iterator e = items.end();

        PackedType<Vector4<double> > p2;
        vector<pair<int, Vector4<double> > > & items2 = p2.getItems();
        
        
        if (this->outVPackedM) {
            i = items.begin();
            e = items.end();
            while (i != e) {
                items2.push_back(pair<int, Vector4<double> >((*i).first, this->v*(*i).second));
                ++i;
            }
            this->outVPackedM->push(new PackedVector4Event(-1, -1, p2));
            items2.clear();
        }

        if (this->outMPackedV) {
            i = items.begin();
            e = items.end();
            while (i != e) {
                items2.push_back(pair<int, Vector4<double> >((*i).first, (*i).second*this->v));
                ++i;
            }
            this->outMPackedV->push(new PackedVector4Event(-1, -1, p2));
        }
    }
    
    delete e;
}


void Vector4Matrix4MultiplicationMSP::initMatrix(const ParameterGroup & parameterGroup, Matrix4<double> & mat) {
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


void Vector4Matrix4MultiplicationMSP::initVector(const ParameterGroup & parameterGroup, Vector4<double> & vec) {
    vec[0] = parameterGroup.getDouble("x");
    vec[1] = parameterGroup.getDouble("y");
    vec[2] = parameterGroup.getDouble("z");
    vec[3] = parameterGroup.getDouble("w");
}


}


