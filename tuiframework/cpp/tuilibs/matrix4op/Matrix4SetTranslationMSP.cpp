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


#include "Matrix4SetTranslationMSP.h"

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

IMSP * Matrix4SetTranslationMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Matrix4SetTranslationMSP(*config);
}

static std::string typeName = "Matrix4SetTranslation";

const std::string & Matrix4SetTranslationMSP::getMSPTypeName() {
    return typeName;
}


Matrix4SetTranslationMSP::Matrix4SetTranslationMSP(const MSPConfig & config) :
    config(config),
    out(0),
    outPacked(0) {
    
    this->eventDelegateV.setReceiver(this, &Matrix4SetTranslationMSP::handleV);
    this->eventDelegateVP.setReceiver(this, &Matrix4SetTranslationMSP::handleVP);
    
    try {
        this->premultiply = this->config.getParameterGroup().getParameterGroup("create").getInt("pre-multiply") != 0;
        TFINFO("pre-multiply = " << this->premultiply);
    }
    catch (Exception & e) {
        e.addErrorMessage("in Matrix4SetTranslationMSP.", __FILE__, __LINE__);
        TFERROR(e.getFormattedString());
    }
}


Matrix4SetTranslationMSP::~Matrix4SetTranslationMSP() {
}


const std::string & Matrix4SetTranslationMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Matrix4SetTranslationMSP::getEventSink(const std::string & name) {
    if (name.compare(inTag) == 0) {
        return &this->eventDelegateV;
    } else if (name.compare(inPackedTag) == 0) {
        return &this->eventDelegateVP;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Matrix4SetTranslationMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outTag) == 0) {
        this->out = eventSink;
    } else if (name.compare(outPackedTag) == 0) {
        this->outPacked = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Matrix4SetTranslationMSP::getMSPType() const {
    return this->type;
}


void Matrix4SetTranslationMSP::handleV(Vector4Event * e) {
    if (this->out) {
        const Vector4<double> & v = e->getPayload();
        Matrix4<double> m;
        m.setRow(0, 1.0, 0, 0, 0);
        m.setRow(1, 0, 1.0, 0, 0);
        m.setRow(2, 0, 0, 1.0, 0);
        m.setRow(3, v[0], v[1], v[2], 1.0);
        
        if (this->premultiply) {
            m = Matrix4<double>::transposed(m);
        }
        
        this->out->push(new Matrix4Event(-1, -1, m));
    }
    
    delete e;
}


void Matrix4SetTranslationMSP::handleVP(PackedVector4Event * e) {
    if (this->outPacked) {
        const PackedType<Vector4<double> > & p = e->getPayload();
        const vector<pair<int, Vector4<double> > > & vv = p.getItems();
        
        PackedType<Matrix4<double> > packedMatrix4;
        vector<pair<int, Matrix4<double> > > & mv = packedMatrix4.getItems();
        
        vector<pair<int, Vector4<double> > >::const_iterator i = vv.begin();
        vector<pair<int, Vector4<double> > >::const_iterator e = vv.end();
        while (i != e) {
            Matrix4<double> m;
            m.setRow(0, 1.0, 0, 0, 0);
            m.setRow(1, 0, 1.0, 0, 0);
            m.setRow(2, 0, 0, 1.0, 0);
            m.setRow(3, (*i).second[0], (*i).second[1], (*i).second[2], 1.0);

            if (this->premultiply) {
                m = Matrix4<double>::transposed(m);
            }
            
            mv.push_back(pair<int, Matrix4<double> >((*i).first, m));
            ++i;
        }

        this->outPacked->push(new PackedMatrix4Event(-1, -1, packedMatrix4));
    }
    
    delete e;
}


}

