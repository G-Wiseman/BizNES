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

    mapper = std::make_unique<Mapper_000>(header.prg, header.chr);
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
        bool approved = false;
        size_t mapped_addr = mapper->cpuReadMapped(addr, approved);
        if (approved) {
            return PRG_Vector[mapped_addr];
        }
    }

    // If address is outside cartridge range, return open bus
    return 0xFF;
}

void Cartridge::cpuWrite(uint16_t addr, uint8_t value) {
    bool approved = false;
    size_t mapped_addr = mapper->cpuWriteMapped(addr, approved);
    if (approved) {
        PRG_Vector[mapped_addr] = value; // TODO: Is this correct? For mapper 0 it won't matter, come back when doing other mappers.
    }
    return;
}

uint8_t Cartridge::ppuRead(uint16_t addr)
{
    // TODO: double check this when you implement the PPU
    bool approved = false;
    size_t mapped_addr = mapper->ppuReadMapped(addr, approved);
    if (approved) {
        return CHR_Vector[mapped_addr];
    }
    return 0xff;
}

void Cartridge::ppuWrite(uint16_t addr, uint8_t value)
{
    // TODO: double check this when you implement the PPU
    bool approved = false;
    size_t mapped_addr = mapper->ppuWriteMapped(addr, approved);
    if (approved) {
        CHR_Vector[mapped_addr] = value;
    }
}

