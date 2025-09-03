#include "mos6502.h"
#include "Bus.h"
#include <array>
#include <iostream>

#define STACK_OFFSET 0x0100

#define IRQ_VECTOR_LO 0xFFFE
#define IRQ_VECTOR_HI 0xFFFF

#define RST_VECTOR_LO 0xFFFC
#define RST_VECTOR_HI 0xFFFD

#define NMI_VECTOR_LO 0xFFFA
#define NMI_VECTOR_HI 0xFFFB

mos6502::mos6502() {
    bus = nullptr;
}

mos6502::mos6502(Bus* b) {
    bus = b;
}

mos6502::~mos6502()
{

}

uint8_t mos6502::read(uint16_t addr) {
    return bus->cpuRead(addr);
}

void mos6502::write(uint16_t addr, uint8_t data) {
    bus->cpuWrite(addr, data);
}

void mos6502::internal_work() {
    // this will be called when a cycle needs to be spent 
    // but no read or write takes place
    return;
}

bool mos6502::getFlag(STATUS_FLAG_MASKS flag) {
    return (status & flag);
}    

void mos6502::setFlag(STATUS_FLAG_MASKS flag, bool value) {
    if (value) {
        status |= flag; 
    }
    else {
        status &= ~flag; 
    }
}

uint16_t mos6502::getPC() {
    return pc;
}

void mos6502::setPC(uint16_t newpc) {
    pc = newpc;
}

uint8_t mos6502::getAccumulator() {
    return a;
}

void mos6502::setAccumulator(uint8_t newa) {
    a = newa;
}

uint8_t mos6502::getXReg() {
    return x;
}

void mos6502::setXReg(uint8_t newx) {
    x = newx;
}

uint8_t mos6502::getYReg() {
    return y;
}

void mos6502::setYReg(uint8_t newy) {
    y = newy;
}


uint8_t mos6502::fetch_value() {
    if (address_was_implied) {
        return a; 
    }
    return read(address);
};

void mos6502::branch(bool condition) {
    if (condition) {
        cycles_remaining++;
        internal_work();

        if ((pc & 0xff00) != (address & 0xff00)) { // page crossing
            cycles_remaining++;
            internal_work();
        }
        pc = address;
    }
}

void mos6502::push_stack(uint8_t data) {
    convenience = STACK_OFFSET + s;
    write(convenience, data);
    s--;
}

uint8_t mos6502::pull_stack() {
    s++;
    convenience = STACK_OFFSET + s;
    return read(convenience);
}

void mos6502::trigger_interrupt(uint16_t lo_vector, uint16_t hi_vector) {
    push_stack(static_cast<uint8_t>(pc >> 8));
    push_stack(static_cast<uint8_t>(pc));
    push_stack(status | B);
    uint8_t lo = read(lo_vector);
    uint8_t hi = read(hi_vector);
    pc = (hi << 8) | lo;
    setFlag(I, true);
}

uint16_t mos6502::execute_addressing(const Opcode& op){
    return (this->*op.address_mode)();
}
uint8_t mos6502::execute_instruction(const Opcode& op) {
    return (this->*op.instruction)();
}


void mos6502::clock_cycle() {
    // This should be once for each cpu clock cycle being performed. 

    // Loop: 
    if (cycles_remaining == 0) {
        // fetch byte at pc counter. 
        opcode_byte = read(pc); 
        pc++;
        // decode what address mode AND operation it is.
        cur_opcode = &opcode_lookup[opcode_byte];
        
        // determine how many cycles it will take.
        // FIXME: This does not currently account for the instruction that have variable cycle counts 
        // See : "oops" page-carrying at https://www.nesdev.org/wiki/CPU_addressing_modes
        // Also see : ex. BCS can take TWO extra cycles?? 
        cycles_remaining = cur_opcode->num_cycles;

        // address will increment the pc an appropriate amount 
        // then it will fetch the addressed value 
        // in 6502 cpu, the ALU only acts on 1 fetched at a time
        // this value will be store in fetched_value
        address_was_implied = false; // set this false every loop. Only changed to true by implied address modes (implicit & accumulator) 
        address = execute_addressing(*cur_opcode);
        
        // operate ON the value that is stored in fetched_value
        execute_instruction(*cur_opcode);
    }
    cycles_remaining--; 

};

