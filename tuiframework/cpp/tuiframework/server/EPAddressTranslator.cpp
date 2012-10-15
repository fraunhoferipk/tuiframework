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


#include "EPAddressTranslator.h"

#include "../core/IEventMsg.h"
#include "../core/EPAddress.h"

#include "../logging/Logger.h"

using namespace std;

namespace tuiframework {

EPAddressTranslator::EPAddressTranslator(int eID, int pNr, IEventSink * eventSink) :
    eID(eID),
    pNr(pNr),
    eventSink(eventSink) {
}


EPAddressTranslator::~EPAddressTranslator() {
}


void EPAddressTranslator::setEventSink(IEventSink * eventSink) {
    this->eventSink = eventSink;
}


void EPAddressTranslator::setEID(int eID) {
    this->eID = eID;
}


int EPAddressTranslator::getEID() const {
    return this->eID;
}


void EPAddressTranslator::setPNr(int pNr) {
    this->pNr = pNr;
}


int EPAddressTranslator::getPNr() const {
    return this->pNr;
}


void EPAddressTranslator::push(IEvent * event) {

    IEventMsg<EPAddress> * e = static_cast<IEventMsg<EPAddress> *>(event);
/*
    TFINFO("(" << e->getAddress().getEntityID() << "|" << e->getAddress().getPortNr() << ")" << " -> " <<
            "(" << this->eID << "|" << this->eID << ")");
*/
    e->setAddress(EPAddress(this->eID, this->pNr));
    this->eventSink->push(e);
}

}
