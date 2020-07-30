#include "boat_weapons.h"
#include <cstdint>
#include "hooking.h"

namespace BoatWeapons {
    bool EnableBoatWeapons() {
        Pattern boatCheck = { "41 83 b9 ?? ?? ?? ?? 0d 75 03 b0 01 c3", 0x28 };
        uintptr_t boatCheckFunc = FindPattern(boatCheck);
        if (boatCheckFunc == 0)
            return false;
        uint8_t check_radar[8] = { 0x41, 0x81, 0x78, 0x10, 0x7E, 0x31, 0x76, 0xD2 };
        return WriteForeignMemory(boatCheckFunc + 0x28, check_radar, 8);
    }
}