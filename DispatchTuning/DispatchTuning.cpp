// DispatchTuning.cpp : Defines the functions for the static library.
//

#include "../combat_tweaks/combat_tweaks.h"
#include "DispatchTuning.h"
#include "hooking.h"

namespace DispatchTuning
{
	Pattern policePassengerSpawnPattern = { "41 8b f1 45 8a d0 c1 e8 1f 40 8a ea 41 84 c4", 0x30 };
	Pattern createCopPattern = { "83 c8 01 ba ea 33 1b b0", 0x70 };
	Pattern pickCarDriverPattern = { "41 23 c5 33 d0 81 e2 00 00 00 10 33 c2 23 c6 41 23 c6", 0x67 };
	
	bool EliminateSwatAutoRestriction(bool preserveHangers)
	{
		Pattern swatDispatchPassengerPtn = { "48 89 5c 24 08 48 89 6c 24 10 48 89 74 24 18 57 41 56 41 57 48 83 ec 50 48 8b 41", 0 };
		uintptr_t swatLoc = FindPattern(swatDispatchPassengerPtn);
		if (swatLoc == NULL)
			return false;
		uint8_t jmp = 0xeb;
		int8_t offset = 0;
		offset = *(int8_t*)(swatLoc + 0xa1);
		offset -= 0x10;
		if (!WriteForeignMemory(swatLoc + 0xe0, &jmp, 1))
			return false;
		if (!WriteForeignMemory(swatLoc + 0xe1, &offset, 1))
			return false;
		if (!preserveHangers)
			return WriteForeignMemory(swatLoc + 0xd0, &jmp, 1);
		return true;
	}

	bool SetupPoliceAutoFlagReq()
	{
		uintptr_t policeSpawnLoc = FindPattern(policePassengerSpawnPattern);
		if (policeSpawnLoc == NULL)
			return false;
		DT_SpawnPolicePassengers_ret = InsertHook(policeSpawnLoc + 0x30, (uintptr_t)&DT_SpawnPolicePassengers_patch);
		return (DT_SpawnPolicePassengers_ret != NULL);
	}

	bool DisableForcedAmbientLoadout()
	{
		uintptr_t createCopLoc = FindPattern(createCopPattern);
		if (createCopLoc == NULL)
			return false;
		return (NopInstruction(createCopLoc + 0x8f) != NULL);
	}

	bool EnableRandomizeVehicleOccupants()
	{
		if (gtaRand == NULL)
			return false;
		DT_gtaRand = (uintptr_t)gtaRand;
		uintptr_t pickDriverLoc = FindPattern(pickCarDriverPattern);
		if (pickDriverLoc == NULL)
			return false;
		DT_ClassOffsets_CVehicleModelInfo_NumPeds = *(int32_t*)(pickDriverLoc + 0x8e);
		DT_GetVehicleOccupant_ret = InsertHookWithSkip(pickDriverLoc + 0x81, pickDriverLoc + 0x8a, (uintptr_t)&DT_GetVehicleOccupant_patch);
		return (DT_GetVehicleOccupant_ret != NULL);
	}

	bool Initialize(std::map<std::string, std::string>& iniData)
	{
		int enabled = Global::SafeGetInt(iniData, "Enabled");
		if (enabled == -1) {
			return false;
		}
		if (enabled == 0) {
			return true;
		}
		enabled = Global::SafeGetInt(iniData, "DisablePoliceAutomobileFlagRequirement", 0);
		if (enabled != 0)
			if (!SetupPoliceAutoFlagReq())
				return false;
		enabled = Global::SafeGetInt(iniData, "RemoveSwatAutomobileRestriction", 0);
		if (enabled != 0) {
			int preserveHangers = Global::SafeGetInt(iniData, "DisableSwatStraphangers", 0);
			if (!EliminateSwatAutoRestriction(preserveHangers != 0))
				return false;
		}
			
		enabled = Global::SafeGetInt(iniData, "DisableForcedAmbientCopLoadout", 0);
		if (enabled != 0)
			if (!DisableForcedAmbientLoadout())
				return false;
		enabled = Global::SafeGetInt(iniData, "RandomizeVehicleOccupants", 0);
		if (enabled != 0)
			if (!EnableRandomizeVehicleOccupants())
				return false;
		return true;
	}
}