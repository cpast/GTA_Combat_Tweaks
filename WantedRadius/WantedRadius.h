#pragma once
#include <cstdint>
#include <map>
#include <string>
namespace WantedRadius
{
	bool Initialize(std::map<std::string, std::string>& iniData);
	bool EnableWantedRadius();
	bool SetupWantedRadiusColorHook();
	extern "C" {
		extern uintptr_t WLR_Minimap_Return;
		extern uintptr_t WLR_Minimap_Update;
		extern uint32_t WLR_Flash_Time;
	}
}