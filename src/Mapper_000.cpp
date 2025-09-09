#pragma once
#include "Mapper_000.h"

Mapper_000::Mapper_000(uint8_t num_PRG, uint8_t num_CHR):
	Mapper(num_PRG, num_CHR){} // Uses the parent's constructor

size_t Mapper_000::cpuReadMapped(uint16_t addr, bool& approved) {
    approved = true; // If you ever need to reject an address, do it here
    if (num_PRG_banks == 1) {
        // 16KB PRG ROM, so mirror it
        return addr & 0x3FFF; // mask to 16KB
    }
    else {
        // 32KB PRG ROM
        return addr - 0x8000;
    }
}

size_t Mapper_000::cpuWriteMapped(uint16_t addr, bool& approved) {
    approved = false; // You cannot write to PRG on Mapper000
    return 0;
}

size_t Mapper_000::ppuReadMapped(uint16_t addr, bool& approved) {
	return 0;
}

size_t Mapper_000::ppuWriteMapped(uint16_t addr, bool& approved) {
    return 0;
}