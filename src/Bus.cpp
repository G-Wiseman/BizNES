#include "Bus.h"
#include "mos6502.h"
#include "Cartridge.h"
#include <sstream>
#include "Cartridge.h"

Bus::Bus(/* args */)
{

}

Bus::~Bus()
{
}

void Bus::connect_cpu(mos6502* c) {
    cpu = c;
}

void Bus::connect_cartridge(Cartridge* cart) {
    this->cartridge = cart;
}

uint8_t Bus::read(uint16_t addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        return ram[addr & 0x07FF]; // handles the mirroring of CPU's RAM
    }
    
    if (addr >= 0x8000) {
        return cartridge->read(addr);
    }
    return 0x00;
}

void Bus::write(uint16_t addr, uint8_t data) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        ram[addr & 0x07FF] = data; return;
    }

}

void Bus::load_string(const std::string& assembled_program, const uint16_t start_address) {
    // Useful to load in assembled programs from https://www.masswerk.at/6502/assembler.html
    std::istringstream iss(assembled_program);
    std::string byte_str;
    uint8_t value;
    uint16_t addr = start_address;
    while (iss >> byte_str) {
        uint8_t value = static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16));
        write(addr++, value);
    }
}