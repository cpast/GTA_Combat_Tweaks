#pragma once

namespace DispatchTuning
{
	bool Initialize(std::map<std::string, std::string>& iniData);
	bool SetupPoliceAutoFlagReq();
	bool DisableForcedAmbientLoadout();
	extern "C"
	{
		extern uintptr_t DT_SpawnPolicePassengers_ret;
		extern uintptr_t DT_SpawnPolicePassengers_patch;
	}
}