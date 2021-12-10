// DriveBys.cpp : Defines the functions for the static library.
//

#include "DriveBys.h"
#include <hooking.h>
#include "../combat_tweaks/combat_tweaks.h"

namespace DriveBys
{

	bool BlockBulletResistantSmashOuts() {
		Pattern smashGun = { "48 8b 43 ?? 8b 88 ?? ?? ?? ?? c1 e9 0c 41 84 cd", 0xe1 };
		Pattern smashProj = { "48 8b 47 ?? 8b 88 ?? ?? ?? ?? c1 e9 0c 41 84 cc", 0xd9 };
		uintptr_t gunLoc = FindPattern(smashGun);
		if (gunLoc == NULL)
			return false;
		uintptr_t projLoc = FindPattern(smashProj);
		if (projLoc == NULL)
			return false;
		DB_VehicleFlags_offset = *(uint32_t*)(gunLoc + 0xe7);
		uintptr_t retLoc = InsertHookWithSkip(gunLoc + 0xe1, gunLoc + 0xf1, (uintptr_t)&DB_SmashWindowsGun_patch);
		if (retLoc == NULL)
			return false;
		DB_SmashWindowsGun_ret = retLoc;
		retLoc = InsertHookWithSkip(projLoc + 0xd9, projLoc + 0xe9, (uintptr_t)&DB_SmashWindowsProjectile_patch);
		if (retLoc == NULL)
			return false;
		DB_SmashWindowsProjectile_ret = retLoc;
		return true;
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
		enabled = Global::SafeGetInt(iniData, "DontBreakBulletResistantWindows", 0);
		if (enabled != 0)
			if (!BlockBulletResistantSmashOuts())
				return false;
		return true;
	}
}