void mos6502::reset() {
    // This should set all the registers to a known state. As well, it should set the pc to a hardcoded value, like the interrupts. 
    // Sets pc to $FFFC & FFFD
    // Interestingly, reset is basically just using the exact same logic as BRK. 
    // see: https://youtu.be/fWqBmmPQP40?t=2504
    // in reality it pushes the program counter and stack pointer to the stack. Thus we see it decrement our stack by 3
    // for consistency sake, i will decrement the stack accordingly, though i won't bother to push old stack pointers, since we are resetting anyways.
    cycles_remaining = 8; 
    trigger_interrupt(RST_VECTOR_LO, RST_VECTOR_HI);
};

void mos6502::masked_interrupt() {
    // A potential interrupt. This will only work if interrupts are enable. Store current values to the stack and then set the pc to a HARDCODED place in memory. 
    // This is called IRQ
    // Hijacks pc to $FFFE and $FFFF
    cycles_remaining = 7; 
    if (!getFlag(I)) {
        trigger_interrupt(IRQ_VECTOR_LO, IRQ_VECTOR_HI);
    }
};

void mos6502::non_masked_interrupt() {
    // This is non skippable interrupt. Store current values to the stack, and set the pc to a HARDCODED space in memory for handling the interrupt.
    // This is called NMI
    // Hijacks pc to $FFFA and FFFB
    cycles_remaining = 7;
    trigger_interrupt(NMI_VECTOR_LO, NMI_VECTOR_HI);
};

void mos6502::step() {
    clock_cycle();
    while (cycles_remaining != 0) {
        clock_cycle();
    }
}

/*
ADDRESS MODE FUNCTIONS HERE
*/
// accumulator address (an implied address, no extra byte to read for opcode)
uint16_t mos6502::Accumulator_addr() { 
    address_was_implied = true;
    return 0; 
}
// absolute address no index 
uint16_t mos6502::Absolute_addr() { 
    address_was_implied = false;
    // low byte of address
    address = static_cast<uint16_t>(read(pc));
    pc++; 
    // highbyte of address
    address |= static_cast<uint16_t>(read(pc)) << 8;
    pc++;
    return address; 
}
// absolute indexed with X
uint16_t mos6502::AbsoluteX_addr() { 
    address_was_implied = false;
    // low byte of address
    address = static_cast<uint16_t>(read(pc));
    pc++;
    // highbyte of address
    address |= static_cast<uint16_t>(read(pc)) << 8;
    pc++;
    address += x;
    // if page_crossed 
        // internal_work() or read()?
    return address;
}
// absolute indexed with Y 
uint16_t mos6502::AbsoluteY_addr() { 
    address_was_implied = false;
    // low byte of address
    convenience = static_cast<uint16_t>(read(pc));
    pc++;
    // highbyte of address
    convenience |= static_cast<uint16_t>(read(pc)) << 8;
    pc++;
    convenience += y;

    // if page_crossed 
        // internal_work() or read()?
    return convenience;
}
// immediate value 
uint16_t mos6502::Immediate_addr() { 
    address_was_implied = false;
    address = pc; 
    pc++; 
    return address;
}
// implicit address 
uint16_t mos6502::Implicit_addr() { 
    address_was_implied = true;
    return 0;
}

// indirect 16-bit used only for JMP.
uint16_t mos6502::Indirect_addr() {  
    address_was_implied = false;
    // low byte of address
    uint8_t lo = read(pc); pc++;
    uint8_t hi = read(pc); pc++;
    address = lo | (static_cast<uint16_t>(hi) << 8); 
    
    lo = read(address); 
    hi = read(address + 1); 
    // TODO: There's a hardware bug on original 6502s, where if lo == 0xXXff, then hi would be 0xXX00, not 0x(X+1)(X+1)00
    // Need to decide if I want to emulate that bug or not. 
    address = lo | (static_cast<uint16_t>(hi) << 8);
    return address;
}

