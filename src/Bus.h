#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>

class mos6502; //forward declaration that allows Bus to know cpu will exist
class Cartridge;
class PPU;

class Bus
{
private:
    // Devices 
    mos6502* cpu = nullptr; 
    PPU* ppu = nullptr;
    Cartridge* cartridge = nullptr;
    uint8_t ram[0x0800] = { 0x00 };

public:
    Bus(/* args */);
    ~Bus();
    uint8_t cpuRead(uint16_t addr);
    void cpuWrite(uint16_t addr, uint8_t data);
    void load_string(const std::string& assembled_program, const uint16_t start_address);
    void clock(); 

public:
    void connect_cpu(mos6502* c);
    void connect_cartridge(Cartridge* cart);
    void connect_ppu(PPU* p);
};