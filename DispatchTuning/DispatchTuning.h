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
		extern uintptr_t DT_RoadblockDirtRoads_patch;
		extern uintptr_t DT_RoadblockDirtRoads_ret;
		extern uintptr_t DT_RoadblockWater_patch;
		extern uintptr_t DT_RoadblockWater_ret_nz;
		extern uintptr_t DT_RoadblockWater_ret_z;
		extern int32_t DT_ClassOffsets_CVehicleModelInfo_NumPeds;
	}
}