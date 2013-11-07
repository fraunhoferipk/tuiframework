/*r
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


#include "Matrix4InverseMSP.h"

#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/server/ParameterGroup.h>
#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inTag = "Ain";
static const char * inPackedTag = "APin";
static const char * outTag = "Aout";
static const char * outPackedTag = "APout";
static const char * invTag = "Ainv";
static const char * invPackedTag = "APinv";



using namespace std;

namespace tuiframework {

IMSP * Matrix4InverseMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Matrix4InverseMSP(*config);
}

static std::string typeName = "Matrix4Inverse";

const std::string & Matrix4InverseMSP::getMSPTypeName() {
    return typeName;
}


Matrix4InverseMSP::Matrix4InverseMSP(const MSPConfig & config) :
    config(config),
    out(0),
    outPacked(0) {
    
    this->eventDelegateM.setReceiver(this, &Matrix4InverseMSP::handleM);
    this->eventDelegateMP.setReceiver(this, &Matrix4InverseMSP::handleMP);
}


Matrix4InverseMSP::~Matrix4InverseMSP() {
}


const std::string & Matrix4InverseMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Matrix4InverseMSP::getEventSink(const std::string & name) {
    if (name.compare(inTag) == 0) {
        return &this->eventDelegateM;
    } else if (name.compare(inPackedTag) == 0) {
        return &this->eventDelegateMP;
    } else {
        TFERROR("")
    }
        
    return 0;
}


void Matrix4InverseMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    if (name.compare(outTag) == 0) {
        this->out = eventSink;
    } else if (name.compare(outPackedTag) == 0) {
        this->outPacked = eventSink;
    } else if (name.compare(invTag) == 0) {
        this->inv = eventSink;
    } else if (name.compare(invPackedTag) == 0) {
        this->invPacked = eventSink;
    } else {
        TFERROR("")
    }
}


const MSPType & Matrix4InverseMSP::getMSPType() const {
    return this->type;
}


void Matrix4InverseMSP::handleM(Matrix4Event * e) {
    if (this->inv) {
        const Matrix4<double> & m = e->getPayload();
        Matrix4<double> minv;
        if (m.getInverse(minv)) {
            this->inv->push(new Matrix4Event(-1, -1, minv));
        }
    }
    
    if (this->out) {
        this->out->push(new Matrix4Event(-1, -1, e->getPayload()));
    }
    
    delete e;
}


void Matrix4InverseMSP::handleMP(PackedMatrix4Event * e) {
    if (this->invPacked) {
        const PackedType<Matrix4<double> > & p = e->getPayload();
        const vector<pair<int, Matrix4<double> > > & vm = p.getItems();
        
        PackedType<Matrix4<double> > packedMatrix4Inverse;
        vector<pair<int, Matrix4<double> > > & mv = packedMatrix4Inverse.getItems();
        
        vector<pair<int, Matrix4<double> > >::const_iterator i = vm.begin();
        vector<pair<int, Matrix4<double> > >::const_iterator e = vm.end();
        while (i != e) {
            Matrix4<double> minv;
            
            if ((*i).second.getInverse(minv)) {
                mv.push_back(pair<int, Matrix4<double> >((*i).first, minv));
            }
            
            ++i;
        }

        if(mv.size()) {
            this->invPacked->push(new PackedMatrix4Event(-1, -1, packedMatrix4Inverse));
        }
    }
    
    if (this->outPacked) {
        this->outPacked->push(new PackedMatrix4Event(-1, -1, e->getPayload()));
    }
    
    delete e;
}


}

