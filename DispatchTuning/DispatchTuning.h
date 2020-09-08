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
		extern uintptr_t DT_GetVehicleOccupant_ret;
		extern uintptr_t DT_GetVehicleOccupant_patch;
		extern uintptr_t DT_gtaRand;
		extern int32_t DT_ClassOffsets_CVehicleModelInfo_NumPeds;
	}
}