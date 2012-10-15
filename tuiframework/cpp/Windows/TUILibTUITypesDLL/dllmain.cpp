// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

/*

LIBRARY	"TUILibTUITypesDLL"

EXPORTS
	dllInitPlugin				@1
    dllGetTFVersion				@2
    dllGetDLLVersion			@3
    dllTypeRegistration			@4
    dllDeviceRegistration		@5
    dllMSPRegistration			@6
    dllGetDeviceTypeNameVector	@7
    dllGetDeviceTypeVersion		@8
    dllNewDeviceInstance		@9
    dllDeleteDeviceInstance		@10
    dllGetMSPTypeNameVector		@11
    dllGetMSPTypeVersion		@12
    dllNewMSPInstance			@13
    dllDeleteMSPInstance		@14
*/