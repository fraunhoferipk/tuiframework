#include <tuiframework/server/MSPFactorySingleton.h>

#include <tuiframework/core/Version.h>
#include <tuiframework/core/Imsp.h>
#include <tuiframework/server/mspConfig.h>
#include <tuiframework/server/IMSPFactory.h>

#include <matrix4depack/Matrix4SkelDepackMSP.h>

#include <vector>

using namespace tuiframework;

namespace tuiframework {

static std::vector<std::string> mspTypeNameVector;

void dllInitPlugin() {
	MSPFactorySingleton::getInstance()->registerCreateFunction(Matrix4SkelDepackMSP::getMSPTypeName(), Matrix4SkelDepackMSP::createFunction);
    mspTypeNameVector.push_back(Matrix4SkelDepackMSP::getMSPTypeName());
}


std::string dllGetTFVersion() {
    return TUIFRAMEWORK_VERSION;
}


std::string dllGetDLLVersion() {
    return "1";
}


void dllMSPRegistration(IMSPFactory * MSPFactory) {
    MSPFactory->registerCreateFunction(Matrix4SkelDepackMSP::getMSPTypeName(), Matrix4SkelDepackMSP::createFunction);
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


}
