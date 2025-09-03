#include "Bus.h"
#include "mos6502.h"

Bus::Bus(/* args */)
{

}

Bus::~Bus()
{
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
    }
}