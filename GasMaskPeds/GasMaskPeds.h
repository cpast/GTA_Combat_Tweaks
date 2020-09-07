#pragma once
#include <map>
#include <cstdint>
#include <string>
#include "../combat_tweaks/rage_structs.h"


namespace GasMaskPeds
{
	bool Initialize(std::map<std::string, std::string>& iniData);
	bool MainSetup();
	bool SetupIgnoreSmoke();
	bool inline isPedSmokeProof(uintptr_t ped);
	void inline setPedSmokeProof(uintptr_t ped, bool proof);
}