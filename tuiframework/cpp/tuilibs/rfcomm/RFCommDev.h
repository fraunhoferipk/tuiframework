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



#ifndef _tuidevices_RFCommDev_h_
#define _tuidevices_RFCommDev_h_

#include "TTYChannel.h"

#include <tuiframework/core/EventQueue.h>
#include <tuiframework/core/IDevice.h>
#include <tuiframework/core/IEventSink.h>
#include <tuiframework/server/DeviceDescriptor.h>

#include <pthread.h>

#include <map>

namespace tuiframework {
    class ITUIServer;
}

namespace tuidevices {


class RFCommDev : public tuiframework::IDevice {
public:
    static IDevice * createFunction(void * arg);
    static std::string getDeviceName();

    RFCommDev(const tuiframework::DeviceConfig & deviceConfig);
    virtual ~RFCommDev();

        // IDevice
    virtual void deviceConnect(tuiframework::ITUIServer & tuiServer);
    virtual void deviceDisconnect();
    virtual bool deviceExecute();
    virtual void deviceStop();
    virtual void deviceSetEventSink(tuiframework::IEventSink * eventSink);
    virtual const tuiframework::DeviceDescriptor & getDeviceDescriptor() const;

protected:
        /// Starts the sender engine.
    void createSenderLoop();
        /// Stops the engine.
    void cancelSenderLoop();
        /// Entry point for the sender thread.
    static void * runSenderLoopFunc(void * arg);
        /// UDP sender loop.
    void runSenderLoop();

        /// Starts the receiver engine.
    void createReceiverLoop();
        /// Stops the receiverengine.
    void cancelReceiverLoop();
        /// Entry point for the receiver thread.
    static void * runReceiverLoopFunc(void * arg);
        /// UDP receiver loop.
    void runReceiverLoop();
        /// Cleanup handler for the case the thread is canceled
        /// during waitForData()
    static void waitForDataCanceled(void * arg);

protected:
    int analogPortOffset;
    int digitalPortOffset;

    TTYChannel ttyChannel;

    int analogOutFieldCount;
    int digitalOutFieldCount;
    int analogInFieldCount;
    int digitalInFieldCount;	
//    int packetSize;

protected:
        /// Sender Thread ID
    pthread_t senderTID;
        /// Receiver Thread ID
    pthread_t receiverTID;


    tuiframework::EventQueue receiverEventQueue;

protected:
    tuiframework::DeviceDescriptor deviceDescriptor;

    int entityID;
    tuiframework::ITUIServer * tuiServer;
    tuiframework::IEventSink * eventSink;

    tuiframework::EventQueue outputEventQueue;
    
    std::string channel;
    
private:
    
    static const int SYNC_VALUE = 170;

    enum {TOKEN_AOUT, TOKEN_DOUT, TOKEN_AIN, TOKEN_DIN, TOKEN_CFGEND, TOKEN_RESET, NUM_TOKENS};

    enum { COMM_STATE_OK, COMM_STATE_UNCONNECTED, COMM_STATE_STALLED, COMM_STATE_RESET_RECEIVED };

    // sleep for microseconds
    void sleepMs(int microseconds);

    // open serial channel
    bool open();

    // negociate connection (receive number of different channels that device provides)
    // returns either COMM_STATE_OK, COMM_STATE_STALLED or COMM_STATE_RESET
    int negotiate(void);

    // wait for n (default == 1) incoming bytes until given time in milli-seconds.
    // returns true if bytes are available before the timeout, false otherwise.
    bool waitForIncomingData(int milliSeconds, int n = 1);
    
    // reads given amount of bytes into given buffer
    // returns either COMM_STATE_OK, COMM_STATE_STALLED or COMM_STATE_RESET
    int readBytes(char *buffer, int n = 1);

    // check whether given char completes one of the tokens in tokenList and in
    // that case return the index of that token.
    int peekForToken(char c);

    // (needed for waitForToken() - function)
    void clearTokenIndices(void);

    // read characters until one of the tokens in tokenList is matched and return it's index.
    // If there are no characters until a timeout then the function returns -1 (connection stalled)
    int waitForToken();

    static char* tokenList[NUM_TOKENS];

    int tokenCompareIndices[NUM_TOKENS];

    unsigned char m_IOBuffer[100];
    
};

}

#endif
