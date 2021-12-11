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
	
	bool EliminateSwatAutoRestriction(int hangersType)
	{
		Pattern swatDispatchPassengerPtn = { "48 89 5c 24 08 48 89 6c 24 10 48 89 74 24 18 57 41 56 41 57 48 83 ec 50 48 8b 41", 0 };
		uintptr_t swatLoc = FindPattern(swatDispatchPassengerPtn);
		if (swatLoc == NULL)
			return false;
		if (!NopInstruction(swatLoc + 0x47))
			return false;
		uint8_t jmp = 0xeb;
		int8_t offset = 0;
		offset = *(int8_t*)(swatLoc + 0xa1);
		offset -= 0x10;
		if (hangersType == 0)
		{
			return NopInstruction(swatLoc + 0xb0) != NULL;
		}
		else if (hangersType == 1)
		{
			return WriteForeignMemory(swatLoc + 0xb1, &offset, 1);
		}
		else if (hangersType == 2)
		{
			return WriteForeignMemory(swatLoc + 0xa0, &jmp, 1);
		}
		return false;
	}

	bool EnableFourManSwat()
	{
		Pattern swatDispatchPassengerPtn = { "48 89 5c 24 08 48 89 6c 24 10 48 89 74 24 18 57 41 56 41 57 48 83 ec 50 48 8b 41", 0 };
		uintptr_t swatLoc = FindPattern(swatDispatchPassengerPtn);
		if (swatLoc == NULL)
			return false;
		int32_t one = 1;
		return WriteForeignMemory(swatLoc + 0x137, &one, 4);
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

	bool EnableDirtRoadRoadblocks()
	{
		Pattern getSpawnPointsPtn = { "c7 45 b4 01 00 00 00 44 88 7d b8", 0x1a0 };
		Pattern spawnPtn = { "44 89 65 a1 c7 45 a5 00 01 00 01", 0x24f };
		Pattern spawnPtn2 = { "48 85 d2 74 ?? 0f bf 42 1c 48 8d 4c 24 20", 0x338 };
		uintptr_t getSpawnPointsLoc = FindPattern(getSpawnPointsPtn);
		uintptr_t spawnLoc = FindPattern(spawnPtn);
		uintptr_t spawnLoc2 = FindPattern(spawnPtn2);
		if (getSpawnPointsLoc == NULL || spawnLoc == NULL || spawnLoc2 == NULL)
			return false;
		uint8_t getSpawnPointsOverwrite[8] = { 0x01, 0x00, 0x01, 0x00, 0xC6, 0x45, 0xB8, 0x01 };
		if (!WriteForeignMemory(getSpawnPointsLoc + 0x1a3, getSpawnPointsOverwrite, 8))
			return false;
		DT_RoadblockDirtRoads_ret = InsertHookWithSkip(spawnLoc + 0x24f, spawnLoc + 0x253, (uintptr_t)&DT_RoadblockDirtRoads_patch);
		DT_RoadblockWater_ret_z = GetReferencedAddress(spawnLoc2 + 0x33b);
		DT_RoadblockWater_ret_nz = InsertHook(spawnLoc2 + 0x33d, (uintptr_t)&DT_RoadblockWater_patch);
		return (DT_RoadblockDirtRoads_ret != NULL && DT_RoadblockWater_ret_nz != NULL && DT_RoadblockWater_ret_z != NULL);
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
		if (enabled != 0)
		{
			int disableHangers = Global::SafeGetInt(iniData, "DisableSwatStraphangers", 0);
			if (!EliminateSwatAutoRestriction(disableHangers))
				return false;
		}
		enabled = Global::SafeGetInt(iniData, "FourManSwatCars", 0);
		if (enabled != 0)
			if (!EnableFourManSwat())
				return false;
		enabled = Global::SafeGetInt(iniData, "DisableForcedAmbientCopLoadout", 0);
		if (enabled != 0)
			if (!DisableForcedAmbientLoadout())
				return false;
		enabled = Global::SafeGetInt(iniData, "RandomizeVehicleOccupants", 0);
		if (enabled != 0)
			if (!EnableRandomizeVehicleOccupants())
				return false;
		enabled = Global::SafeGetInt(iniData, "EnableDirtRoadRoadblocks", 0);
		if (enabled != 0)
			if (!EnableDirtRoadRoadblocks())
				return false;
		return true;
	}
}