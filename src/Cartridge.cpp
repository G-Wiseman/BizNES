#pragma once
#include "Cartridge.h"
#include "Bus.h"
#include "mos6502.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <stdexcept>
#include <Mapper.h>
#include <Mapper_000.h>



void Cartridge::load_from_ines(const std::string filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    // Read iNES header (16 bytes)
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.flag6 && (1 << 1)) {
        file.seekg(512, std::ios::cur); // skip trainer
    }

    PRG_Vector.resize(16384 * header.prg);
    CHR_Vector.resize(8192 * header.chr);
    file.read(reinterpret_cast<char*>(PRG_Vector.data()), 16384 * header.prg);
    file.read(reinterpret_cast<char*>(CHR_Vector.data()), 8192 * header.chr);
    file.close();
}


Cartridge::Cartridge()
{
    bus = nullptr;
}

Cartridge::~Cartridge()
{
}

uint8_t Cartridge::cpuRead(uint16_t addr) {
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        size_t mapped_addr;

        if (header.prg == 1) {
            // 16KB PRG ROM, mirror it
            mapped_addr = addr & 0x3FFF; // mask to 16KB
        }
        else {
            // 32KB PRG ROM
            mapped_addr = addr - 0x8000;
        }

        return PRG_Vector[mapped_addr];
    }

    // If address is outside cartridge range, return open bus
    return 0xFF;
}

void Cartridge::cpuWrite(uint16_t addr, uint8_t value) {
    // TODO: Finish this function
    return;
}

uint8_t Cartridge::ppuRead(uint16_t addr)
{
    return 0;
}

void Cartridge::ppuWrite(uint16_t addr, uint8_t value)
{
    return;
}

