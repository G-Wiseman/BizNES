#include "mos6502.h"
#include "Bus.h"
#include <array>
#include <cstdint>
#include <iostream>

void mos6502::opcode_table_gen() {
	using cpu = mos6502;
    
    opcode_lookup[0x0] = Opcode(
        "BRK",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::BRK, this),
        uint8_t(7),
        false
    );
    
    opcode_lookup[0x0A] = Opcode(
        "ASL",
        Opcode::AddrMode::Accumulator_addr, 
        std::bind(&cpu::Accumulator_addr, this),
        std::bind(&cpu::ASL, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x0D] = Opcode(
        "ORA",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::ORA, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x0E] = Opcode(
        "ASL",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::ASL, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x1] = Opcode(
        "ORA",
        Opcode::AddrMode::IndirectX_addr, 
        std::bind(&cpu::IndirectX_addr, this),
        std::bind(&cpu::ORA, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x10] = Opcode(
        "BPL",
        Opcode::AddrMode::Relative_addr, 
        std::bind(&cpu::Relative_addr, this),
        std::bind(&cpu::BPL, this),
        uint8_t(2),
        true
    );
    
    opcode_lookup[0x11] = Opcode(
        "ORA",
        Opcode::AddrMode::IndirectY_addr, 
        std::bind(&cpu::IndirectY_addr, this),
        std::bind(&cpu::ORA, this),
        uint8_t(5),
        true
    );
    
    opcode_lookup[0x15] = Opcode(
        "ORA",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::ORA, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x16] = Opcode(
        "ASL",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::ASL, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x18] = Opcode(
        "CLC",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::CLC, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x19] = Opcode(
        "ORA",
        Opcode::AddrMode::AbsoluteY_addr, 
        std::bind(&cpu::AbsoluteY_addr, this),
        std::bind(&cpu::ORA, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0x1D] = Opcode(
        "ORA",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::ORA, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0x1E] = Opcode(
        "ASL",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::ASL, this),
        uint8_t(7),
        false
    );
    
    opcode_lookup[0x20] = Opcode(
        "JSR",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::JSR, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x21] = Opcode(
        "AND",
        Opcode::AddrMode::IndirectX_addr, 
        std::bind(&cpu::IndirectX_addr, this),
        std::bind(&cpu::AND, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x24] = Opcode(
        "BIT",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::BIT, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x25] = Opcode(
        "AND",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::AND, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x26] = Opcode(
        "ROL",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::ROL, this),
        uint8_t(5),
        false
    );
    
    opcode_lookup[0x28] = Opcode(
        "PLP",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::PLP, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x29] = Opcode(
        "AND",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::AND, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x2A] = Opcode(
        "ROL",
        Opcode::AddrMode::Accumulator_addr, 
        std::bind(&cpu::Accumulator_addr, this),
        std::bind(&cpu::ROL, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x2C] = Opcode(
        "BIT",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::BIT, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x2D] = Opcode(
        "AND",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::AND, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x2E] = Opcode(
        "ROL",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::ROL, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x30] = Opcode(
        "BMI",
        Opcode::AddrMode::Relative_addr, 
        std::bind(&cpu::Relative_addr, this),
        std::bind(&cpu::BMI, this),
        uint8_t(2),
        true
    );
    
    opcode_lookup[0x31] = Opcode(
        "AND",
        Opcode::AddrMode::IndirectY_addr, 
        std::bind(&cpu::IndirectY_addr, this),
        std::bind(&cpu::AND, this),
        uint8_t(5),
        true
    );
    
    opcode_lookup[0x35] = Opcode(
        "AND",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::AND, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x36] = Opcode(
        "ROL",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::ROL, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x38] = Opcode(
        "SEC",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::SEC, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x39] = Opcode(
        "AND",
        Opcode::AddrMode::AbsoluteY_addr, 
        std::bind(&cpu::AbsoluteY_addr, this),
        std::bind(&cpu::AND, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0x3D] = Opcode(
        "AND",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::AND, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0x3E] = Opcode(
        "ROL",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::ROL, this),
        uint8_t(7),
        false
    );
    
    opcode_lookup[0x40] = Opcode(
        "RTI",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::RTI, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x41] = Opcode(
        "EOR",
        Opcode::AddrMode::IndirectX_addr, 
        std::bind(&cpu::IndirectX_addr, this),
        std::bind(&cpu::EOR, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x45] = Opcode(
        "EOR",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::EOR, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x46] = Opcode(
        "LSR",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::LSR, this),
        uint8_t(5),
        false
    );
    
    opcode_lookup[0x48] = Opcode(
        "PHA",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::PHA, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x49] = Opcode(
        "EOR",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::EOR, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x4A] = Opcode(
        "LSR",
        Opcode::AddrMode::Accumulator_addr, 
        std::bind(&cpu::Accumulator_addr, this),
        std::bind(&cpu::LSR, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x4C] = Opcode(
        "JMP",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::JMP, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x4D] = Opcode(
        "EOR",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::EOR, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x4E] = Opcode(
        "LSR",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::LSR, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x5] = Opcode(
        "ORA",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::ORA, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x50] = Opcode(
        "BVC",
        Opcode::AddrMode::Relative_addr, 
        std::bind(&cpu::Relative_addr, this),
        std::bind(&cpu::BVC, this),
        uint8_t(2),
        true
    );
    
    opcode_lookup[0x51] = Opcode(
        "EOR",
        Opcode::AddrMode::IndirectY_addr, 
        std::bind(&cpu::IndirectY_addr, this),
        std::bind(&cpu::EOR, this),
        uint8_t(5),
        true
    );
    
    opcode_lookup[0x55] = Opcode(
        "EOR",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::EOR, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x56] = Opcode(
        "LSR",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::LSR, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x58] = Opcode(
        "CLI",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::CLI, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x59] = Opcode(
        "EOR",
        Opcode::AddrMode::AbsoluteY_addr, 
        std::bind(&cpu::AbsoluteY_addr, this),
        std::bind(&cpu::EOR, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0x5D] = Opcode(
        "EOR",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::EOR, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0x5E] = Opcode(
        "LSR",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::LSR, this),
        uint8_t(7),
        false
    );
    
    opcode_lookup[0x6] = Opcode(
        "ASL",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::ASL, this),
        uint8_t(5),
        false
    );
    
    opcode_lookup[0x60] = Opcode(
        "RTS",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::RTS, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x61] = Opcode(
        "ADC",
        Opcode::AddrMode::IndirectX_addr, 
        std::bind(&cpu::IndirectX_addr, this),
        std::bind(&cpu::ADC, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x65] = Opcode(
        "ADC",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::ADC, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x66] = Opcode(
        "ROR",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::ROR, this),
        uint8_t(5),
        false
    );
    
    opcode_lookup[0x68] = Opcode(
        "PLA",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::PLA, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x69] = Opcode(
        "ADC",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::ADC, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x6A] = Opcode(
        "ROR",
        Opcode::AddrMode::Accumulator_addr, 
        std::bind(&cpu::Accumulator_addr, this),
        std::bind(&cpu::ROR, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x6C] = Opcode(
        "JMP",
        Opcode::AddrMode::Indirect_addr, 
        std::bind(&cpu::Indirect_addr, this),
        std::bind(&cpu::JMP, this),
        uint8_t(5),
        false
    );
    
    opcode_lookup[0x6D] = Opcode(
        "ADC",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::ADC, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x6E] = Opcode(
        "ROR",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::ROR, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x70] = Opcode(
        "BVS",
        Opcode::AddrMode::Relative_addr, 
        std::bind(&cpu::Relative_addr, this),
        std::bind(&cpu::BVS, this),
        uint8_t(2),
        true
    );
    
    opcode_lookup[0x71] = Opcode(
        "ADC",
        Opcode::AddrMode::IndirectY_addr, 
        std::bind(&cpu::IndirectY_addr, this),
        std::bind(&cpu::ADC, this),
        uint8_t(5),
        true
    );
    
    opcode_lookup[0x75] = Opcode(
        "ADC",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::ADC, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x76] = Opcode(
        "ROR",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::ROR, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x78] = Opcode(
        "SEI",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::SEI, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x79] = Opcode(
        "ADC",
        Opcode::AddrMode::AbsoluteY_addr, 
        std::bind(&cpu::AbsoluteY_addr, this),
        std::bind(&cpu::ADC, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0x7D] = Opcode(
        "ADC",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::ADC, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0x7E] = Opcode(
        "ROR",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::ROR, this),
        uint8_t(7),
        false
    );
    
    opcode_lookup[0x8] = Opcode(
        "PHP",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::PHP, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x81] = Opcode(
        "STA",
        Opcode::AddrMode::IndirectX_addr, 
        std::bind(&cpu::IndirectX_addr, this),
        std::bind(&cpu::STA, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x84] = Opcode(
        "STY",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::STY, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x85] = Opcode(
        "STA",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::STA, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x86] = Opcode(
        "STX",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::STX, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0x88] = Opcode(
        "DEY",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::DEY, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x8A] = Opcode(
        "TXA",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::TXA, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x8C] = Opcode(
        "STY",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::STY, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x8D] = Opcode(
        "STA",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::STA, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x8E] = Opcode(
        "STX",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::STX, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x9] = Opcode(
        "ORA",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::ORA, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x90] = Opcode(
        "BCC",
        Opcode::AddrMode::Relative_addr, 
        std::bind(&cpu::Relative_addr, this),
        std::bind(&cpu::BCC, this),
        uint8_t(2),
        true
    );
    
    opcode_lookup[0x91] = Opcode(
        "STA",
        Opcode::AddrMode::IndirectY_addr, 
        std::bind(&cpu::IndirectY_addr, this),
        std::bind(&cpu::STA, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0x94] = Opcode(
        "STY",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::STY, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x95] = Opcode(
        "STA",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::STA, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x96] = Opcode(
        "STX",
        Opcode::AddrMode::ZeroPageY_addr, 
        std::bind(&cpu::ZeroPageY_addr, this),
        std::bind(&cpu::STX, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0x98] = Opcode(
        "TYA",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::TYA, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x99] = Opcode(
        "STA",
        Opcode::AddrMode::AbsoluteY_addr, 
        std::bind(&cpu::AbsoluteY_addr, this),
        std::bind(&cpu::STA, this),
        uint8_t(5),
        false
    );
    
    opcode_lookup[0x9A] = Opcode(
        "TXS",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::TXS, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0x9D] = Opcode(
        "STA",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::STA, this),
        uint8_t(5),
        false
    );
    
    opcode_lookup[0xA0] = Opcode(
        "LDY",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::LDY, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xA1] = Opcode(
        "LDA",
        Opcode::AddrMode::IndirectX_addr, 
        std::bind(&cpu::IndirectX_addr, this),
        std::bind(&cpu::LDA, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0xA2] = Opcode(
        "LDX",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::LDX, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xA4] = Opcode(
        "LDY",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::LDY, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0xA5] = Opcode(
        "LDA",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::LDA, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0xA6] = Opcode(
        "LDX",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::LDX, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0xA8] = Opcode(
        "TAY",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::TAY, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xA9] = Opcode(
        "LDA",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::LDA, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xAA] = Opcode(
        "TAX",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::TAX, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xAC] = Opcode(
        "LDY",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::LDY, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xAD] = Opcode(
        "LDA",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::LDA, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xAE] = Opcode(
        "LDX",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::LDX, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xB0] = Opcode(
        "BCS",
        Opcode::AddrMode::Relative_addr, 
        std::bind(&cpu::Relative_addr, this),
        std::bind(&cpu::BCS, this),
        uint8_t(2),
        true
    );
    
    opcode_lookup[0xB1] = Opcode(
        "LDA",
        Opcode::AddrMode::IndirectY_addr, 
        std::bind(&cpu::IndirectY_addr, this),
        std::bind(&cpu::LDA, this),
        uint8_t(5),
        true
    );
    
    opcode_lookup[0xB4] = Opcode(
        "LDY",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::LDY, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xB5] = Opcode(
        "LDA",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::LDA, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xB6] = Opcode(
        "LDX",
        Opcode::AddrMode::ZeroPageY_addr, 
        std::bind(&cpu::ZeroPageY_addr, this),
        std::bind(&cpu::LDX, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xB8] = Opcode(
        "CLV",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::CLV, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xB9] = Opcode(
        "LDA",
        Opcode::AddrMode::AbsoluteY_addr, 
        std::bind(&cpu::AbsoluteY_addr, this),
        std::bind(&cpu::LDA, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0xBA] = Opcode(
        "TSX",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::TSX, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xBC] = Opcode(
        "LDY",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::LDY, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0xBD] = Opcode(
        "LDA",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::LDA, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0xBE] = Opcode(
        "LDX",
        Opcode::AddrMode::AbsoluteY_addr, 
        std::bind(&cpu::AbsoluteY_addr, this),
        std::bind(&cpu::LDX, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0xC0] = Opcode(
        "CPY",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::CPY, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xC1] = Opcode(
        "CMP",
        Opcode::AddrMode::IndirectX_addr, 
        std::bind(&cpu::IndirectX_addr, this),
        std::bind(&cpu::CMP, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0xC4] = Opcode(
        "CPY",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::CPY, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0xC5] = Opcode(
        "CMP",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::CMP, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0xC6] = Opcode(
        "DEC",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::DEC, this),
        uint8_t(5),
        false
    );
    
    opcode_lookup[0xC8] = Opcode(
        "INY",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::INY, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xC9] = Opcode(
        "CMP",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::CMP, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xCA] = Opcode(
        "DEX",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::DEX, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xCC] = Opcode(
        "CPY",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::CPY, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xCD] = Opcode(
        "CMP",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::CMP, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xCE] = Opcode(
        "DEC",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::DEC, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0xD0] = Opcode(
        "BNE",
        Opcode::AddrMode::Relative_addr, 
        std::bind(&cpu::Relative_addr, this),
        std::bind(&cpu::BNE, this),
        uint8_t(2),
        true
    );
    
    opcode_lookup[0xD1] = Opcode(
        "CMP",
        Opcode::AddrMode::IndirectY_addr, 
        std::bind(&cpu::IndirectY_addr, this),
        std::bind(&cpu::CMP, this),
        uint8_t(5),
        true
    );
    
    opcode_lookup[0xD5] = Opcode(
        "CMP",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::CMP, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xD6] = Opcode(
        "DEC",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::DEC, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0xD8] = Opcode(
        "CLD",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::CLD, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xD9] = Opcode(
        "CMP",
        Opcode::AddrMode::AbsoluteY_addr, 
        std::bind(&cpu::AbsoluteY_addr, this),
        std::bind(&cpu::CMP, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0xDD] = Opcode(
        "CMP",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::CMP, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0xDE] = Opcode(
        "DEC",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::DEC, this),
        uint8_t(7),
        false
    );
    
    opcode_lookup[0xE0] = Opcode(
        "CPX",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::CPX, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xE1] = Opcode(
        "SBC",
        Opcode::AddrMode::IndirectX_addr, 
        std::bind(&cpu::IndirectX_addr, this),
        std::bind(&cpu::SBC, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0xE4] = Opcode(
        "CPX",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::CPX, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0xE5] = Opcode(
        "SBC",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::SBC, this),
        uint8_t(3),
        false
    );
    
    opcode_lookup[0xE6] = Opcode(
        "INC",
        Opcode::AddrMode::ZeroPage_addr, 
        std::bind(&cpu::ZeroPage_addr, this),
        std::bind(&cpu::INC, this),
        uint8_t(5),
        false
    );
    
    opcode_lookup[0xE8] = Opcode(
        "INX",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::INX, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xE9] = Opcode(
        "SBC",
        Opcode::AddrMode::Immediate_addr, 
        std::bind(&cpu::Immediate_addr, this),
        std::bind(&cpu::SBC, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xEA] = Opcode(
        "NOP",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::NOP, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xEC] = Opcode(
        "CPX",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::CPX, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xED] = Opcode(
        "SBC",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::SBC, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xEE] = Opcode(
        "INC",
        Opcode::AddrMode::Absolute_addr, 
        std::bind(&cpu::Absolute_addr, this),
        std::bind(&cpu::INC, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0xF0] = Opcode(
        "BEQ",
        Opcode::AddrMode::Relative_addr, 
        std::bind(&cpu::Relative_addr, this),
        std::bind(&cpu::BEQ, this),
        uint8_t(2),
        true
    );
    
    opcode_lookup[0xF1] = Opcode(
        "SBC",
        Opcode::AddrMode::IndirectY_addr, 
        std::bind(&cpu::IndirectY_addr, this),
        std::bind(&cpu::SBC, this),
        uint8_t(5),
        true
    );
    
    opcode_lookup[0xF5] = Opcode(
        "SBC",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::SBC, this),
        uint8_t(4),
        false
    );
    
    opcode_lookup[0xF6] = Opcode(
        "INC",
        Opcode::AddrMode::ZeroPageX_addr, 
        std::bind(&cpu::ZeroPageX_addr, this),
        std::bind(&cpu::INC, this),
        uint8_t(6),
        false
    );
    
    opcode_lookup[0xF8] = Opcode(
        "SED",
        Opcode::AddrMode::Implicit_addr, 
        std::bind(&cpu::Implicit_addr, this),
        std::bind(&cpu::SED, this),
        uint8_t(2),
        false
    );
    
    opcode_lookup[0xF9] = Opcode(
        "SBC",
        Opcode::AddrMode::AbsoluteY_addr, 
        std::bind(&cpu::AbsoluteY_addr, this),
        std::bind(&cpu::SBC, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0xFD] = Opcode(
        "SBC",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::SBC, this),
        uint8_t(4),
        true
    );
    
    opcode_lookup[0xFE] = Opcode(
        "INC",
        Opcode::AddrMode::AbsoluteX_addr, 
        std::bind(&cpu::AbsoluteX_addr, this),
        std::bind(&cpu::INC, this),
        uint8_t(7),
        false
    );
    
}