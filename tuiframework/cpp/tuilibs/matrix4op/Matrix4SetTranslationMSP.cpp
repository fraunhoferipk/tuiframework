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


#include "Matrix4TranslMSP.h"

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

IMSP * Matrix4TranslMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Matrix4TranslMSP(*config);
}

static std::string typeName = "Matrix4Transl";

const std::string & Matrix4TranslMSP::getMSPTypeName() {
    return typeName;
}


Matrix4TranslMSP::Matrix4TranslMSP(const MSPConfig & config) :
    config(config),
    out(0),
    outPacked(0) {
    
    this->eventDelegateV.setReceiver(this, &Matrix4TranslMSP::handleV);
    this->eventDelegateVP.setReceiver(this, &Matrix4TranslMSP::handleVP);
    
    try {
        this->transpose = this->config.getParameterGroup().getParameterGroup("create").getInt("transposed") != 0;
        TFINFO("Transpose = " << this->transpose);
    }
    catch (Exception & e) {
        e.addErrorMessage("in Matrix4TranslMSP.", __FILE__, __LINE__);
        TFERROR(e.getFormattedString());
    }
}


Matrix4TranslMSP::~Matrix4TranslMSP() {
}


const std::string & Matrix4TranslMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Matrix4TranslMSP::getEventSink(const std::string & name) {
    if (name.compare(inTag) == 0) {
        return &this->eventDelegateV;
    } else if (name.compare(inPackedTag) == 0) {
        return &this->eventDelegateVP;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Matrix4TranslMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outTag) == 0) {
        this->out = eventSink;
    } else if (name.compare(outPackedTag) == 0) {
        this->outPacked = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Matrix4TranslMSP::getMSPType() const {
    return this->type;
}


void Matrix4TranslMSP::handleV(Vector3Event * e) {
    if (this->out) {
        const Vector3<double> & v = e->getPayload();
        Matrix4<double> m;
        m.setRow(0, 1.0, 0, 0, 0);
        m.setRow(1, 0, 1.0, 0, 0);
        m.setRow(2, 0, 0, 1.0, 0);
        m.setRow(3, v[0], v[1], v[2], 1.0);
        
        if (this->transpose) {
            m = Matrix4<double>::transposed(m);
        }
        
        this->out->push(new Matrix4Event(-1, -1, m));
    }
    
    delete e;
}


void Matrix4TranslMSP::handleVP(PackedVector3Event * e) {
    if (this->outPacked) {
        const PackedType<Vector3<double> > & p = e->getPayload();
        const vector<pair<int, Vector3<double> > > & vv = p.getItems();
        
        PackedType<Matrix4<double> > packedMatrix4;
        vector<pair<int, Matrix4<double> > > & mv = packedMatrix4.getItems();
        
        vector<pair<int, Vector3<double> > >::const_iterator i = vv.begin();
        vector<pair<int, Vector3<double> > >::const_iterator e = vv.end();
        while (i != e) {
            Matrix4<double> m;
            m.setRow(0, 1.0, 0, 0, 0);
            m.setRow(1, 0, 1.0, 0, 0);
            m.setRow(2, 0, 0, 1.0, 0);
            m.setRow(3, (*i).second[0], (*i).second[1], (*i).second[2], 1.0);

            if (this->transpose) {
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

