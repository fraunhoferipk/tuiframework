#include <tuitypes/common/CommonTypeReg.h>

#include <tuiframework/core/Version.h>


void dllInitPlugin() {
}


void dllTypeRegistration(tuiframework::IEventFactory * eventFactory, tuiframework::IEventChannelFactory * eventChannelFactory) {
    CommonTypeReg::registerTypes(eventFactory, eventChannelFactory);
}


std::string dllGetTFVersion() {
    return TUIFRAMEWORK_VERSION;
}


std::string dllGetDLLVersion() {
    return "1";
}
