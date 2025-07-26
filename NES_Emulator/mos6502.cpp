#include "mos6502.h"
#include "Bus.h"

mos6502::mos6502(Bus* b) {
    bus = b;


    using cpu = mos6502; 
    
}

mos6502::~mos6502()
{

}

uint8_t mos6502::read(uint16_t addr) {
    return bus->read(addr);
}

void mos6502::write(uint16_t addr, uint8_t data) {
    bus->write(addr, data);
}

void mos6502::clock_cycle() {
    // This should be once for each cpu clock cycle being performed. 

    // Loop: 
    if (cycles_remaining == 0) {
        // fetch byte at pc counter. 
        fetched = read(pc); pc++;
        // decode what address mode AND operation it is. 
        // get-addr-mode[fetched] //lookup table?
        // get-instruction[fetched] //lookup table? 

        
        // if a value is needed for the operation, use the address mode to get it
        // fetched = 
        // determine how many cycles it will take.
        // run FULL operation right now. 
    }
    cycles_remaining--; 

};

void mos6502::reset() {
    // This should set all the registers to a known state. As well, it should set the pc to a hardcoded value, like the interrupts. 
    // Sets pc to $FFFC & FFFD
};

void mos6502::masked_interrupt() {
    // A potential interrupt. This will only work if interrupts are enable. Store current values to the stack and then set the pc to a HARDCODED place in memory. 
    // This is called IRQ
    // Hijacks pc to $FFFE and $FFFF
};

void mos6502::non_masked_interrupt() {
    // This is non skippable interrupt. Store current values to the stack, and set the pc to a HARDCODED space in memory for handling the interrupt.
    // This is called NMI
    // Hijacks pc to $FFFA and FFFB
};

void mos6502::return_from_interrupt() {
    // This should return the cpu to the state it was in before it was last interrupted. Restore values to registers from it's stack. 
}

uint8_t mos6502::ADC()
{
    return 0;
}