// indexed indirect with X (this is DIFFERENT than indirect indexed)
uint16_t mos6502::IndirectX_addr() { 
    address_was_implied = false;

    uint16_t ptr = static_cast<uint16_t>(read(pc));
    ptr += x; // X-indexed indirection here
    ptr &= 0x00ff; // ensure we're still on the zero page 
    uint8_t lo = read(ptr); 
    ptr++;
    ptr &= 0x00ff; // ensure we're still on the zero page 
    uint8_t hi = read(ptr + 1);

    address = lo | (static_cast<uint16_t>(hi) << 8);

    internal_work(); // TODO: is this supposed to be here?
    return address; 
}

// indirect indexed with Y
uint16_t mos6502::IndirectY_addr() {
    address_was_implied = false;

    uint16_t ptr = static_cast<uint16_t>(read(pc));
    uint8_t lo = read(ptr);
    ptr++;
    ptr &= 0x00ff; // ensure we're still on the zero page 
    uint8_t hi = read(ptr + 1);

    uint16_t base_address = lo | (static_cast<uint16_t>(hi) << 8);
    address = base_address + y; // indirect indexing by y here
    bool page_crossed = (base_address & 0xff00) != (address & 0xff00);
    if (page_crossed && cur_opcode->extra_cycle_possible) {
        internal_work(); // already correctly stored the correct address, just dummy here
    }
        
    return address;
}

// Relative offset to the current pc. 
uint16_t mos6502::Relative_addr() { 
    address_was_implied = false;
    
    int8_t offset = static_cast<int8_t>(read(pc));
    pc++;
    address = pc + offset; 
    return address;
}

// zero page no index
uint16_t mos6502::ZeroPage_addr() {
    address_was_implied = false;

    address = read(pc);
    pc++;
    return address; 
}

// zero paged indexed with X
uint16_t mos6502::ZeroPageX_addr() {
    address_was_implied = false;

    address = read(pc) + x;
    pc++;
    // IDLE here?
    return address;
}
// zero paged indexed with Y
uint16_t mos6502::ZeroPageY_addr() {
    address_was_implied = false;

    address = read(pc) + y;
    pc++;
    // IDLE here?
    return address;
}


/*
Instruction Functions here. After the address mode fetches any data, then the 
instruction section will 
*/

// Add with Carry
// flags to check: C,Z,N,V
uint8_t mos6502::ADC() {
    value = fetch_value();

    convenience = value + a; 
    setFlag(C, (convenience & (1 << 8))); // check bit 8 for a carry 
    setFlag(Z, (convenience & 0x00ff) == 0); // check if bits 0-7 are all zero
    setFlag(N, (convenience & (1 << 7)));
    // check if there has been a change in sign from BOTH operands to the result
    setFlag(V, ((a ^ convenience | value ^ convenience) & (1 << 7)) == 0);
    a = static_cast<uint8_t>(convenience); // drop the highest 8 bits 
    return 0; 
}
// logical AND
// flags to check Z,N
uint8_t mos6502::AND() {
    value = fetch_value();
    a = value & a;
    setFlag(Z, (a & 0x00ff) == 0); // set if A==0
    setFlag(N, (a & (1 << 7))); // set if bit 7 is set
    return 0; 
}


uint8_t mos6502::ASL() { 
    if (cur_opcode->address_mode_enum == Opcode::AddrMode::AbsoluteX_addr) {
        internal_work();
    }

    value = fetch_value();

    setFlag(C, (value & (1 << 7))); // set carry to the old bit 7
    convenience = value << 1;
    setFlag(Z, (convenience & 0x00ff) == 0); // set if a is 0 after shift
    setFlag(N, (convenience & 0x80));
    if (address_was_implied) {
        a = static_cast<uint8_t>(convenience);
    }
    else {
        write(address, value); // Read-Modify-Write operations have a dummy write
        write(address, static_cast<uint8_t>(convenience));
    }
    return 0;
}

// Branching Instructions
uint8_t mos6502::BCC() { 
    branch(!getFlag(C));
    return 0; 
}
uint8_t mos6502::BCS() { 
    branch(getFlag(C));
    return 0;
}
uint8_t mos6502::BEQ() {
    branch(getFlag(Z));
    return 0;
}
// Bit Test. Z = A & M, N = M7, V = M6
uint8_t mos6502::BIT() { 
    value = fetch_value();

    setFlag(N, value & (1 << 7)); // set N if bit7 of memory is set 
    setFlag(V, value & (1 << 6)); // set V if bit6 of memory is set
    setFlag(Z, (value & a) == 0); // set Z if memory and a is 0.
    return 0;
}

