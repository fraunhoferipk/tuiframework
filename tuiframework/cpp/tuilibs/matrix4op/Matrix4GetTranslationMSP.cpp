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


#include "Matrix4GetTranslationMSP.h"

#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/server/ParameterGroup.h>
#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inTag = "in";
static const char * inPackedTag = "inPacked";
static const char * outTag = "out";
static const char * outPackedTag = "outPacked";

using namespace std;

namespace tuiframework {

IMSP * Matrix4GetTranslationMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Matrix4GetTranslationMSP(*config);
}

static std::string typeName = "Matrix4GetTranslation";

const std::string & Matrix4GetTranslationMSP::getMSPTypeName() {
    return typeName;
}


Matrix4GetTranslationMSP::Matrix4GetTranslationMSP(const MSPConfig & config) :
    config(config),
    out(0),
    outPacked(0) {
    
    this->eventDelegateM.setReceiver(this, &Matrix4GetTranslationMSP::handleM);
    this->eventDelegateMP.setReceiver(this, &Matrix4GetTranslationMSP::handleMP);
    
    try {
        this->premultiply = this->config.getParameterGroup().getParameterGroup("create").getInt("pre-multiply") != 0;
        TFINFO("pre-multiply = " << this->premultiply);
    }
    catch (Exception & e) {
        e.addErrorMessage("in Matrix4GetTranslationMSP.", __FILE__, __LINE__);
        TFERROR(e.getFormattedString());
    }
}


Matrix4GetTranslationMSP::~Matrix4GetTranslationMSP() {
}


const std::string & Matrix4GetTranslationMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Matrix4GetTranslationMSP::getEventSink(const std::string & name) {
    if (name.compare(inTag) == 0) {
        return &this->eventDelegateM;
    } else if (name.compare(inPackedTag) == 0) {
        return &this->eventDelegateMP;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Matrix4GetTranslationMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outTag) == 0) {
        this->out = eventSink;
    } else if (name.compare(outPackedTag) == 0) {
        this->outPacked = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Matrix4GetTranslationMSP::getMSPType() const {
    return this->type;
}


void Matrix4GetTranslationMSP::handleM(Matrix4Event * e) {
    if (this->out) {
        const Matrix4<double> & m = e->getPayload();
        Vector4<double> v;
        if (this->premultiply) {
            v.set(m[0][3], m[1][3], m[2][3], m[3][3]);
        } else {
            v.set(m[3][0], m[3][1], m[3][2], m[3][3]);
        }
        
        this->out->push(new Vector4Event(-1, -1, v));
    }
    delete e;
}


void Matrix4GetTranslationMSP::handleMP(PackedMatrix4Event * e) {
    if (this->outPacked) {
        const PackedType<Matrix4<double> > & p = e->getPayload();
        const vector<pair<int, Matrix4<double> > > & mv = p.getItems();
        
        PackedType<Vector4<double> > packedVector4;
        vector<pair<int, Vector4<double> > > & vv = packedVector4.getItems();
        
        vector<pair<int, Matrix4<double> > >::const_iterator i = mv.begin();
        vector<pair<int, Matrix4<double> > >::const_iterator e = mv.end();
        while (i != e) {
            Vector4<double> v;
            if (this->premultiply) {
                v.set((*i).second[0][3], (*i).second[1][3], (*i).second[2][3], (*i).second[3][3]);
            } else {
                v.set((*i).second[3][0], (*i).second[3][1], (*i).second[3][2], (*i).second[3][3]);
            }
            
            vv.push_back(pair<int, Vector4<double> >((*i).first, v));
            ++i;
        }

        this->outPacked->push(new PackedVector4Event(-1, -1, packedVector4));
    }
    
    delete e;
}


}

