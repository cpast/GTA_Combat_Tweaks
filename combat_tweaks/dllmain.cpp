// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "hooking.h"
#include "../boat_weapons/boat_weapons.h"
#include "../sub_weapons/sub_weapons.h"
#include "misc_tweaks.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        BoatWeapons::EnableBoatWeapons();
        SubWeapons::EnableAiUse();
        MiscTweaks::EnableHighStarCopArrests();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

