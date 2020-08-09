// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "combat_tweaks.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    Global::MH_success = false;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Global::SetupHook();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

