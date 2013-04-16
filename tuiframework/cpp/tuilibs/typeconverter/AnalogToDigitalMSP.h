#ifndef _tuiframework_AnalogToDigitalMSP_h_
#define _tuiframework_AnalogToDigitalMSP_h_

#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/MSPType.h>
#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/core/EventDelegate.h>

#include <string>
#include <map>

namespace tuiframework {

class AnalogToDigitalMSP : public IMSP {
public:
    static IMSP * createFunction(void * arg);
    static std::string getMSPTypeName();

    AnalogToDigitalMSP(const MSPConfig & config);
    virtual ~AnalogToDigitalMSP();

    virtual const std::string & getTypeName() const;
    virtual IEventSink * getEventSink(const std::string & name);
    virtual void registerEventSink(const std::string & name, IEventSink * eventSink);
    virtual const MSPType & getMSPType() const;

    void handleEvent(AnalogChangedEvent * e);

protected:
    MSPConfig config;
    MSPType type;

    EventDelegateNC<AnalogChangedEvent, AnalogToDigitalMSP> eventDelegate;

    IEventSink * out;
    
    float threshold;
};

}

#endif