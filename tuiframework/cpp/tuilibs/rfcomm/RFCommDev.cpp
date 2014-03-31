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



#include "RFCommDev.h"

#include <tuiframework/core/ITUIServer.h>
#include <tuiframework/core/IEvent.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuiframework/core/Exception.h>
#include <tuitypes/common/CommonTypeReg.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

#include <cstdlib> // GCC 4.3 related build problem

#include <ctime>

#include <iostream>

#define BTP_MODEMDEVICE "/dev/rfcomm0"
#define BTP_BAUDRATE B38400
#define BTP_SYNC_VALUE 170
using namespace tuiframework;
using namespace std;

namespace tuidevices {

IDevice * RFCommDev::createFunction(void * arg) {
    DeviceConfig * deviceConfig = static_cast<DeviceConfig *>(arg);
    return new RFCommDev(*deviceConfig);
}


std::string RFCommDev::getDeviceName() {
    return "RFCommDev";
}


char* RFCommDev::tokenList[NUM_TOKENS] = 
{
    "aout",
    "dout",
    "ain",
    "din",
    "cfgend",
    "reset"
};

RFCommDev::RFCommDev(const DeviceConfig & deviceConfig)
{

    this->deviceDescriptor.setEntityID(deviceConfig.getEntityID());
    this->entityID = this->deviceDescriptor.getEntityID();

    map<string, Port> portMap;

    portMap["AOUT1"] = Port("AOUT1", "AnalogChannel", Port::Source);
    portMap["AOUT2"] = Port("AOUT2", "AnalogChannel", Port::Source);
    portMap["AOUT3"] = Port("AOUT3", "AnalogChannel", Port::Source);
    portMap["AOUT4"] = Port("AOUT4", "AnalogChannel", Port::Source);
    portMap["AOUT5"] = Port("AOUT5", "AnalogChannel", Port::Source);
    portMap["AOUT6"] = Port("AOUT6", "AnalogChannel", Port::Source);
    portMap["AOUT7"] = Port("AOUT7", "AnalogChannel", Port::Source);
    portMap["AOUT8"] = Port("AOUT8", "AnalogChannel", Port::Source);

    portMap["DOUT1"] = Port("DOUT1", "DigitalChannel", Port::Source);
    portMap["DOUT2"] = Port("DOUT2", "DigitalChannel", Port::Source);
    portMap["DOUT3"] = Port("DOUT3", "DigitalChannel", Port::Source);
    portMap["DOUT4"] = Port("DOUT4", "DigitalChannel", Port::Source);
    portMap["DOUT5"] = Port("DOUT5", "DigitalChannel", Port::Source);
    portMap["DOUT6"] = Port("DOUT6", "DigitalChannel", Port::Source);
    portMap["DOUT7"] = Port("DOUT7", "DigitalChannel", Port::Source);
    portMap["DOUT8"] = Port("DOUT8", "DigitalChannel", Port::Source);

    DeviceType deviceType;
    deviceType.setPortMap(portMap);
    deviceType.setDeviceTypeName("RFCommDevice");
    deviceType.setDescription("");

    this->deviceDescriptor.setDeviceType(deviceType);
    this->deviceDescriptor.setInstanceName(deviceConfig.getInstanceName());

    std::map<std::string, int> nameChannelNrMap;
    int nr = 1;
    map<string, Port>::iterator i = portMap.begin();
    map<string, Port>::iterator e = portMap.end();
    while (i != e) {
        nameChannelNrMap[(*i).second.getName()] = nr;
        ++nr;
        ++i;
    }
    this->deviceDescriptor.setNameChannelNrMap(nameChannelNrMap);

    try {
        this->channel = deviceConfig.getParameterGroup().getString("Config|Channel");
        TFDEBUG("rfcomm channel = " << this->channel);
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
    }


    this->analogPortOffset = nameChannelNrMap["AOUT1"];
    this->digitalPortOffset = nameChannelNrMap["DOUT1"];
}


RFCommDev::~RFCommDev() {
}


bool RFCommDev::deviceExecute() {

/* old stuff
    try {
        
        this->ttyChannel.open(this->channel, BTP_BAUDRATE);
        sleep(1);
        
        // serial connection established, wait for channel count ("A" + number + "D" + number)
        const char * str1 = "config";
        this->ttyChannel.writeBytes(str1, 6);
        
        sleep(1);
        
        while(this->ttyChannel.readByte() != 'A') {};
        this->analogOutFieldCount = this->ttyChannel.readByte();
        while(this->ttyChannel.readByte() != 'D') {};
        this->digitalOutFieldCount = this->ttyChannel.readByte();
        
        this->packetSize = 2 * this->analogOutFieldCount + (this->digitalOutFieldCount / 8) + 2;

        
        // serial connection established, now send "start" command to pen
        const char * str2 = "start";
        this->ttyChannel.writeBytes(str2, 5);
                
    } catch (Exception & e) {
        TFERROR(e.getFormattedString());
        return false;
    }
*/

    this->createReceiverLoop();
    this->createSenderLoop();

    return true;
}


void RFCommDev::deviceStop() {
    pthread_cancel(this->senderTID);
    pthread_join(this->senderTID, 0);
    pthread_cancel(this->receiverTID);
    pthread_join(this->receiverTID, 0);
}



void RFCommDev::deviceConnect(tuiframework::ITUIServer & tuiServer) {
    tuiServer.tuiServerRegisterDevice(this->entityID, *this);
    tuiServer.tuiServerRegisterEventSink(this->entityID, this->receiverEventQueue);
    this->tuiServer = &tuiServer;
}


void RFCommDev::deviceDisconnect() {
    if (tuiServer) {
        this->tuiServer->tuiServerDeregisterDevice(this->entityID);
        this->tuiServer->tuiServerDeregisterEventSink(this->entityID);
        this->tuiServer = 0;
    }
}


void RFCommDev::deviceSetEventSink(tuiframework::IEventSink * eventSink) {
    this->eventSink = eventSink;
}


const DeviceDescriptor & RFCommDev::getDeviceDescriptor() const {
    return this->deviceDescriptor;
}


void RFCommDev::createSenderLoop() {
    int rc = pthread_create(&this->senderTID, NULL, RFCommDev::runSenderLoopFunc, this);
    if (rc) {
        TFFATAL("pthread_create(...) failed.");
    }
}


void RFCommDev::cancelSenderLoop() {
    pthread_cancel(this->senderTID);
}


void * RFCommDev::runSenderLoopFunc(void * arg) {
    TFINFO("RFCommDev sender thread started.");
    RFCommDev * b = static_cast<RFCommDev *>(arg);
    b->runSenderLoop();
    return 0;
}


void RFCommDev::runSenderLoop() {
    
    int commState = COMM_STATE_UNCONNECTED;

    int i;
    
    while (true) {

    try {
        if (commState != COMM_STATE_OK)
        {
            if (commState == COMM_STATE_RESET_RECEIVED)
            {
                //cerr << "COMM_STATE_RESET_RECEIVED\n";
                commState = this->negotiate();
                if (commState == COMM_STATE_RESET_RECEIVED) sleepMs(1000);
            }
            else if (commState == COMM_STATE_STALLED)
            {
                //cerr << "COMM_STATE_STALLED\n"; 
                ttyChannel.close();
                commState = COMM_STATE_UNCONNECTED;
                sleepMs(1000);
            }
            else
            {
                // state is COMM_STATE_UNCONNECTED
                //cerr << "COMM_STATE_UNCONNECTED\n"; 
                while(!this->open()) sleepMs(1000);
                commState = this->negotiate();			
            }
            
        }
        else
        {
                
            int inputSize = 2 * analogOutFieldCount + ((digitalOutFieldCount+7) / 8);

            if (inputSize > 0)
            {
                if (inputSize > 1)
                {
                    // wait for sync-byte
                    do {
                        commState = readBytes((char*)m_IOBuffer, 1);
                    } while((commState == COMM_STATE_OK) && (m_IOBuffer[0] != SYNC_VALUE));
                }

                // read device output data
                if (commState == COMM_STATE_OK)
                {
                    commState = readBytes((char*)&(m_IOBuffer[0]), inputSize);
                }


                if (commState == COMM_STATE_OK)
                {
                    int offset = 0;

                    // read analog channels
                    for (i = 0; i < analogOutFieldCount; i++)
                    {
                        AnalogChangedEvent * event = new AnalogChangedEvent();
                        event->setAddress(EPAddress(this->entityID, this->analogPortOffset + i));					
                        event->setPayload(static_cast<float>(((unsigned char)m_IOBuffer[offset])*256 + ((unsigned char)m_IOBuffer[offset+1]))/1023.0f);
                        this->eventSink->push(event);						
                        
                        offset += 2;
                    }

                    // read digital channels (each byte = 8 channels)
                    for (i = 0; i < digitalOutFieldCount; i++)
                    {
                    
                        DigitalChangedEvent * event = new DigitalChangedEvent();
                        event->setAddress(EPAddress(this->entityID, this->digitalPortOffset + i));
                        event->setPayload((m_IOBuffer[offset + i / 8] & (1 << i)) > 0);
                        this->eventSink->push(event);					
                    }
                }
            }
            else
            {
                // even if device has no outgoing channels, check for "reset" command from device
                if (ttyChannel.availableBytes() > 0)
                {
                    int token = peekForToken( (char)ttyChannel.readByte() );
                    if (token == TOKEN_RESET) commState = COMM_STATE_RESET_RECEIVED;
                }
            }


            // write output channels (if any)
            
            // TODO: TUI Data-Sources?
/*			

            //---- aus DummyDeviceDA ----

            this->outputEventQueue.waitForData();
            IEvent * event = this->outputEventQueue.pop();
            if (event) {
                cout << "DummyDeviceDA: " << event->getEventTypeID() << " " << event << endl;
                delete event;
            }
            //---------------------------


        --> in PhidgetsDevice schauen

            int outputSize = 2 * analogInFieldCount+ ((digitalInFieldCount+7) / 8);
            if (outputSize > 0)
            {
                if (outputSize > 1) m_tty.writeByte((unsigned char)SYNC_VALUE);

                // write analog channels
                for (i = 0; i < m_NumChannels[2]; i++)
                {
                    m_tty.writeByte(m_AnalogValuesOut[i] >> 8);
                    m_tty.writeByte(m_AnalogValuesOut[i] & 255);
                }

                // write digital channels (each byte = 8 channels)
                for (i = 0; i < (digitalInFieldCount+ 7) / 8; i++)
                {
                    m_tty.writeByte(m_DigitalValuesOut[i]);
                }
            }
*/
        
        }
    
    
/*	alter kram
    char buffer[this->packetSize];
            this->ttyChannel.readNumBytes(buffer, this->packetSize);
                // check that first byte contains frame-marker
                // if not, synchronize (read until next frame-marker received)
            if (buffer[0] != BTP_SYNC_VALUE) {
                // synchronize
            while (this->ttyChannel.readByte() != BTP_SYNC_VALUE) {
            }
            this->ttyChannel.readNumBytes(&buffer[1], this->packetSize - 1);
            }

                // decode the buffer
                // skip the synchronization byte
            int offset = 1;
            {
                for (int i = 0; i < this->analogOutFieldCount; ++i) {
                    AnalogChangedEvent * event = new AnalogChangedEvent();
                    event->setAddress(EPAddress(this->entityID, this->analogPortOffset + i));
                    // TODO
                    event->setPayload(static_cast<float>(((unsigned char)buffer[offset])*256 + ((unsigned char)buffer[offset+1]))/1023.0f);
                    this->eventSink->push(event);

                    offset += 2;
                }
            }

            if (this->digitalOutFieldCount > 0)
            {
                int i;
                for (i = 0; i < this->digitalOutFieldCount; ++i) {
                    DigitalChangedEvent * event = new DigitalChangedEvent();
                    event->setAddress(EPAddress(this->entityID, this->digitalPortOffset + i));
                    event->setPayload((buffer[offset + i / 8] & (1 << i)) > 0);
                    this->eventSink->push(event);
                }
                offset = offset + ( i-1)/8 +1;
            }
*/









        } catch (Exception & e) {
            TFERROR("Exception" << endl << e.getFormattedString());
        }

    }

}


void RFCommDev::createReceiverLoop() {
    int rc = pthread_create(&this->receiverTID, NULL, RFCommDev::runReceiverLoopFunc, this);
    if (rc) {
        TFFATAL("pthread_create(...) failed.");
    }
}


void RFCommDev::cancelReceiverLoop() {
    pthread_cancel(this->receiverTID);
}


void * RFCommDev::runReceiverLoopFunc(void * arg) {
    TFINFO("RFCommDev receiver thread started.");
    RFCommDev * b = static_cast<RFCommDev *>(arg);
    b->runReceiverLoop();
    return 0;
}


void RFCommDev::runReceiverLoop() {
    pthread_cleanup_push(RFCommDev::waitForDataCanceled, this);
    while (true) {
        this->receiverEventQueue.waitForData();
        IEvent * event = this->receiverEventQueue.pop();
        if (event) {
        cout << "RFCommDev: " << event->getEventTypeID() << " " << event << endl;
        delete event;
        }
    }
    pthread_cleanup_pop(1);
}


void RFCommDev::waitForDataCanceled(void * arg) {
    RFCommDev * b = static_cast<RFCommDev *>(arg);
    b->outputEventQueue.cleanupAfterWaitCanceled();
}


/************************** stuff for new protocol ******************************/

void RFCommDev::sleepMs(int microseconds)
{
#ifdef WIN32
    Sleep(microseconds);
#else
    usleep(microseconds * 1000);
#endif
}


bool RFCommDev::open()
{

    try {	
        if (!ttyChannel.open(this->channel, BTP_BAUDRATE)) return false;  
    } catch (tuiframework::Exception e) {
        cerr << "TTYChannel Exception: " << e.getFormattedString().c_str() << "\n";
        return false;
    }

    ttyChannel.flushInput();

    sleepMs(500);

            
    //cerr << "sending reset request...\n";
    char resetCommand[] = "reset";
    ttyChannel.writeBytes(resetCommand, sizeof(resetCommand));

    return true;
}

int RFCommDev::negotiate(void)
{
    
    int numChannels[4] = {0, 0, 0, 0};

    //cerr << "sending configuration request...\n";
    char configCommand[] = "config";
    ttyChannel.writeBytes(configCommand, sizeof(configCommand));

    //cerr << "waiting for config ...\n";

    int token;
    do
    {
        token = waitForToken();
        if ((token < 0) || (token == TOKEN_RESET)) 
        {
            if (token == TOKEN_RESET) return COMM_STATE_RESET_RECEIVED;
            else return COMM_STATE_STALLED;
        }

        if (token < TOKEN_CFGEND) numChannels[token] = ttyChannel.readByte();
    }
    while (token != TOKEN_CFGEND);

    // cerr << "config: aout=" << numChannels[0] << " dout=" << numChannels[1] << " ain=" << numChannels[2] << " din=" << numChannels[3] << "\n\n";

    analogOutFieldCount = numChannels[0];
    digitalOutFieldCount = numChannels[1];
    analogInFieldCount = numChannels[2];
    digitalInFieldCount = numChannels[3];
    
    
    char startCommand[] = "start";
    ttyChannel.writeBytes(startCommand, sizeof(startCommand));

    if ((numChannels[0] + numChannels[1]) > 0)
    {
        //cerr << "waiting for incoming data...";
        if (!waitForIncomingData(5000)) 
        {
            //cerr << "failed, no incoming data during timeout!\n";
            return COMM_STATE_STALLED;
        }
        //cerr << "ok, received incoming data\n";
    }

    return COMM_STATE_OK;
}


bool RFCommDev::waitForIncomingData(int milliSeconds, int n)
{
    int timeout = 0;
    while (ttyChannel.availableBytes() < n)
    {
        timeout++;
        sleepMs(1);
        if (timeout > milliSeconds) return false;
    }
    return true;
}


int RFCommDev::readBytes(char *buffer, int n)
{
    int i=0;

    if (!waitForIncomingData(1000, n)) return COMM_STATE_STALLED;
    
    do
    {
        i += ttyChannel.readBytes((char *)&buffer[i], n-i);
    } while (i<n);
        
    int j=0;
    while (j<i)
    {
        if (peekForToken(buffer[j]) == TOKEN_RESET) return COMM_STATE_RESET_RECEIVED;
        j++;
    }
        
    return COMM_STATE_OK;
}


int RFCommDev::peekForToken(char c)
{
    for (int i=0; i<NUM_TOKENS; i++)
    {
        if (c == tokenList[i][tokenCompareIndices[i]])
        {
            tokenCompareIndices[i]++;
            if (tokenList[i][tokenCompareIndices[i]] == 0)
                return i;
        }
        else tokenCompareIndices[i] = 0;
    }
    return -1;
}


void RFCommDev::clearTokenIndices(void)
{
    for (int i=0; i<NUM_TOKENS; i++) tokenCompareIndices[i] = 0;
}


int RFCommDev::waitForToken()
{
    clearTokenIndices();

    int token;
    do
    {
        int ret;

        if (!waitForIncomingData(5000)) return -1;
        ret = ttyChannel.readByte();
        char c = (char)ret;
        //cerr << c;
        token = peekForToken(c);
    }
    while (token < 0);
    return token;
}

} // namespace
