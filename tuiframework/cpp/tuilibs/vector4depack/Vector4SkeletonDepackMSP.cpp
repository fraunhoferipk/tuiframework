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


#include "Matrix4SkelDepackMSP.h"

#include <tuitypes/common/SkeletonJointID.h>

static const char * inPackedMatrix4Tag      = "pmIn";
static const char * outPackedMatrix4Tag     = "pmOut";
#define PM_OUT 0

static const char * outHeadTag              = "Head";
static const char * outNeckTag              = "Neck";
static const char * outTorsoTag             = "Torso";
static const char * outWaistTag             = "Waist";
static const char * outLeftCollarTag        = "left_Collar";
static const char * outLeftShoulderTag      = "left_Shoulder";
static const char * outLeftElbowTag         = "left_Elbow";
static const char * outLeftWristTag         = "left_Wrist";
static const char * outLeftHandTag          = "left_Hand";
static const char * outLeftFingertipTag     = "left_Fingertip";
static const char * outRightCollarTag       = "right_Collar";
static const char * outRightShoulderTag     = "right_Shoulder";
static const char * outRightElbowTag        = "right_Elbow";
static const char * outRightWristTag        = "right_Wrist";
static const char * outRightHandTag         = "right_Hand";
static const char * outRightFingertipTag    = "right_Fingertip";
static const char * outLeftHipTag           = "left_Hip";
static const char * outLeftKneeTag          = "left_Knee";
static const char * outLeftAnkleTag         = "left_Ankle";
static const char * outLeftFootTag          = "left_Foot";
static const char * outRightHipTag          = "right_Hip";
static const char * outRightKneeTag         = "right_Knee";
static const char * outRightAnkleTag        = "right_Ankle";
static const char * outRightFootTag         = "right_Foot";

using namespace std;

namespace tuiframework {

IMSP * Matrix4SkelDepackMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new Matrix4SkelDepackMSP(*config);
}

static std::string typeName = "Matrix4SkelDepackMSP";

const std::string & Matrix4SkelDepackMSP::getMSPTypeName() {
    return typeName;
}


Matrix4SkelDepackMSP::Matrix4SkelDepackMSP(const MSPConfig & config) :
    config(config) {

    this->matrix4EventDelegate.setReceiver(this, &Matrix4SkelDepackMSP::handlePackedMatrix4Event);
    
    this->nameToIdMap[outPackedMatrix4Tag   ] = PM_OUT; 
    this->nameToIdMap[outHeadTag            ] = SKEL_HEAD; 
    this->nameToIdMap[outNeckTag            ] = SKEL_NECK; 
    this->nameToIdMap[outTorsoTag           ] = SKEL_TORSO; 
    this->nameToIdMap[outWaistTag           ] = SKEL_WAIST; 
    this->nameToIdMap[outLeftCollarTag      ] = SKEL_LEFT_COLLAR; 
    this->nameToIdMap[outLeftShoulderTag    ] = SKEL_LEFT_SHOULDER; 
    this->nameToIdMap[outLeftElbowTag       ] = SKEL_LEFT_ELBOW; 
    this->nameToIdMap[outLeftWristTag       ] = SKEL_LEFT_WRIST; 
    this->nameToIdMap[outLeftHandTag        ] = SKEL_LEFT_HAND; 
    this->nameToIdMap[outLeftFingertipTag   ] = SKEL_LEFT_FINGERTIP; 
    this->nameToIdMap[outRightCollarTag     ] = SKEL_RIGHT_COLLAR; 
    this->nameToIdMap[outRightShoulderTag   ] = SKEL_RIGHT_SHOULDER; 
    this->nameToIdMap[outRightElbowTag      ] = SKEL_RIGHT_ELBOW; 
    this->nameToIdMap[outRightWristTag      ] = SKEL_RIGHT_WRIST; 
    this->nameToIdMap[outRightHandTag       ] = SKEL_RIGHT_HAND; 
    this->nameToIdMap[outRightFingertipTag  ] = SKEL_RIGHT_FINGERTIP; 
    this->nameToIdMap[outLeftHipTag         ] = SKEL_LEFT_HIP; 
    this->nameToIdMap[outLeftKneeTag        ] = SKEL_LEFT_KNEE; 
    this->nameToIdMap[outLeftAnkleTag       ] = SKEL_LEFT_ANKLE; 
    this->nameToIdMap[outLeftFootTag        ] = SKEL_LEFT_FOOT; 
    this->nameToIdMap[outRightHipTag        ] = SKEL_RIGHT_HIP; 
    this->nameToIdMap[outRightKneeTag       ] = SKEL_RIGHT_KNEE; 
    this->nameToIdMap[outRightAnkleTag      ] = SKEL_RIGHT_ANKLE; 
    this->nameToIdMap[outRightFootTag       ] = SKEL_RIGHT_FOOT; 
}


Matrix4SkelDepackMSP::~Matrix4SkelDepackMSP() {
}


const std::string & Matrix4SkelDepackMSP::getTypeName() const {
    return getMSPTypeName();
}


IEventSink * Matrix4SkelDepackMSP::getEventSink(const std::string & name) {
    if (name.compare(inPackedMatrix4Tag) == 0) {
        return &matrix4EventDelegate;
    }
    return 0;
}


void Matrix4SkelDepackMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
    map<string, int>::iterator iter = this->nameToIdMap.find(name);
    if (iter == this->nameToIdMap.end()) {
        return;
    }
    this->out[(*iter).second] = eventSink;
}


const MSPType & Matrix4SkelDepackMSP::getMSPType() const {
    return this->type;
}


void Matrix4SkelDepackMSP::handlePackedMatrix4Event(PackedMatrix4Event * e) {
    const PackedType<Matrix4<double> > & pm = e->getPayload();

    const vector<pair<int, Matrix4<double> > > & vm = pm.getItems();
    
    vector<pair<int, Matrix4<double> > >::const_iterator i2 = vm.begin();
    vector<pair<int, Matrix4<double> > >::const_iterator e2 = vm.end();
    
    while (i2 != e2) {
        map<int, IEventSink *>::iterator iter = this->out.find((*i2).first);
        if (iter != this->out.end()) {
            (*iter).second->push(new Matrix4Event(-1, -1, (*i2).second));
        }
        ++i2;
    }

    map<int, IEventSink *>::iterator iter = this->out.find(PM_OUT);
    if (iter != this->out.end()) {
        (*iter).second->push(new PackedMatrix4Event(-1, -1, pm));
    }

    delete e;
}



}