#pragma once
#include <map>
#include <string>

namespace Global
{
	extern bool MH_success;
	int SafeGetInt(const std::map<std::string, std::string>& iniData, const std::string& key);
	int SafeGetInt(const std::map<std::string, std::string>& iniData, const std::string& key, int fallback);
	bool SetupHook();
}