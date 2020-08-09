#pragma once
#include <map>
#include <string>
namespace BoatWeapons {
	bool Initialize(std::map<std::string, std::string>& iniData);
	bool EnableBoatWeapons();
}