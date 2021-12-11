#pragma once
#include <map>
#include <string>
namespace MiscTweaks {
	bool EnableHighStarCopArrests();
	bool TurnOffSearchPosDrift();
	bool AllowForcedPropsInScenarios();
	bool Initialize(std::map<std::string, std::string>& iniData);
}