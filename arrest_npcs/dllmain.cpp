// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "hooking.h"
void apply_patch() {
    Pattern ptn = { "48 85 c0 74 ?? 83 b8 ?? ?? ?? ?? 01 7f ?? 32 c0 48 83 c4 20", 0x80 };
    uintptr_t loc = FindPattern(ptn);
    if (loc == NULL)
        return;
    NopInstruction(loc + 0x85);
    NopInstruction(loc + 0x8c);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        apply_patch();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

