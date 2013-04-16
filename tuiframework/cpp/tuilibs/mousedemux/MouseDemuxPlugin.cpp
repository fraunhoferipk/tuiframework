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

#include "MouseDemuxPlugin.h"
#include "MouseDemuxMSP.h"

#include <tuiframework/server/MSPFactorySingleton.h>

#include <tuiframework/core/Version.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/IMSPFactory.h>

#include <vector>

using namespace tuiframework;

namespace tuiframework {

static std::vector<std::string> mspTypeNameVector;

void soInitPlugin() {
    MSPFactorySingleton::getInstance()->registerCreateFunction(MouseDemuxMSP::getMSPTypeName(), MouseDemuxMSP::createFunction);
    mspTypeNameVector.push_back(MouseDemuxMSP::getMSPTypeName());
}


std::string soGetTFVersion() {
    return TUIFRAMEWORK_VERSION;
}


std::string soGetSOVersion() {
    return "1";
}


void soMSPRegistration(IMSPFactory * MSPFactory) {
    MSPFactory->registerCreateFunction(MouseDemuxMSP::getMSPTypeName(), MouseDemuxMSP::createFunction);
}


const std::vector<std::string> & soGetMSPTypeNameVector() {
    return mspTypeNameVector;
}


std::string soGetMSPTypeVersion(const std::string & mspTypeName) {
    return "1";
}


IMSP * soNewMSPInstance(const std::string & mspTypeName, MSPConfig & mspConfig) {
    return MSPFactorySingleton::getInstance()->createInstance(mspTypeName, &mspConfig);
}


void soDeleteMSPInstance(IMSP * msp) {
    delete msp;
}


}
