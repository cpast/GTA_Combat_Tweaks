#include "hooking.h"
#include "misc_tweaks.h"

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
}