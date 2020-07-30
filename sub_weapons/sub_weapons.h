#pragma once
#include <cstdint>
namespace SubWeapons {
	extern "C" {
		extern uintptr_t AllocateFromPool;
		extern uintptr_t ThrowError;
		extern uintptr_t CreateTaskVehicleCombat;
		extern uintptr_t PatchRet;
		extern uintptr_t TaskPool;
		extern uintptr_t PatchMakeTask;
	}

	bool EnableAiUse();
}