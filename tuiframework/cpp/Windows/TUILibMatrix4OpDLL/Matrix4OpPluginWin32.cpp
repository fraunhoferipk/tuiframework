#include <tuiframework/server/MSPFactorySingleton.h>

#include <tuiframework/core/Version.h>
#include <tuiframework/core/Imsp.h>
#include <tuiframework/server/mspConfig.h>
#include <tuiframework/server/IMSPFactory.h>

#include <matrix4op/Matrix4DuplicateMSP.h>
#include <matrix4op/Matrix4GetTranslationMSP.h>
#include <matrix4op/Matrix4InverseMSP.h>
#include <matrix4op/Matrix4MultiplicationMSP.h>
#include <matrix4op/Matrix4PackedDuplicateMSP.h>
#include <matrix4op/Matrix4SetTranslationMSP.h>
#include <matrix4op/Matrix4TimesMSP.h>
#include <matrix4op/Vector4DuplicateMSP.h>
#include <matrix4op/Vector4Matrix4MultiplicationMSP.h>
#include <matrix4op/Vector4PackedDuplicateMSP.h>
#include <matrix4op/Vector4TimesMSP.h>

#include <vector>

using namespace tuiframework;

namespace tuiframework {

static std::vector<std::string> mspTypeNameVector;

void dllInitPlugin() {
	MSPFactorySingleton::getInstance()->registerCreateFunction(Matrix4DuplicateMSP::getMSPTypeName(), Matrix4DuplicateMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Matrix4GetTranslationMSP::getMSPTypeName(), Matrix4GetTranslationMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Matrix4InverseMSP::getMSPTypeName(), Matrix4InverseMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Matrix4MultiplicationMSP::getMSPTypeName(), Matrix4MultiplicationMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Matrix4PackedDuplicateMSP::getMSPTypeName(), Matrix4PackedDuplicateMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Matrix4SetTranslationMSP::getMSPTypeName(), Matrix4SetTranslationMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Matrix4TimesMSP::getMSPTypeName(), Matrix4TimesMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Vector4DuplicateMSP::getMSPTypeName(), Vector4DuplicateMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Vector4Matrix4MultiplicationMSP::getMSPTypeName(), Vector4Matrix4MultiplicationMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Vector4PackedDuplicateMSP::getMSPTypeName(), Vector4PackedDuplicateMSP::createFunction);
	MSPFactorySingleton::getInstance()->registerCreateFunction(Vector4TimesMSP::getMSPTypeName(), Vector4TimesMSP::createFunction);


    mspTypeNameVector.push_back(Matrix4DuplicateMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Matrix4GetTranslationMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Matrix4InverseMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Matrix4MultiplicationMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Matrix4PackedDuplicateMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Matrix4SetTranslationMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Matrix4TimesMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Vector4DuplicateMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Vector4Matrix4MultiplicationMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Vector4PackedDuplicateMSP::getMSPTypeName());
	mspTypeNameVector.push_back(Vector4TimesMSP::getMSPTypeName());
}


std::string dllGetTFVersion() {
    return TUIFRAMEWORK_VERSION;
}


std::string dllGetDLLVersion() {
    return "1";
}


void dllMSPRegistration(IMSPFactory * MSPFactory) {
    MSPFactory->registerCreateFunction(Matrix4DuplicateMSP::getMSPTypeName(), Matrix4DuplicateMSP::createFunction);
	MSPFactory->registerCreateFunction(Matrix4GetTranslationMSP::getMSPTypeName(), Matrix4GetTranslationMSP::createFunction);
	MSPFactory->registerCreateFunction(Matrix4InverseMSP::getMSPTypeName(), Matrix4InverseMSP::createFunction);
	MSPFactory->registerCreateFunction(Matrix4MultiplicationMSP::getMSPTypeName(), Matrix4MultiplicationMSP::createFunction);
	MSPFactory->registerCreateFunction(Matrix4PackedDuplicateMSP::getMSPTypeName(), Matrix4PackedDuplicateMSP::createFunction);
	MSPFactory->registerCreateFunction(Matrix4SetTranslationMSP::getMSPTypeName(), Matrix4SetTranslationMSP::createFunction);
	MSPFactory->registerCreateFunction(Matrix4TimesMSP::getMSPTypeName(), Matrix4TimesMSP::createFunction);
	MSPFactory->registerCreateFunction(Vector4DuplicateMSP::getMSPTypeName(), Vector4DuplicateMSP::createFunction);
	MSPFactory->registerCreateFunction(Vector4Matrix4MultiplicationMSP::getMSPTypeName(), Vector4Matrix4MultiplicationMSP::createFunction);
	MSPFactory->registerCreateFunction(Vector4PackedDuplicateMSP::getMSPTypeName(), Vector4PackedDuplicateMSP::createFunction);
	MSPFactory->registerCreateFunction(Vector4TimesMSP::getMSPTypeName(), Vector4TimesMSP::createFunction);
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
