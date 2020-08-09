// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "hooking.h"
#include "../boat_weapons/boat_weapons.h"
#include "../sub_weapons/sub_weapons.h"
#include "../WantedRadius/WantedRadius.h"
#include "misc_tweaks.h"
#include <MinHook.h>

bool MH_success = false;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MH_success = MH_Initialize() == MH_OK;
        BoatWeapons::EnableBoatWeapons();
        MiscTweaks::EnableHighStarCopArrests();
        MiscTweaks::TurnOffSearchPosDrift();
        WantedRadius::EnableWantedRadius();
        WantedRadius::SetupWantedRadiusColorHook();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

