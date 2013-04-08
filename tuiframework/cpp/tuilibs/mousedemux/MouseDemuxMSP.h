#ifndef _tuiframework_MouseDemuxMSP_h_
#define _tuiframework_MouseDemuxMSP_h_

#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/MSPType.h>
#include <tuitypes/common/CommonTypeReg.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/core/EventDelegate.h>

#include <string>
#include <map>

namespace tuiframework {

class MouseDemuxMSP : public IMSP {
public:
    static IMSP * createFunction(void * arg);
    static std::string getMSPTypeName();

    MouseDemuxMSP(const MSPConfig & config);
    virtual ~MouseDemuxMSP();

    virtual const std::string & getTypeName() const;
    virtual IEventSink * getEventSink(const std::string & name);
    virtual void registerEventSink(const std::string & name, IEventSink * eventSink);
    virtual const MSPType & getMSPType() const;

    void handleMouseEvent(MouseEvent * e);

protected:
    MSPConfig config;
    MSPType type;

    EventDelegateNC<MouseEvent, MouseDemuxMSP> mouseEventDelegate;
    std::map<std::string, int> nameToIdMap;

    IEventSink * out[8];
};

}

#endif