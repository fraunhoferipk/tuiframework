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


#include "TUIServerApp.h"

#include "../core/IEvent.h"
#include "../core/IDevice.h"
#include "../core/HostAddress.h"
#include "../core/Exception.h"
#define USE_TFDEBUG
#include "../logging/Logger.h"

#include "EPAddressTranslator.h"

#include "../core/TypeRegistration.h"

#include <vector>
#include <sstream>
#include <string>

#include <iostream>
#include <cstdio>
#include <cstring> // GCC 4.3 related build problem

using namespace std;

namespace tuiframework {

TUIServerApp::TUIServerApp(int senderPortNr, int receiverPortNr, bool usingMulticast, const HostAddress & multicastGroup) :
        senderPortNr(senderPortNr),
        receiverPortNr(receiverPortNr),
        usingMulticast(usingMulticast),
        multicastGroup(multicastGroup),
        udpSenderSocket(outHostMsgQueue),
        udpReceiverSocket(inSerializedDataQueue),
        eventDeserializer(inSerializedDataQueue, inEventQueue, eventFactory),
        outHostMsgDispatcher(outHostMsgQueue),
        mcHostMsgDispatcher(outHostMsgQueue),
        eventSerializer(outEventQueue, outHostMsgDispatcher),
        mcEventSerializer(mcEventQueue, mcHostMsgDispatcher),
        deviceContainer(deviceFactory),
        mspContainer(mspFactory) {

	TFINFO("Booting TUI Server")

    TREG_THREADNAME("Main")
    
    this->mcHostMsgDispatcher.addDstAddress(this->multicastGroup);
}


TUIServerApp::~TUIServerApp() {
}



void TUIServerApp::createEntityGraph(const map<string, TUIObjectInstance> & tuiObjectInstanceMap,
        const vector<Connector> & connectorVector) {
    try {
        vector<Connector>::const_iterator i = connectorVector.begin();
        vector<Connector>::const_iterator e = connectorVector.end();
        while (i != e) {

            TFINFO("Connector: " << "(from: " << (*i).getSrcAddress().getOwnerType() << " "
                    << (*i).getSrcAddress().getName() << " "
                    << (*i).getSrcAddress().getPortName() << ") "
                    << "(to: " << (*i).getDstAddress().getOwnerType()
                    << " " << (*i).getDstAddress().getName() << " "
                    << (*i).getDstAddress().getPortName() << ") ");

            IEventSink * sink;
            switch ((*i).getDstAddress().getOwnerType()) {
                case PortAddress::DEVICE: {

                    int dstDeviceID = this->deviceContainer.getEntityIDByName(
                            (*i).getDstAddress().getName());
                    int dstChannelNr = this->deviceContainer.getChannelIDByName(
                            dstDeviceID, (*i).getDstAddress().getPortName());

                    std::map<int, IEventSink *>::iterator iter =
                            this->eventSinkMap.find(dstDeviceID);
                    if (iter == this->eventSinkMap.end()) {
                        throw Exception(TFSTR(""), __FILE__, __LINE__);
                        // TODO Throw Exception
                        TFERROR("");
                    }
                    // int eID = -1, int chNr = -1, IEventSink * eventSink = 0
                    EPAddressTranslator * at = new EPAddressTranslator(
                            dstDeviceID, dstChannelNr, (*iter).second);
                    this->addressTranslatorVector.push_back(at);
                    sink = at;
                    break;
                }
                case PortAddress::MSP: {
                    sink = this->mspContainer.getEventSink(
                            (*i).getDstAddress().getName(),
                            (*i).getDstAddress().getPortName());
                    break;
                }
                case PortAddress::TUIOBJECT: {
                    std::map<std::string, TUIObjectInstance>::const_iterator iter =
                            tuiObjectInstanceMap.find((*i).getDstAddress().getName());
                    if (iter == tuiObjectInstanceMap.end()) {
                        // TODO throw Exception
                        TFERROR("");
                    }

                    int tuiObjectID = (*iter).second.getID();
                    //TFDEBUG("Searching port: " << (*i).getDstAddress().getPortName());
                    map<string, int>::const_iterator iter2 =
                            (*iter).second.getNameChannelNrMap().find(
                                    (*i).getDstAddress().getPortName());
                    if (iter2 == (*iter).second.getNameChannelNrMap().end()) {
                        // TODO throw Exception
                        TFERROR("Size: " << (*iter).second.getNameChannelNrMap().size());
                    }
                    int tuiChannelNr = (*iter2).second;

                    IEventSink * es;
                    if (this->usingMulticast) {
                        es = &this->mcEventQueue;
                    } else {
                        es = &this->outEventQueue;
                    }
                    
                    EPAddressTranslator * at = new EPAddressTranslator(
                            tuiObjectID, tuiChannelNr, es);
                    this->addressTranslatorVector.push_back(at);
                    sink = at;
                    break;
                }
                default: {
                    // TODO Throw Exception
                    TFERROR("");
                    break;
                }
            }

            switch ((*i).getSrcAddress().getOwnerType()) {
                case PortAddress::DEVICE: {
                    int srcDeviceID = this->deviceContainer.getEntityIDByName(
                                            (*i).getSrcAddress().getName());
                    int srcChannelNr = this->deviceContainer.getChannelIDByName(
                            srcDeviceID,
                            (*i).getSrcAddress().getPortName());

                    std::map<int, EventMsgRouter<EPAddress, IEventSink, EPAddress::CompareByChannelNr> *>::iterator iter =
                            this->deviceChannelRouterMap.find(srcDeviceID);
                    if (iter == this->deviceChannelRouterMap.end()) {
                        // TODO Throw Exception
                        TFERROR("");
                    }
                    (*iter).second->addRoutingTableEntry(EPAddress(-1, srcChannelNr), sink);
                    break;
                }
                case PortAddress::MSP: {
                    this->mspContainer.registerEventSink((*i).getSrcAddress().getName(), (*i).getSrcAddress().getPortName(), sink);
                    break;
                }
                case PortAddress::TUIOBJECT: {

                    std::map<std::string, TUIObjectInstance>::const_iterator iter =
                            tuiObjectInstanceMap.find((*i).getSrcAddress().getName());
                    if (iter == tuiObjectInstanceMap.end()) {
                        // TODO throw Exception
                        TFERROR("");
                    }

                    int tuiObjectID = (*iter).second.getID();
                    map<string, int>::const_iterator iter2 =
                            (*iter).second.getNameChannelNrMap().find(
                                    (*i).getSrcAddress().getPortName());
                    if (iter2 == (*iter).second.getNameChannelNrMap().end()) {
                        // TODO throw Exception
                        TFERROR("");
                    }
                    int tuiChannelNr = (*iter2).second;

                    std::map<int, EventMsgRouter<EPAddress, IEventSink, EPAddress::CompareByChannelNr> *>::iterator iter3 =
                            this->tuiChannelRouterMap.find(tuiObjectID);
                    if (iter3 == this->tuiChannelRouterMap.end()) {
                        // TODO Throw Exception
                        TFERROR("");
                    }
                   (*iter3).second->addRoutingTableEntry(EPAddress(-1, tuiChannelNr), sink);
                    break;
                }
                default: {
                    // TODO Throw Exception
                    TFERROR("");
                    break;
                }
            }

            ++i;
        }
    }
    catch (Exception & e) {
        TFERROR(e.getFormattedString());
        //this->tuiServerExit();
    }

}


void TUIServerApp::init(const ServerConfig & serverConfig) {

    cout << serverConfig << endl;

    deviceContainer.createDeviceInstances(serverConfig.getDeviceInstanceMap());
    deviceContainer.connectDevices(*this);
    mspContainer.createInstances(serverConfig.getMSPInstanceMap());

    map<string, TUIObjectInstance> tuiObjectInstanceMap = serverConfig.getTUIObjectInstanceMap();

    // it follows the filtering of not used tui object types
    // initialization of the tuiRouterMap
    vector<TUIObjectType> tuiObjectTypeVector;
    vector<TUIObjectInstance> tuiObjectInstanceVector;
    map<string, TUIObjectType> tuiObjectTypeMap;
        // fills the tuiObjectTypeVector only with TUIObjectType objects,
        // which are used by TUIObjectInstance objects.
    int tuiID = 1;
    {
        map<string, TUIObjectInstance>::iterator i = tuiObjectInstanceMap.begin();
        map<string, TUIObjectInstance>::iterator e = tuiObjectInstanceMap.end();
        while (i != e) {
            //TFDEBUG((*i).first);
            EventMsgRouter<EPAddress, IEventSink, EPAddress::CompareByChannelNr> * tuiChannelRouter =
                        new EventMsgRouter<EPAddress, IEventSink, EPAddress::CompareByChannelNr>();
            this->tuiChannelRouterMap[tuiID] = tuiChannelRouter;
            this->tuiRouter.addRoutingTableEntry(EPAddress(tuiID, -1), tuiChannelRouter);
            map<string, TUIObjectType>::const_iterator iter = tuiObjectTypeMap.find((*i).second.getTypeName());
            if (iter == tuiObjectTypeMap.end()) {
                iter = serverConfig.getTUIObjectTypeMap().find((*i).second.getTypeName());
                tuiObjectTypeMap[(*i).second.getName()] = (*iter).second;
                tuiObjectTypeVector.push_back((*iter).second);
            }

            map<string, int> nameChannelNrMap;
            int curChannelNr = 1;
            map<string, Port>::const_iterator i2 = (*iter).second.getPortMap().begin();
            map<string, Port>::const_iterator e2 = (*iter).second.getPortMap().end();
            while (i2 != e2) {
                //TFDEBUG((*i2).first << " has port number: " << curChannelNr);
                nameChannelNrMap[(*i2).first] = curChannelNr;
                ++curChannelNr;
                ++i2;
            }

            (*i).second.setID(tuiID);
            (*i).second.setNameChannelNrMap(nameChannelNrMap);
            tuiObjectInstanceVector.push_back((*i).second);
            //TFDEBUG((*i).second)

        ++tuiID;
        ++i;
        }
    }

    this->createEntityGraph(tuiObjectInstanceMap, serverConfig.getConnectorVector());

   // cout << "SIZE OF tuiObjectTypeMap: " << this->tuiObjectTypeMap.size();
    //cout << "SIZE OF tuiObjectTypeMap: " << this->tuiObjectTypeMap.size();
    //this->attachedObjects.setDeviceDescriptorVector(deviceContainer.getDeviceDescriptorVector());
    this->attachedObjects.setTUIObjectTypeVector(tuiObjectTypeVector);
    this->attachedObjects.setTUIObjectInstanceVector(tuiObjectInstanceVector);
    
    /*
    {
        vector<pair<string, int> > gv;
        map<string, int>::iterator i = guidEventTypeIDMap.begin();
        map<string, int>::iterator e = guidEventTypeIDMap.end();
        while (i != e) {
            gv.push_back(*i);
            ++i;
        }
        this->guidEventTypeIDVector.setGUIDEventTypeIDVector(gv);
    }
    */
    
    //TFDEBUG(this->attachedObjects)
}


void TUIServerApp::tuiServerRegisterDevice(int entityID, IDevice & device) {
    //TFDEBUG("Device with ID = " << entityID << " registered to server");

    this->deviceMap[entityID] = &device;

    EventMsgRouter<EPAddress, IEventSink, EPAddress::CompareByChannelNr> * deviceChannelRouter =
            new EventMsgRouter<EPAddress, IEventSink, EPAddress::CompareByChannelNr>();
    deviceChannelRouter->setDefaultDst(&this->eventTerminator);
    this->deviceChannelRouterMap[entityID] = deviceChannelRouter;
    device.deviceSetEventSink(deviceChannelRouter);
}


void TUIServerApp::tuiServerDeregisterDevice(int entityID) {
    map<int, IDevice *>::iterator iter = this->deviceMap.find(entityID);
    (*iter).second->deviceSetEventSink(0);
    this->deviceMap.erase(iter);
}


void TUIServerApp::tuiServerRegisterEventSink(int entityID, IEventSink & eventSink) {
    //TFDEBUG("Server registered its event sink to device with ID = " << entityID)
    this->eventSinkMap[entityID] = &eventSink;
}


void TUIServerApp::tuiServerDeregisterEventSink(int entityID) {
    this->eventSinkMap.erase(entityID);
}


void TUIServerApp::tuiServerExecute() {

    //HostAddress senderHostAddress("127.0.0.1:5998");
    //int senderPortNr = 6999;
    //int receiverPortNr = 6998;

    //this->udpSenderSocket.setAddress(senderHostAddress);
    //this->udpSenderSocket.setMyPort(senderPortNr);
    //this->udpReceiverSocket.setMyPort(receiverPortNr);

    this->udpReceiverSocket.setMyPort(this->receiverPortNr);
    this->udpSenderSocket.setMyPort(this->senderPortNr);

    this->udpReceiverSocket.create();

    {
        int rc = pthread_create(&this->inputThread, NULL, TUIServerApp::inputLoopThread_run, this);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    this->eventDeserializer.create();
    this->eventSerializer.create();
    this->udpSenderSocket.create();
    
    if (this->usingMulticast) {
        this->mcEventSerializer.create();
    }
/*
    AttachedObjectsMsg * event = new AttachedObjectsMsg();
    event->setPayload(this->attachedObjects);
    this->outEventQueue.push(event);
*/
    {
        map<int, IDevice *>::iterator i = this->deviceMap.begin();
        map<int, IDevice *>::iterator e = this->deviceMap.end();
        while (i != e) {
            (*i).second->deviceExecute();
            ++i;
        }
    }

}


void TUIServerApp::tuiServerExit() {
    map<int, IDevice *>::iterator i = this->deviceMap.begin();
    map<int, IDevice *>::iterator e = this->deviceMap.end();
    while (i != e) {
        (*i).second->deviceStop();
        ++i;
    }
}


void * TUIServerApp::inputLoopThread_run(void * arg) {
    TREG_THREADNAME("EventInc")
    TFINFO("input loop thread started\n");
    TUIServerApp * serverApp = static_cast<TUIServerApp *>(arg);
    serverApp->executeInputLoop();
    return 0;
}


void TUIServerApp::executeInputLoop() {
    this->inputLoopRunning = true;
    while (inputLoopRunning) {
        this->inEventQueue.waitForData();
        IEvent * event = this->inEventQueue.pop();
        if (event) {
            //cout << event->getEventTypeID() << " " << event << endl;

            if (event->getEventTypeID() == HostEvent::EventTypeID()) {
               HostEvent * ipEventMsg = static_cast<HostEvent *>(event);
               IEvent * event2 = ipEventMsg->getPayload();
               //cout << event2->getEventTypeID() << " " << event2 << endl;

               if (event2->getAddressTypeID() == EPAddress::AddressTypeID()) {
                   this->tuiRouter.push(event2->getDuplicate());
                   /*
                   IEventMsg<EPAddress> * eventMsg = static_cast<IEventMsg<EPAddress> *>(event2);
                   map<int, IEventSink *>::iterator iter = this->eventSinkMap.find(eventMsg->getAddress().getEntityID());

                   if (iter != this->eventSinkMap.end()) {
                       //cout << "delivering: " << event2 << endl;
                       (*iter).second->push(event2->getDuplicate());

                   } else {
                       cout << "event nicht zustellber, entityID = " << eventMsg->getAddress().getEntityID() << endl;
                       delete event;
                   }
                   */
               } else if (event2->getEventTypeID() == SystemCmdMsg::EventTypeID()) {
                   this->handleSystemCmdMsg(ipEventMsg);
               }
           }
           delete event;
        }
    }
}


void TUIServerApp::handleSystemCmdMsg(HostEvent * msg) {
    HostAddress address = msg->getAddress();
    SystemCmdMsg * msg2 = static_cast<SystemCmdMsg *>(msg->getPayload());
    const SystemCmd & systemCmd = msg2->getPayload();
    address.setPortNr(systemCmd.getPortNr());
    if (systemCmd.getCmd() == SystemCmd::requestConnection) {
        TFDEBUG("SystemCmdMsg::RequestConnection: " << address);
        this->outHostMsgDispatcher.addDstAddress(address);
        if (this->usingMulticast) {
            MulticastGroupInvitationMsg * event = new MulticastGroupInvitationMsg();
            event->setPayload(this->multicastGroup);
            this->outEventQueue.push(event);
        } 

        {
            {
                GUIDEventTypeIDVectorMsg * event = new GUIDEventTypeIDVectorMsg();
                event->setPayload(this->guidEventTypeIDVector);
                this->outEventQueue.push(event);
            }
            {
                AttachedObjectsMsg * event = new AttachedObjectsMsg();
                event->setPayload(this->attachedObjects);
                this->outEventQueue.push(event);
            }

        }
    } else if (systemCmd.getCmd() == SystemCmd::removeConnection) {
        TFDEBUG("SystemCmdMsg::RemoveConnection: " << address);
        this->outHostMsgDispatcher.removeDstAddress(address);
    }
}


IEventFactory & TUIServerApp::getEventFactory() {
    return this->eventFactory;
}


IDeviceFactory & TUIServerApp::getDeviceFactory() {
    return this->deviceFactory;
}


IMSPFactory & TUIServerApp::getMSPFactory() {
    return this->mspFactory;
}


}

