// dllmain.cpp : Defines the entry point for the DLL application.
#include "hooking.h"
#include "sub_weapons.h"

namespace SubWeapons {
    Pattern SpawnSubtaskPattern = { "c7 45 ?? 00 00 02 80 ba c0 01 00 00 41 b8 10 00 00 00 f3 0f 11 45 ?? f3 0f 10 8e ?? ?? ?? ?? f3 0f 11 4d", 0x82 };
    Pattern CreateTaskVehicleCombatPattern = { "48 8b c4 48 89 58 08 48 89 68 10 48 89 70 18 57 48 83 ec 30 0f 29 70 e8 41 8b d8 48 8b f2 0f 28 f3 48 8b f9", 0 };
    bool EnableAiUse(void) {
        uintptr_t funcAddress = FindPattern(SpawnSubtaskPattern);
        if (funcAddress == NULL)
            return false;
        AllocateFromPool = GetReferencedAddress(funcAddress + 0xa6);
        ThrowError = GetReferencedAddress(funcAddress + 0xb5);
        CreateTaskVehicleCombat = FindPattern(CreateTaskVehicleCombatPattern);
        TaskPool = funcAddress + 0x7f + *(int32_t*)(funcAddress + 0x7b);
        if (!AllocateFromPool || !ThrowError || !CreateTaskVehicleCombat)
            return false;
        uintptr_t targetAddress = funcAddress + 0x130;
        PatchRet = InsertHookWithSkip(targetAddress, targetAddress + 3, (uintptr_t)&PatchMakeTask);
        return (PatchRet != NULL);
    }
}