#include "hooking.h"
#include "../boat_weapons/boat_weapons.h"
#include "../sub_weapons/sub_weapons.h"
#include "../WantedRadius/WantedRadius.h"
#include "misc_tweaks.h"
#include "combat_tweaks.h"
#include <MinHook.h>
#include <map>
#include <string>
#include <stdexcept>
#include <Windows.h>

#define PROCESS_MODULE(moduleName, iniName, outSuccess) { \
        std::map<std::string, std::string>* _PM_iniData = ParseIniSection((iniName)); \
        if (_PM_iniData != NULL) { \
            bool _PM_success = true;\
            _PM_success = moduleName::Initialize(*_PM_iniData); \
            (outSuccess) = (outSuccess) && _PM_success;\
            delete _PM_iniData; \
        } \
    }

bool Global::MH_success = false;

std::map<std::string, std::string>* ParseIniSection(LPCSTR sectionName) {
    LPSTR buffer = (LPSTR)malloc(32767);
    if (buffer == NULL)
        return NULL;
    char iniLocation[4096] = { '\0' };
    if (GetCurrentDirectoryA(4075, iniLocation) == 0) {
        free(buffer);
        return NULL;
    }
    strcat_s(iniLocation, 4096, "\\combat_tweaks.ini");
    DWORD numBytesRead = GetPrivateProfileSectionA(sectionName, buffer, 32767, iniLocation);
    if (numBytesRead == 0 || numBytesRead >= 32767 - 2) {
        free(buffer);
        return NULL;
    }
    size_t i = 0;
    std::map<std::string, std::string>* retVal = new std::map<std::string, std::string>();
    if (retVal == NULL) {
        free(buffer);
        return NULL;
    }
    while (i < numBytesRead) {
        char* pair = buffer + i;
        size_t pairlen = strlen(pair) + 1;
        char* equalsSign = strchr(pair, '=');
        if (equalsSign == NULL) {
            i += pairlen;
            continue;
        }
        *equalsSign = '\0';
        equalsSign++;
        std::string key(pair);
        std::string val(equalsSign);
        retVal->emplace(key, val);
        i += pairlen;
    }
    free(buffer);
    return retVal;
}

int Global::SafeGetInt(const std::map<std::string, std::string>& iniData, const std::string& key, int fallback)
{
    if (iniData.count(key) == 0)
        return fallback;
    int retVal = fallback;
    try {
        retVal = std::stoi(iniData.at(key));
    }
    catch (const std::invalid_argument& ia) {
        return fallback;
    }
    catch (const std::out_of_range& oor) {
        return fallback;
    }
    return retVal;
}

int Global::SafeGetInt(const std::map<std::string, std::string>& iniData, const std::string& key) {
    return Global::SafeGetInt(iniData, key, -1);
}

bool Global::SetupHook() {
    Global::MH_success = MH_Initialize() == MH_OK;
    bool bSuccess = true;
    PROCESS_MODULE(BoatWeapons,"Boat Weapons", bSuccess);
    PROCESS_MODULE(SubWeapons, "Submarine Weapons", bSuccess);
    PROCESS_MODULE(MiscTweaks, "Miscellaneous Tweaks", bSuccess);
    PROCESS_MODULE(WantedRadius, "Wanted Radius", bSuccess);
    return bSuccess;
}
