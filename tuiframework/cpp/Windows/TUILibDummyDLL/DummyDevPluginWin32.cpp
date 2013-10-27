#include <tuiframework/server/DeviceFactorySingleton.h>
#include <tuiframework/server/MSPFactorySingleton.h>

#include <tuiframework/core/Version.h>
#include <tuiframework/core/IDevice.h>
#include <tuiframework/core/IMSP.h>
#include <tuiframework/server/DeviceConfig.h>
#include <tuiframework/server/MSPConfig.h>
#include <tuiframework/server/IDeviceFactory.h>

#include <dummy/DummyDevDA.h>
#include <dummy/DummyDevTracker.h>
#include <dummy/DummyDevMatrix4.h>
#include <dummy/DummyDevPackedVector3.h>

using namespace tuidevices;
using namespace tuiframework;

//namespace tuiframework {

static std::vector<std::string> deviceTypeNameVector;
static std::vector<std::string> mspTypeNameVector;

void dllInitPlugin() {
    DeviceFactorySingleton::getInstance()->registerCreateFunction(DummyDevDA::getDeviceName(), DummyDevDA::createFunction);
    DeviceFactorySingleton::getInstance()->registerCreateFunction(DummyDevTracker::getDeviceName(), DummyDevTracker::createFunction);
    DeviceFactorySingleton::getInstance()->registerCreateFunction(DummyDevMatrix4::getDeviceName(), DummyDevMatrix4::createFunction);
    DeviceFactorySingleton::getInstance()->registerCreateFunction(DummyDevPackedVector3::getDeviceName(), DummyDevPackedVector3::createFunction);
    
    deviceTypeNameVector.push_back(DummyDevDA::getDeviceName());
    deviceTypeNameVector.push_back(DummyDevTracker::getDeviceName());
    deviceTypeNameVector.push_back(DummyDevMatrix4::getDeviceName());
    deviceTypeNameVector.push_back(DummyDevPackedVector3::getDeviceName());    
}


std::string dllGetTFVersion() {
    return TUIFRAMEWORK_VERSION;
}


std::string dllGetDLLVersion() {
    return "1";
}


void dllDeviceRegistration(IDeviceFactory * deviceFactory) {
    deviceFactory->registerCreateFunction(DummyDevDA::getDeviceName(), DummyDevDA::createFunction);
    deviceFactory->registerCreateFunction(DummyDevTracker::getDeviceName(), DummyDevTracker::createFunction);
    deviceFactory->registerCreateFunction(DummyDevMatrix4::getDeviceName(), DummyDevMatrix4::createFunction);
    deviceFactory->registerCreateFunction(DummyDevPackedVector3::getDeviceName(), DummyDevPackedVector3::createFunction);
}



const std::vector<std::string> & dllGetDeviceTypeNameVector() {
    return deviceTypeNameVector;
}


std::string dllGetDeviceTypeVersion(const std::string & deviceTypeName) {
    return "1";
}


IDevice * dllNewDeviceInstance(const std::string & deviceTypeName, DeviceConfig & deviceConfig) {
    return DeviceFactorySingleton::getInstance()->createInstance(deviceTypeName, &deviceConfig);
}


void dllDeleteDeviceInstance(IDevice * device) {
    delete device;
}


const std::vector<std::string> & dllGetMSPTypeNameVector() {
    return mspTypeNameVector;
}


std::string dllGetMSPTypeVersion(const std::string & mspTypeName) {
    return "1";
}


IMSP * dllNewMSPInstance(const std::string & mspTypeName, MSPConfig & mspConfig) {
    return MSPFactorySingleton::getInstance()->createInstance(mspTypeName, &mspConfig);
}


void dllDeleteMSPInstance(IMSP * msp) {
    delete msp;
}

//}
