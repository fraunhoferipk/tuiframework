// SupercolliderDLLTest.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "ExportedFunctions.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	SuperColliderWrapper::SoundManager* sound_mgr = supercollider::soundDllCreateSoundManager("C:\\git\\tuiframework-supercollider-2013-11-08\\tuiframework\\tuiframework\\cpp\\tuilibs\\sound\\friction.cfg", 
		"c:\\git\\tuiframework-supercollider-2013-11-08\\tuiframework\\tuiframework\\cpp\\tuilibs\\sound\\frictionSpeedGradations.cfg");

	std::vector<int> gradations = std::vector<int>();
	gradations.push_back(5); //friction number 6
	gradations.push_back(2); //speed number 3

	supercollider::soundDllPlay(sound_mgr, 0, gradations);

	supercollider::soundDllDeleteSoundManager(sound_mgr);


	return 0;
}

