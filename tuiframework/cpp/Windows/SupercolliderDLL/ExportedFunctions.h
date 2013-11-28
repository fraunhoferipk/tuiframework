#include "SoundManager.h"

namespace supercollider {

SuperColliderWrapper::SoundManager* soundDllCreateSoundManager(const string cfgFilename, const string gradationFilename);
void soundDllPlay(SuperColliderWrapper::SoundManager* soundMgr, int dimension, std::vector<int> gradations);
void soundDllDeleteSoundManager(SuperColliderWrapper::SoundManager* sm);
void soundDllGetMaxGradations(SuperColliderWrapper::SoundManager* soundMgr, std::vector<int>& maxgrads);

}