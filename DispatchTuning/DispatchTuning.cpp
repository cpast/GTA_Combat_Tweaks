// DispatchTuning.cpp : Defines the functions for the static library.
//

#include "../combat_tweaks/combat_tweaks.h"
#include "DispatchTuning.h"
#include "hooking.h"

namespace DispatchTuning
{
	Pattern policePassengerSpawnPattern = { "41 8b f1 45 8a d0 c1 e8 1f 40 8a ea 41 84 c4", 0x30 };
	Pattern createCopPattern = { "83 c8 01 ba ea 33 1b b0", 0x70 };


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
		enabled = Global::SafeGetInt(iniData, "DisableForcedAmbientCopLoadout", 0);
		if (enabled != 0)
			if (!DisableForcedAmbientLoadout())
				return false;
		return true;
	}
}