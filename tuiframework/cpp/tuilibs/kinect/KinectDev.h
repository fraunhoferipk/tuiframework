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



#ifndef _tuidevices_KinectDev_h_
#define _tuidevices_KinectDev_h_

#include <tuiframework/core/EventQueue.h>
#include <tuiframework/core/IDevice.h>
#include <tuiframework/core/IEventSink.h>
#include <tuiframework/server/DeviceDescriptor.h>
#include <tuitypes/common/Gesture.h>

#include <pthread.h>

#include <map>
#include <cstdio>
#include "SimpleFilter.h"

namespace tuiframework {
    class ITUIServer;
}

namespace tuidevices {

/**
 * @brief Kinect integration.
 *
 * The Kinect is read in an own programm ( see Project %KinectDev )
 * and connected to the TUIServer via network and sockets ( in this class ).
 *
 * <h3>  Mapping of joints to ports  </h3>
 * <table>
 * <tr><td>  OpenNI Skeleton Joints </td><td>  Device Port </td><td>  TUIObject Port      </td></tr>
 * <tr><td>  XN_SKEL_HEAD           </td><td>  JOINT_1     </td><td>  SKEL_HEAD           </td></tr>
 * <tr><td>  XN_SKEL_NECK           </td><td>  JOINT_2     </td><td>  SKEL_NECK           </td></tr>
 * <tr><td>  XN_SKEL_TORSO          </td><td>  JOINT_3     </td><td>  SKEL_TORSO          </td></tr>
 * <tr><td>  XN_SKEL_LEFT_SHOULDER  </td><td>  JOINT_6     </td><td>  SKEL_LEFT_SHOULDER  </td></tr>
 * <tr><td>  XN_SKEL_LEFT_ELBOW     </td><td>  JOINT_7     </td><td>  SKEL_LEFT_ELBOW     </td></tr>
 * <tr><td>  XN_SKEL_LEFT_HAND      </td><td>  JOINT_9     </td><td>  SKEL_LEFT_HAND      </td></tr>
 * <tr><td>  XN_SKEL_RIGHT_SHOULDER </td><td>  JOINT_12    </td><td>  SKEL_RIGHT_SHOULDER </td></tr>
 * <tr><td>  XN_SKEL_RIGHT_ELBOW    </td><td>  JOINT_13    </td><td>  SKEL_RIGHT_ELBOW    </td></tr>
 * <tr><td>  XN_SKEL_RIGHT_HAND     </td><td>  JOINT_15    </td><td>  SKEL_RIGHT_HAND     </td></tr>
 * <tr><td>  XN_SKEL_LEFT_HIP       </td><td>  JOINT_17    </td><td>  SKEL_LEFT_HIP       </td></tr>
 * <tr><td>  XN_SKEL_LEFT_KNEE      </td><td>  JOINT_18    </td><td>  SKEL_LEFT_KNEE      </td></tr>
 * <tr><td>  XN_SKEL_LEFT_FOOT      </td><td>  JOINT_20    </td><td>  SKEL_LEFT_FOOT      </td></tr>
 * <tr><td>  XN_SKEL_RIGHT_HIP      </td><td>  JOINT_21    </td><td>  SKEL_RIGHT_HIP      </td></tr>
 * <tr><td>  XN_SKEL_RIGHT_KNEE     </td><td>  JOINT_22    </td><td>  SKEL_RIGHT_KNEE     </td></tr>
 * <tr><td>  XN_SKEL_RIGHT_FOOT     </td><td>  JOINT_24    </td><td>  SKEL_RIGHT_FOOT     </td></tr>
 * </table>
 *
 * @see tuiframework::SimpleFilter
 */
class KinectDev : public tuiframework::IDevice, public tuiframework::IEventSink {

public:
    static IDevice * createFunction(void * arg);
    static std::string getDeviceName();

    KinectDev(const tuiframework::DeviceConfig & deviceConfig);
    virtual ~KinectDev();

        // IDevice
    virtual void deviceConnect(tuiframework::ITUIServer & tuiServer);
    virtual void deviceDisconnect();
    virtual bool deviceExecute();
    virtual void deviceStop();
    virtual void deviceSetEventSink(tuiframework::IEventSink * eventSink);
    virtual const tuiframework::DeviceDescriptor & getDeviceDescriptor() const;

    // IEventSink
    virtual void push(tuiframework::IEvent * event);

protected:
    static void * inputLoopThread_run(void * arg);
    void executeInputLoop();

    static void * outputLoopThread_run(void * arg);
    void executeOutputLoop();

    static void * inputGestureLoopThread_run(void * arg);
    void executeGestureInputLoop();

    static void * outputGestureLoopThread_run(void * arg);
    void executeGestureOutputLoop();

protected:
    tuiframework::DeviceDescriptor deviceDescriptor;

    int entityID;
    tuiframework::ITUIServer * tuiServer;
    tuiframework::IEventSink * eventSink;

    volatile bool outputLoopRunning;
    pthread_t outputLoopThread;
    volatile bool inputLoopRunning;
    pthread_t inputLoopThread;

    volatile bool outputGestureLoopRunning;
    pthread_t outputGestureLoopThread;
    volatile bool inputGestureLoopRunning;
    pthread_t inputGestureLoopThread;

    tuiframework::EventQueue outputEventQueue;
};

}

#endif
