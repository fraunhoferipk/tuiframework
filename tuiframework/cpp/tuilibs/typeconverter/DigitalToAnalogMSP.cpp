#include "DigitalToAnalogMSP.h"

#include <tuiframework/core/Exception.h>
#define USE_TFDEBUG
#include <tuiframework/logging/Logger.h>

static const char * inTag = "in";
static const char * outTag = "out";

using namespace std;

namespace tuiframework {

IMSP * DigitalToAnalogMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new DigitalToAnalogMSP(*config);
}

static std::string typeName = "DigitalToAnalogMSP";

std::string DigitalToAnalogMSP::getMSPTypeName() {
    return typeName;
}


DigitalToAnalogMSP::DigitalToAnalogMSP(const MSPConfig & config) :
    config(config),
    out(0) {

    this->eventDelegate.setReceiver(this, &DigitalToAnalogMSP::handleEvent);
    
    try {
        this->value0 = config.getParameterGroup().getDouble("Convert|Value_0");
        this->value1 = config.getParameterGroup().getDouble("Convert|Value_1");
        TFDEBUG("Value_0 = " << this->value0);
        TFDEBUG("Value_1 = " << this->value1);
    }
    catch(Exception & e) {
        TFERROR(e.getFormattedString());
    }
}


DigitalToAnalogMSP::~DigitalToAnalogMSP() {
}


const std::string & DigitalToAnalogMSP::getTypeName() const {
    return this->type.getTypeName();
}


IEventSink * DigitalToAnalogMSP::getEventSink(const std::string & name) {
    if (name.compare(inTag) == 0) {
        return &eventDelegate;
    }
    return 0;
}


void DigitalToAnalogMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
     if (name.compare(outTag) == 0) {
         this->out = eventSink;
     }
}


const MSPType & DigitalToAnalogMSP::getMSPType() const {
    return this->type;
}


void DigitalToAnalogMSP::handleEvent(DigitalChangedEvent * e) {
    if (this->out) {
        if (e->getPayload()) {
            this->out->push(new AnalogChangedEvent(-1, -1, this->value1));
        } else {
            this->out->push(new AnalogChangedEvent(-1, -1, this->value0));
        }
    }

    delete e;
}

}