uint8_t mos6502::BMI() {
    branch(getFlag(N));
    return 0;
}
uint8_t mos6502::BNE() {
    branch(!getFlag(Z));
    return 0;
}
uint8_t mos6502::BPL() {
    branch(!getFlag(N));
    return 0;
}
uint8_t mos6502::BRK() {
    internal_work();
    pc++; // BRK skips over its following byte with a dummy read
    push_stack(static_cast<uint8_t>(pc >> 8));
    push_stack(static_cast<uint8_t>(pc));
    push_stack(status | B);

    uint8_t lo = read(IRQ_VECTOR_LO);
    uint8_t hi = read(IRQ_VECTOR_HI);
    pc = (hi << 8) | lo; 
    setFlag(I, true);
    return 0; 
}

uint8_t mos6502::BVC() {
    branch(!getFlag(V));
    return 0;
}

uint8_t mos6502::BVS() {
    branch(getFlag(V));
    return 0;
}

// Clear Flag Functions. Easy to implement :-)
uint8_t mos6502::CLC() { 
    setFlag(C, false);
    internal_work();
    return 0;
}
uint8_t mos6502::CLD() { 
    setFlag(D, false); 
    internal_work();
    return 0; 
}
uint8_t mos6502::CLI() { 
    setFlag(I, false); 
    internal_work();
    return 0; 
}
uint8_t mos6502::CLV() { 
    setFlag(V, false); 
    internal_work();
    return 0; 
}

// Compare: Compares fetched value to accumulator / reg X / reg Y. 
// Sets flags as defined in Docs. 
uint8_t mos6502::CMP() { 
    value = fetch_value();

    setFlag(C, (a >= value)); // set C if a larger
    setFlag(Z, (a == value)); // set Z if a == value
    convenience = a-value;
    setFlag(N, convenience & (1 << 7)); // check if bit 7 of a-fetch is set
    return 0; 
}
uint8_t mos6502::CPX() {
    value = fetch_value();

    setFlag(C, (x >= value)); // set C if a larger
    setFlag(Z, (x == value)); // set Z if a == value
    convenience = x - value;
    setFlag(N, convenience & (1 << 7)); // check if bit 7 of a-fetch is set
    return 0;
}
uint8_t mos6502::CPY() {
    value = fetch_value();

    setFlag(C, (y >= value)); // set C if a larger
    setFlag(Z, (y == value)); // set Z if a == value
    convenience = y - value;
    setFlag(N, convenience & (1 << 7)); // check if bit 7 of a-fetch is set
    return 0;
}

// Decremennt Memory
uint8_t mos6502::DEC() { 
    if (cur_opcode->address_mode_enum == Opcode::AddrMode::AbsoluteX_addr) {
        internal_work();
    }
    value = fetch_value();

    convenience = value - 1;
    setFlag(Z, value == 0);
    setFlag(N, value & (1 << 7));
    if (address_was_implied) {
        a = value;
    }
    else {
        write(address, value); // Read-Modify-Write operations have a dummy write
        write(address, static_cast<uint8_t>(convenience));
    }
    return 0;
}
// Decrement X Register
uint8_t mos6502::DEX() {
    x -= 1;
    setFlag(Z, x == 0);
    setFlag(N, x & (1 << 7));
    internal_work();
    return 0;
}
// Decrement Y Register
uint8_t mos6502::DEY() {
    y -= 1;
    setFlag(Z, y == 0);
    setFlag(N, y & (1 << 7));
    internal_work();
    return 0;
}

// Exclusive OR
uint8_t mos6502::EOR() {
    value = fetch_value();

    a = value ^ a; 
    setFlag(Z, a == 0);
    setFlag(N, a & (1 << 7)); // check if bit 7 is set
    return 0;
}

