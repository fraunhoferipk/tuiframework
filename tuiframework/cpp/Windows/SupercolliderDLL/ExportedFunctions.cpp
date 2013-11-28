#include "ExportedFunctions.h"


SuperColliderWrapper::SoundManager* supercollider::soundDllCreateSoundManager(const string cfgFilename, const string gradationFilename) {
	SuperColliderWrapper::SoundManager* soundMgr;
	soundMgr = new SuperColliderWrapper::SoundManager(cfgFilename, gradationFilename);
	return soundMgr;
}

void supercollider::soundDllPlay(SuperColliderWrapper::SoundManager* soundMgr, int dimension, std::vector<int> gradations) {
	soundMgr->play(dimension, gradations);
}

void supercollider::soundDllDeleteSoundManager(SuperColliderWrapper::SoundManager* sm){
	delete sm;
}

void supercollider::soundDllGetMaxGradations(SuperColliderWrapper::SoundManager* soundMgr, std::vector<int>& maxgrads){

	soundMgr->getNumberGradations(maxgrads);

}
