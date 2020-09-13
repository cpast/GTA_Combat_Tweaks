// dllmain.cpp : Defines the entry point for the DLL application.
#include "hooking.h"
#include "sub_weapons.h"
#include "../combat_tweaks/combat_tweaks.h"
#include <Windows.h>

namespace SubWeapons {
    Pattern SpawnSubtaskPattern = { "c7 45 ?? 00 00 02 80 ba c0 01 00 00 41 b8 10 00 00 00 f3 0f 11 45 ?? f3 0f 10 8e ?? ?? ?? ?? f3 0f 11 4d", 0x82 };
    Pattern CreateTaskVehicleCombatPattern = { "48 8b c4 48 89 58 08 48 89 68 10 48 89 70 18 57 48 83 ec 30 0f 29 70 e8 41 8b d8 48 8b f2 0f 28 f3 48 8b f9", 0 };
    Pattern SwitchWeaponsPattern = { "48 8b 83 ?? ?? ?? ?? 48 85 c0 0f 84 ?? ?? ?? ?? 83 b8 ?? ?? ?? ?? 05 0f 85", 0x14b };

    bool Initialize(std::map<std::string, std::string>& iniData)
    {
        int enabled = Global::SafeGetInt(iniData, "Enabled");
        if (enabled == -1) {
            return false;
        }
        if (enabled == 0) {
            return true;
        }
        if (Global::SafeGetInt(iniData, "SubWeaponsAutoSwitch", 0) != 0) {
            if (!EnableAutoSwitch())
                return false;
        }
        if (Global::SafeGetInt(iniData, "EnableAiWeaponUse", 0) != 0) {
            if (!EnableAiUse())
                return false;
        }
        if (Global::SafeGetInt(iniData, "EnableDaytimeSearchlights", 0) != 0) {
            if (!EnableDaytimeSearchlights())
                return false;
        }
        return true;
    }

    bool EnableAutoSwitch(void)
    {
        uintptr_t funcAddress = FindPattern(SwitchWeaponsPattern);
        if (funcAddress == NULL)
            return false;
        SW_PatchAutoSwitchRetFail = GetReferencedAddress(funcAddress + 0x155);
        SW_VehicleTypeOffset = *(int32_t*)(funcAddress + 0x15d);
        if (SW_PatchAutoSwitchRetFail == NULL || SW_VehicleTypeOffset == NULL)
            return false;
        SW_PatchAutoSwitchRetSucceed = InsertHookWithSkip(funcAddress + 0x15b, funcAddress + 0x168, (uintptr_t)&SW_PatchAutoSwitch);
        if (SW_PatchMakeTaskRet == NULL)
            return false;
        return true;
    }

    bool EnableAiUse(void)
    {
        uintptr_t funcAddress = FindPattern(SpawnSubtaskPattern);
        if (funcAddress == NULL)
            return false;
        SW_AllocateFromPool = GetReferencedAddress(funcAddress + 0xa6);
        SW_ThrowError = GetReferencedAddress(funcAddress + 0xb5);
        SW_CreateTaskVehicleCombat = FindPattern(CreateTaskVehicleCombatPattern);
        SW_TaskPool = funcAddress + 0x7f + *(int32_t*)(funcAddress + 0x7b);
        if (!SW_AllocateFromPool || !SW_ThrowError || !SW_CreateTaskVehicleCombat)
            return false;
        uintptr_t targetAddress = funcAddress + 0x130;
        SW_PatchMakeTaskRet = InsertHookWithSkip(targetAddress, targetAddress + 3, (uintptr_t)&SW_PatchMakeTask);
        if (SW_PatchMakeTaskRet == NULL)
            return false;
        return true;
    }

    bool EnableDaytimeSearchlights(void)
    {
        Pattern duskCheckerPattern = { "b9 3f 00 f8 00 e8 ?? ?? ?? ?? 45 0f 57 c0 44 0f 28 d0", 0x0 };
        uintptr_t duskChecker = FindPattern(duskCheckerPattern);
        if (duskChecker == NULL)
            return false;
        uintptr_t funcaddr = duskChecker + 0xa;
        funcaddr += *(int*)(funcaddr - 4);
        SW_GetDuskCheck_addr = funcaddr;
        SW_GetDuskCheck_ret = InsertHookWithSkip(duskChecker, duskChecker + 0xa, (uintptr_t)&SW_GetDuskCheck_patch);
        if (SW_GetDuskCheck_ret == NULL)
            return false;
        return true;
    }
}