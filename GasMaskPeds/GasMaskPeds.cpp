// GasMaskPeds.cpp : Defines the functions for the static library.
//

#include "GasMaskPeds.h"
#include "GasMaskPeds_internal.h"
#include "../combat_tweaks/combat_tweaks.h"
#include "../combat_tweaks/rage_structs.h"
#include <hooking.h>
#include <MinHook.h>

namespace GasMaskPeds
{
	bool MainSetup() {
		Pattern offsetPattern = { "74 ?? 83 f9 07 75 ?? f6 87 ?? ?? ?? ?? 01", 0x0 };
		uintptr_t offsetLoc = FindPattern(offsetPattern);
		if (offsetLoc == NULL)
			return false;
		int32_t* offsetPtr = (int32_t*)(offsetLoc + 0x9);
		SmokeProofOffset = *offsetPtr;
		return true;
	}

	bool inline IsSmokeExplosion(Rage::CEvent* event) {
		return *(float*)((uintptr_t)event + 0x60) == SmokeVisualRange;
	}

	void BlockSmokeEvents(uintptr_t poolEntry, uintptr_t ped, Rage::CEvent* event, uintptr_t param_4) {
		if (event->vftable->GetID(event) == (int32_t)Rage::CEventID::CEventShockingExplosion) {
			if (isPedSmokeProof(ped) && IsSmokeExplosion(event)) {
				*(int16_t*)(param_4) = -1;
				*(int8_t*)(param_4 + 2) = 0;
				return;
			}
		}
		origCheckIfBlocked(poolEntry, ped, event, param_4);
	}

	bool SetupIgnoreSmoke() {
		Pattern blockPattern = { "48 89 5c 24 08 48 89 6c 24 10 48 89 74 24 18 57 41 56 41 57 48 83 ec 30 41 83 cf ff 41", 0x0 };
		uintptr_t blockLoc = FindPattern(blockPattern);
		if (blockLoc == NULL)
			return false;
		origCheckIfBlocked = (CheckIfBlocked) InsertHook(blockLoc, (uintptr_t)BlockSmokeEvents);
		return origCheckIfBlocked != NULL;
	}

	bool inline isPedSmokeProof(uintptr_t ped)
	{
		if (ped == NULL)
			return false;
		return (*(byte*)(ped + SmokeProofOffset) & 1);
	}

	void inline setPedSmokeProof(uintptr_t ped, bool proof)
	{
		if (ped == NULL)
			return;
		byte* flag = (byte*)(ped + SmokeProofOffset);
		*flag &= 0xfe;
		if (proof)
			*flag |= 0x01;
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
		if (!MainSetup()) {
			return false;
		}
		enabled = Global::SafeGetInt(iniData, "IgnoreSmokeClouds", 0);
		if (enabled != 0)
			return SetupIgnoreSmoke();
		SmokeVisualRange = Global::SafeGetFloat(iniData, "SmokeExplosionVisualRangeOverride", 10.0625f);
		return true;
	}
}