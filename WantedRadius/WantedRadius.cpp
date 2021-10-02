// WantedRadius.cpp : Defines the functions for the static library.
//

#include "WantedRadius.h"
#include "../combat_tweaks/combat_tweaks.h"
#include "hooking.h"
#include <cstdint>

namespace WantedRadius
{
	uint32_t* (*BlipColorToHudColor)(uint32_t*, uint32_t, uint8_t, uint32_t*) = NULL;
	uint32_t* HookBlipColorToHudColor(uint32_t* outHudColor, uint32_t blipColor, uint8_t lighten, uint32_t* outColorInfo) {
		uint8_t alphaMask = 0xff;
		if (blipColor == 0x6f || blipColor == 0x70) {
			alphaMask = 100;
			blipColor -= 0x48;
		}
		uint32_t* retVal = BlipColorToHudColor(outHudColor, blipColor, lighten, outColorInfo);
		*retVal = *retVal & 0xffffff | (((*retVal >> 0x18) & alphaMask) << 0x18);
		*outHudColor = *outHudColor & 0xffffff | (((*outHudColor >> 0x18) & alphaMask) << 0x18);
		return retVal;
	}

	bool SetupWantedRadiusColorHook()
	{
		Pattern blipColorPtn = { "48 89 5c 24 ?? 48 89 74 24 ?? 48 89 7c 24 ?? 4c 63 da 48 8b f9 b3 ff 41 83 fb", 0 };
		Pattern wlrMinimapPtn = { "b9 fa 00 00 00 84 c0 41 8b c6 0f 45 d9 2b c6 3b c3", 0x1df };
		uintptr_t blipColorLoc = FindPattern(blipColorPtn);
		uintptr_t wlrMinimapLoc = FindPattern(wlrMinimapPtn);
		if (wlrMinimapLoc == NULL || blipColorLoc == NULL)
			return false;
		BlipColorToHudColor = (uint32_t* (*)(uint32_t*, uint32_t, uint8_t, uint32_t*)) InsertHook(blipColorLoc, (uintptr_t)HookBlipColorToHudColor);
		if (BlipColorToHudColor == NULL)
			return false;
		WriteForeignMemory(wlrMinimapLoc + 0x1e0, &WLR_Flash_Time, 4);
		WLR_Minimap_Return = InsertHookWithSkip(wlrMinimapLoc + 0x214, wlrMinimapLoc + 0x222, (uintptr_t)&WLR_Minimap_Update);
		if (WLR_Minimap_Return == NULL)
			return false;
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
		bool bSuccess = EnableWantedRadius();
		if (!bSuccess)
			return false;
		WLR_Flash_Time = Global::SafeGetInt(iniData, "OutOfSightFlashRate", 750);
		enabled = Global::SafeGetInt(iniData, "FlashGrayWhenNotSeen", 0);
		if (enabled != 0)
			return SetupWantedRadiusColorHook();
		return true;
	}

	bool EnableWantedRadius()
	{
		Pattern wlrFlagAccessPtn = { "03 c0 88 0d ?? ?? ?? ?? 88 0d ?? ?? ?? ?? 88 0d ?? ?? ?? ?? c6 05 ?? ?? ?? ?? 01 89 05", 0x24 };
		Pattern wloFlashPtn = { "48 83 ec 58 8b 05 ?? ?? ?? ?? 8b 15 ?? ?? ?? ?? a8 01 75 ?? 83 c8 01 89 15", 0 };
		uintptr_t wlrFlagAccessLoc = FindPattern(wlrFlagAccessPtn);
		uintptr_t wloFlashLoc = FindPattern(wloFlashPtn);
		if (wlrFlagAccessLoc == NULL || wloFlashLoc == NULL) {
			return false;
		}
		uintptr_t wlrFlagLoc = GetReferencedAddress(wlrFlagAccessLoc + 0x32);
		if (wlrFlagLoc == NULL) {
			return false;
		}
		NopInstruction(wlrFlagAccessLoc + 0x32);
		*(uint8_t*)wlrFlagLoc = 1;
		uint8_t ret = 0xc3;
		return WriteForeignMemory(wloFlashLoc, &ret, 1);
	}
}