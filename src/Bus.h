#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>

class mos6502; //forward declaration that allows Bus to know cpu will exist
class Cartridge;

class Bus
{
private:
    // Devices 
    mos6502* cpu = nullptr; 
    Cartridge* cartridge = nullptr;

    /* data */
    uint8_t ram[0x0800] = { 0x00 };

public:
    Bus(/* args */);
    ~Bus();
    void connect_cpu(mos6502* c);
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
    void load_string(const std::string& assembled_program, const uint16_t start_address);
    void connect_cartridge(Cartridge* cart);
};