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



#ifndef _tuiframework_KinectARTTransfUnit_h_
#define _tuiframework_KinectARTTransfUnit_h_

#include <tuiframework/core/IEventSink.h>
#include <tuiframework/server/ParameterGroup.h>
#include <tuitypes/common/CommonTypeReg.h>

#include <map>

namespace tuiframework {


class KinectARTTransfUnit : public IEventSink {
public:
    KinectARTTransfUnit();
    virtual ~KinectARTTransfUnit();

    void setParameterGroup(const ParameterGroup & parameterGroup);

    void setEventSink(IEventSink * sink);

    virtual void push(IEvent * event);

    void setEventSinkMap(std::map<std::string, IEventSink *> * EventSinkMap);

protected:
    IEventSink * eventSink;

private:

    void kinectPoseChanged(const Matrix4ChangedEvent * event);

    void HEAD_Changed(const KinectEvent * event);
    void TORSO_Changed(const KinectEvent * event);
    void NECK_Changed(const KinectEvent * event);

    void LEFT_HAND_Changed(const KinectEvent * event);
    void RIGHT_HAND_Changed(const KinectEvent * event);

    void LEFT_ELBOW_Changed(const KinectEvent * event);
    void RIGHT_ELBOW_Changed(const KinectEvent * event);

    void LEFT_HIP_Changed(const KinectEvent * event);
    void RIGHT_HIP_Changed(const KinectEvent * event);

    void LEFT_SHOULDER_Changed(const KinectEvent * event);
    void RIGHT_SHOULDER_Changed(const KinectEvent * event);

    void LEFT_KNEE_Changed(const KinectEvent * event);
    void RIGHT_KNEE_Changed(const KinectEvent * event);

    void LEFT_FOOT_Changed(const KinectEvent * event);
    void RIGHT_FOOT_Changed(const KinectEvent * event);

    void GESTURE(const GestureEvent * event);

    std::map<std::string, IEventSink *> * registeredEventSinkMap;
    Matrix4Data rot_trans;

    Vector3d m_lh;
    Vector3d m_rh;
    Vector3d m_le;
    Vector3d m_re;

    float translX;
    float translY;
    float translZ;

    float filterTreshhold;

    int m_dev;
    bool clicked;
};

}

#endif

