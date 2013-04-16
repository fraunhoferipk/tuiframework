#ifndef _tuiframework_DigitalToAnalogMSP_h_
#define _tuiframework_DigitalToAnalogMSP_h_

#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/MSPType.h>
#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/core/EventDelegate.h>

#include <string>
#include <map>

namespace tuiframework {

class DigitalToAnalogMSP : public IMSP {
public:
    static IMSP * createFunction(void * arg);
    static std::string getMSPTypeName();

    DigitalToAnalogMSP(const MSPConfig & config);
    virtual ~DigitalToAnalogMSP();

    virtual const std::string & getTypeName() const;
    virtual IEventSink * getEventSink(const std::string & name);
    virtual void registerEventSink(const std::string & name, IEventSink * eventSink);
    virtual const MSPType & getMSPType() const;

    void handleEvent(DigitalChangedEvent * e);

protected:
    MSPConfig config;
    MSPType type;

    EventDelegateNC<DigitalChangedEvent, DigitalToAnalogMSP> eventDelegate;

    IEventSink * out;
    
    float value0;
    float value1;
};

}

#endif