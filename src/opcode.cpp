#include "mos6502.h"
#include "opcode.h"

Opcode::Opcode() :
    name("ILG"),
    address_mode_enum(AddrMode::Absolute_addr),
    address_mode(&mos6502::Implicit_addr),
    instruction(&mos6502::NOP),
    num_cycles(2),
    extra_cycle_possible(false) {
}

Opcode::Opcode(
    char* name,
    AddrMode addr_enum,
    uint16_t(mos6502::* addr)(),
    uint8_t(mos6502::* inst)(),
    uint8_t num_cycles,
    bool ecp
) : name(name),
    address_mode_enum(addr_enum),
    address_mode(addr),
    instruction(inst),
    num_cycles(num_cycles),
    extra_cycle_possible(ecp) {
};