#pragma once
#include <map>
#include <cstdint>
#include <string>

namespace DriveBys
{
	bool Initialize(std::map<std::string, std::string>& iniData);
	bool BlockBulletResistantSmashOuts();
	extern "C" {
		extern uintptr_t DB_SmashWindowsGun_ret;
		extern uintptr_t DB_SmashWindowsGun_patch;
		extern uintptr_t DB_SmashWindowsProjectile_ret;
		extern uintptr_t DB_SmashWindowsProjectile_patch;
		extern uint64_t DB_VehicleFlags_offset;
	}
}