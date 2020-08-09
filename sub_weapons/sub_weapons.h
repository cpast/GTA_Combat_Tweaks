#pragma once
#include <cstdint>
#include <map>
#include <string>

namespace SubWeapons {
	extern "C" {
		extern uintptr_t SW_AllocateFromPool;
		extern uintptr_t SW_ThrowError;
		extern uintptr_t SW_CreateTaskVehicleCombat;
		extern uintptr_t SW_PatchMakeTaskRet;
		extern uintptr_t SW_TaskPool;
		extern uintptr_t SW_PatchAutoSwitchRetFail;
		extern uintptr_t SW_PatchAutoSwitchRetSucceed;
		extern uintptr_t SW_PatchMakeTask;
		extern uintptr_t SW_PatchAutoSwitch;
		extern int32_t SW_VehicleTypeOffset;
	}
	bool Initialize(std::map<std::string, std::string>& iniData);
	bool EnableAiUse();
}