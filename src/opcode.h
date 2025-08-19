#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <array>
#include <functional>
#include <stdint.h>
#include <string>

class mos6502;
class Opcode {
public:
    enum class AddrMode {
        Absolute_addr,
        AbsoluteX_addr,
        Implicit_addr,
        Immediate_addr,
        Indirect_addr,
        IndirectX_addr,
        AbsoluteY_addr,
        Accumulator_addr,
        Relative_addr,
        ZeroPage_addr,
        IndirectY_addr,
        ZeroPageX_addr,
        ZeroPageY_addr
    };


    const char* name;
    AddrMode address_mode_enum;
    uint16_t(mos6502::* address_mode)();
    uint8_t(mos6502::* instruction)();
    uint8_t num_cycles;
    bool extra_cycle_possible;
    Opcode();
    Opcode(
        char* name,
        AddrMode addr_enum,
        uint16_t(mos6502::* addr)(),
        uint8_t(mos6502::* inst)(),
        uint8_t num_cycles,
        bool ecp = false
    );
};