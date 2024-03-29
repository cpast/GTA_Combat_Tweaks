#include "hooking.h"
#include "misc_tweaks.h"
#include "combat_tweaks.h"

namespace MiscTweaks {
    bool EnableHighStarCopArrests() {
        Pattern ptn = { "48 85 c0 74 ?? 83 b8 ?? ?? ?? ?? 01 7f ?? 32 c0 48 83 c4 20", 0x80 };
        uintptr_t loc = FindPattern(ptn);
        if (loc == NULL)
            return false;
        if (!NopInstruction(loc + 0x85))
            return false;
        if (!NopInstruction(loc + 0x8c))
            return false;
        return true;
    }

    bool TurnOffSearchPosDrift() {
        uint8_t bytes[5] = { 0x45, 0x0F, 0x57, 0xC0, 0x90 };
        Pattern ptn = { "0f c6 e3 55 f3 0f 59 d2 0f c6 eb aa f3 44 0f 59 c6 f3 0f 5c cc f3 0f 5c c5 f3 0f 59 c9 f3 0f 59 c0 f3 0f 58 ca f3 0f 58 c8 0f 51 d1 a8 01", 0x10d };
        Pattern ref120 = { "66 0f 6e c8 0f 5b c9 0f 28 c1 f3 0f 59 c6 0f 2f c2", 0x888 };
        uintptr_t loc = FindPattern(ptn);
        uintptr_t loc120 = FindPattern(ref120);
        if (loc == NULL || loc120 == NULL)
            return false;
        intptr_t off120 = *(int32_t*)(loc120 + 0x884) + loc120 + 0x888;
        if (!WriteForeignMemory(loc + 0x119, bytes, 5))
            return false;
        intptr_t offset2 = off120 - loc;
        int32_t offset = (int32_t)offset2 - 0x15f;
        if (!WriteForeignMemory(loc + 0x15b, &offset, 4))
            return false;
        offset2 = off120 - loc;
        offset = (int32_t)offset2;
        offset -= 0x155;
        return WriteForeignMemory(loc + 0x151, &offset, 4);
    }

    bool AllowForcedPropsInScenarios()
    {
        Pattern ptn = { "8b 4c 04 54 85 4c 94 54 75 59 49 03 d1 49 3b d1", 0x52f };
        uint8_t jmp = 0xeb;
        uintptr_t loc = FindPattern(ptn);
        if (loc == NULL)
            return false;
        return WriteForeignMemory(loc + 0x537, &jmp, 1);
    }

    bool Initialize(std::map<std::string, std::string>& iniData)
    {
        int enabled = Global::SafeGetInt(iniData, "EnableCopArrestsAboveOneStar", 0);
        if (enabled != 0) {
            if (!EnableHighStarCopArrests())
                return false;
        }
        enabled = Global::SafeGetInt(iniData, "DisableCopSixthSense", 0);
        if (enabled != 0) {
            return TurnOffSearchPosDrift();
        }
        enabled = Global::SafeGetInt(iniData, "AllowForcedPropsInScenarios", 0);
        if (enabled != 0) {
            return AllowForcedPropsInScenarios();
        }
        return true;
    }

}