#pragma once
#include "GasMaskPeds.h"

namespace GasMaskPeds {
	typedef void (*CheckIfBlocked)(uintptr_t poolEntry, uintptr_t ped, Rage::CEvent* event, uintptr_t param_4);

	CheckIfBlocked origCheckIfBlocked;
	void BlockSmokeEvents(uintptr_t poolEntry, uintptr_t ped, Rage::CEvent* event, uintptr_t param_4);
	int32_t SmokeProofOffset;
	float SmokeVisualRange = 10.0625f;
}