// increment Memory
uint8_t mos6502::INC() {
    if (cur_opcode->address_mode_enum == Opcode::AddrMode::AbsoluteX_addr) {
        internal_work();
    }

    value = fetch_value();

    convenience = value - 1;
    setFlag(Z, value == 0);
    setFlag(N, value & (1 << 7));
    if (address_was_implied) {
        a = value;
    }
    else {
        write(address, value); // Read-Modify-Write operations have a dummy write
        write(address, static_cast<uint8_t>(convenience));
    }
    return 0;
}
// Increment x register
uint8_t mos6502::INX() {
    x += 1;
    setFlag(Z, x == 0);
    setFlag(N, x & (1 << 7));
    internal_work();
    return 0;
}
//increment y register
uint8_t mos6502::INY() {
    y += 1;
    setFlag(Z, y == 0);
    setFlag(N, y & (1 << 7));
    internal_work();
    return 0;
}
uint8_t mos6502::JMP() { 
    pc = address; 
    return 0; 
}
uint8_t mos6502::JSR() { 
    internal_work();
    pc--; // pc will be incremented again at the end of RTS. This is intended behaviour

    push_stack(static_cast<uint8_t>(pc >> 8)); // hi
    push_stack(static_cast<uint8_t>(pc)); // lo
    pc = address;
    return 0; 
}

// Load Register A from Memory
uint8_t mos6502::LDA() { 
    value = fetch_value();

    a = value;
    setFlag(Z, a == 0);
    setFlag(N, a & (1 << 7));
    return 0;
}
uint8_t mos6502::LDX() {
    value = fetch_value();

    x = value;
    setFlag(Z, x == 0);
    setFlag(N, x & (1 << 7));
    return 0;
}

uint8_t mos6502::LDY() {
    value = fetch_value();

    y = value;
    y = value;
    setFlag(Z, y == 0);
    setFlag(N, y & (1 << 7));
    return 0;
}

// Logical Shift Right 
uint8_t mos6502::LSR() {
    if (cur_opcode->address_mode_enum == Opcode::AddrMode::AbsoluteX_addr) {
        internal_work();
    }

    value = fetch_value();

    setFlag(C, (value & 0x01)); // set carry to the old bit 0
    convenience = value >> 1;
    setFlag(Z, (convenience & 0x00ff) == 0); // set if a is 0 after shift
    setFlag(N, (convenience & 0x80));
    if (address_was_implied) {
        a = static_cast<uint8_t>(convenience);
    }
    else {
        write(address, value); // Read-Modify-Write operations have a dummy write
        write(address, static_cast<uint8_t>(convenience));
    }
    return 0;
}
// No operation. Takes 2 cycles. 
uint8_t mos6502::NOP() { 
    internal_work();
    return 0;
}

// Bitwise OR on the Accumulator
uint8_t mos6502::ORA() { 
    value = fetch_value();

    a |= value; 
    setFlag(Z, a == 0);
    setFlag(N, a & (1 << 7));
    return 0;
}

uint8_t mos6502::PHA() { 
    internal_work();
    push_stack(a);
    return 0; 
}
uint8_t mos6502::PHP() { 
    internal_work();
    push_stack(status);
    return 0; 
}
uint8_t mos6502::PLA() { 
    internal_work();
    internal_work();
    a = pull_stack();
    return 0;
}
uint8_t mos6502::PLP() {
    internal_work();
    internal_work();
    status = pull_stack();
    return 0; 
}

