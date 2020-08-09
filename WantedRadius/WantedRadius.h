#pragma once
#include <cstdint>
namespace WantedRadius
{
	bool EnableWantedRadius();
	bool SetupWantedRadiusColorHook();
	extern "C" {
		extern uintptr_t WLR_Minimap_Return;
		extern uintptr_t WLR_Minimap_Update;
		extern uint32_t WLR_Flash_Time;
	}
}