// Rotate Left. Bit 0 is whatever the current carry flag is. New carry flag is the old Bit 7.
uint8_t mos6502::ROL() {
    if (cur_opcode->address_mode_enum == Opcode::AddrMode::AbsoluteX_addr) {
        internal_work();
    }

    value = fetch_value();

    convenience = value << 1; 
    convenience |= (getFlag(C) ? 0x01 : 0x00); // use the carrry flag as a new bit 0.
    setFlag(C, value & (1 << 7)); // set Carry flag to the old bit 7
    setFlag(Z, (convenience & 0x00FF) == 0);
    setFlag(N, (convenience & 0x00FF) & (1 << 7)); 
    if (address_was_implied) {
        a = static_cast<uint8_t>(convenience);
    }
    else {
        write(address, value); // Read-Modify-Write operations have a dummy write
        write(address, static_cast<uint8_t>(convenience));
    }
    return 0;
}
uint8_t mos6502::ROR() { 
    if (cur_opcode->address_mode_enum == Opcode::AddrMode::AbsoluteX_addr) {
        internal_work();
    }

    value = fetch_value();

    convenience = value >> 1; 
    convenience |= (getFlag(C) ? (1 << 7) : 0x00); // use carry flag as the new bit 7
    setFlag(C, value & 0x01); // set Carry flag to the old bit 0
    setFlag(Z, (convenience & 0x00FF) == 0);
    setFlag(N, (convenience & 0x00FF) & (1 << 7));
    if (address_was_implied) {
        a = static_cast<uint8_t>(convenience);
    }
    else {
        write(address, value); // Read-Modify-Write operations have a dummy write
        write(address, static_cast<uint8_t>(convenience));
    }
    return 0;
}
uint8_t mos6502::RTI() { 
    internal_work();
    internal_work();
    status = pull_stack();
    setFlag(B, false);
    uint8_t lo = pull_stack();
    uint8_t hi = pull_stack();
    pc = (hi << 8) | lo; 
    return 0; 
}
uint8_t mos6502::RTS() { 
    internal_work(); // t1
    internal_work(); // t2
    uint8_t lo = pull_stack(); // t3
    uint8_t hi = pull_stack(); // t4
    pc = (hi << 8) | lo;
    internal_work(); // t5
    pc++;
    return 0;
}
uint8_t mos6502::SBC() {
    value = fetch_value();

    convenience = a - value;
    convenience -= getFlag(C) ? 0 : 1;
    setFlag(C, (convenience & (1 << 8))); // check bit 8 for a carry 
    setFlag(Z, (convenience & 0x00ff) == 0); // check if bits 0-7 are all zero
    setFlag(N, (convenience & (1 << 7)));
    // check if there has been subtraction overflow/underflow
    setFlag(V, ((a ^ (convenience & 0x00FF)) & (a ^ value)) & (1 << 7));
    a = static_cast<uint8_t>(convenience); // drop the highest 8 bits 
    return 0;
}

// Set carry flag
uint8_t mos6502::SEC() { 
    setFlag(C, true);
    internal_work();
    return 0;
}
// Set Decimal Mode Flag
uint8_t mos6502::SED() { 
    setFlag(D, true);
    internal_work();
    return 0;
}
// Set Interrupt Disabled Flag
uint8_t mos6502::SEI() { 
    setFlag(I, true);
    internal_work();
    return 0;
}


uint8_t mos6502::STA() { 
    switch (cur_opcode->address_mode_enum) {
        case Opcode::AddrMode::AbsoluteX_addr:
        case Opcode::AddrMode::AbsoluteY_addr:
        case Opcode::AddrMode::IndirectY_addr:
            internal_work();
            break;
        default:
            break;
    }

    write(address, a);
    return 0;
}
uint8_t mos6502::STX() { 
    write(address, x);
    return 0;
}
uint8_t mos6502::STY() { 
    write(address, y);
    return 0;
}

uint8_t mos6502::TAX() { 
    x = a; 
    setFlag(Z, x == 0);
    setFlag(N, x & (1 << 7));
    internal_work();
    return 0;
}
uint8_t mos6502::TAY() {
    y = a;
    setFlag(Z, y == 0);
    setFlag(N, y & (1 << 7));
    internal_work();
    return 0;
}
uint8_t mos6502::TSX() {
    x = s;
    setFlag(Z, x == 0);
    setFlag(N, x &(1 << 7));
    internal_work();
    return 0;
}
uint8_t mos6502::TXA() {
    a = x;
    setFlag(Z, a == 0);
    setFlag(N, a &(1 << 7));
    internal_work();
    return 0;
}
uint8_t mos6502::TXS() {
    s = x;
    setFlag(Z, s == 0);
    setFlag(N, s &(1 << 7));
    internal_work();
    return 0;
}
uint8_t mos6502::TYA() {
    a = y;
    setFlag(Z, a == 0);
    setFlag(N, a &(1 << 7));
    internal_work();
    return 0;
}
