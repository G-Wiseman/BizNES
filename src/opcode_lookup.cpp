#include "mos6502.h"
#include "Bus.h"
#include <array>
#include <cstdint>
#include <iostream>


const std::array<Opcode, 256> mos6502::opcode_lookup = [] {
    std::array<Opcode, 256> table{}; 
    using cpu = mos6502;
    table[0x0] = Opcode(
        "BRK",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::BRK,
        uint8_t(7),
        false
    );
    
    table[0x0A] = Opcode(
        "ASL",
        Opcode::AddrMode::Accumulator_addr, 
        &cpu::Accumulator_addr,
        &cpu::ASL,
        uint8_t(2),
        false
    );
    
    table[0x0D] = Opcode(
        "ORA",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::ORA,
        uint8_t(4),
        false
    );
    
    table[0x0E] = Opcode(
        "ASL",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::ASL,
        uint8_t(6),
        false
    );
    
    table[0x1] = Opcode(
        "ORA",
        Opcode::AddrMode::IndirectX_addr, 
        &cpu::IndirectX_addr,
        &cpu::ORA,
        uint8_t(6),
        false
    );
    
    table[0x10] = Opcode(
        "BPL",
        Opcode::AddrMode::Relative_addr, 
        &cpu::Relative_addr,
        &cpu::BPL,
        uint8_t(2),
        true
    );
    
    table[0x11] = Opcode(
        "ORA",
        Opcode::AddrMode::IndirectY_addr, 
        &cpu::IndirectY_addr,
        &cpu::ORA,
        uint8_t(5),
        true
    );
    
    table[0x15] = Opcode(
        "ORA",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::ORA,
        uint8_t(4),
        false
    );
    
    table[0x16] = Opcode(
        "ASL",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::ASL,
        uint8_t(6),
        false
    );
    
    table[0x18] = Opcode(
        "CLC",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::CLC,
        uint8_t(2),
        false
    );
    
    table[0x19] = Opcode(
        "ORA",
        Opcode::AddrMode::AbsoluteY_addr, 
        &cpu::AbsoluteY_addr,
        &cpu::ORA,
        uint8_t(4),
        true
    );
    
    table[0x1D] = Opcode(
        "ORA",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::ORA,
        uint8_t(4),
        true
    );
    
    table[0x1E] = Opcode(
        "ASL",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::ASL,
        uint8_t(7),
        false
    );
    
    table[0x20] = Opcode(
        "JSR",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::JSR,
        uint8_t(6),
        false
    );
    
    table[0x21] = Opcode(
        "AND",
        Opcode::AddrMode::IndirectX_addr, 
        &cpu::IndirectX_addr,
        &cpu::AND,
        uint8_t(6),
        false
    );
    
    table[0x24] = Opcode(
        "BIT",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::BIT,
        uint8_t(3),
        false
    );
    
    table[0x25] = Opcode(
        "AND",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::AND,
        uint8_t(3),
        false
    );
    
    table[0x26] = Opcode(
        "ROL",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::ROL,
        uint8_t(5),
        false
    );
    
    table[0x28] = Opcode(
        "PLP",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::PLP,
        uint8_t(4),
        false
    );
    
    table[0x29] = Opcode(
        "AND",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::AND,
        uint8_t(2),
        false
    );
    
    table[0x2A] = Opcode(
        "ROL",
        Opcode::AddrMode::Accumulator_addr, 
        &cpu::Accumulator_addr,
        &cpu::ROL,
        uint8_t(2),
        false
    );
    
    table[0x2C] = Opcode(
        "BIT",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::BIT,
        uint8_t(4),
        false
    );
    
    table[0x2D] = Opcode(
        "AND",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::AND,
        uint8_t(4),
        false
    );
    
    table[0x2E] = Opcode(
        "ROL",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::ROL,
        uint8_t(6),
        false
    );
    
    table[0x30] = Opcode(
        "BMI",
        Opcode::AddrMode::Relative_addr, 
        &cpu::Relative_addr,
        &cpu::BMI,
        uint8_t(2),
        true
    );
    
    table[0x31] = Opcode(
        "AND",
        Opcode::AddrMode::IndirectY_addr, 
        &cpu::IndirectY_addr,
        &cpu::AND,
        uint8_t(5),
        true
    );
    
    table[0x35] = Opcode(
        "AND",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::AND,
        uint8_t(4),
        false
    );
    
    table[0x36] = Opcode(
        "ROL",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::ROL,
        uint8_t(6),
        false
    );
    
    table[0x38] = Opcode(
        "SEC",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::SEC,
        uint8_t(2),
        false
    );
    
    table[0x39] = Opcode(
        "AND",
        Opcode::AddrMode::AbsoluteY_addr, 
        &cpu::AbsoluteY_addr,
        &cpu::AND,
        uint8_t(4),
        true
    );
    
    table[0x3D] = Opcode(
        "AND",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::AND,
        uint8_t(4),
        true
    );
    
    table[0x3E] = Opcode(
        "ROL",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::ROL,
        uint8_t(7),
        false
    );
    
    table[0x40] = Opcode(
        "RTI",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::RTI,
        uint8_t(6),
        false
    );
    
    table[0x41] = Opcode(
        "EOR",
        Opcode::AddrMode::IndirectX_addr, 
        &cpu::IndirectX_addr,
        &cpu::EOR,
        uint8_t(6),
        false
    );
    
    table[0x45] = Opcode(
        "EOR",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::EOR,
        uint8_t(3),
        false
    );
    
    table[0x46] = Opcode(
        "LSR",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::LSR,
        uint8_t(5),
        false
    );
    
    table[0x48] = Opcode(
        "PHA",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::PHA,
        uint8_t(3),
        false
    );
    
    table[0x49] = Opcode(
        "EOR",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::EOR,
        uint8_t(2),
        false
    );
    
    table[0x4A] = Opcode(
        "LSR",
        Opcode::AddrMode::Accumulator_addr, 
        &cpu::Accumulator_addr,
        &cpu::LSR,
        uint8_t(2),
        false
    );
    
    table[0x4C] = Opcode(
        "JMP",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::JMP,
        uint8_t(3),
        false
    );
    
    table[0x4D] = Opcode(
        "EOR",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::EOR,
        uint8_t(4),
        false
    );
    
    table[0x4E] = Opcode(
        "LSR",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::LSR,
        uint8_t(6),
        false
    );
    
    table[0x5] = Opcode(
        "ORA",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::ORA,
        uint8_t(3),
        false
    );
    
    table[0x50] = Opcode(
        "BVC",
        Opcode::AddrMode::Relative_addr, 
        &cpu::Relative_addr,
        &cpu::BVC,
        uint8_t(2),
        true
    );
    
    table[0x51] = Opcode(
        "EOR",
        Opcode::AddrMode::IndirectY_addr, 
        &cpu::IndirectY_addr,
        &cpu::EOR,
        uint8_t(5),
        true
    );
    
    table[0x55] = Opcode(
        "EOR",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::EOR,
        uint8_t(4),
        false
    );
    
    table[0x56] = Opcode(
        "LSR",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::LSR,
        uint8_t(6),
        false
    );
    
    table[0x58] = Opcode(
        "CLI",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::CLI,
        uint8_t(2),
        false
    );
    
    table[0x59] = Opcode(
        "EOR",
        Opcode::AddrMode::AbsoluteY_addr, 
        &cpu::AbsoluteY_addr,
        &cpu::EOR,
        uint8_t(4),
        true
    );
    
    table[0x5D] = Opcode(
        "EOR",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::EOR,
        uint8_t(4),
        true
    );
    
    table[0x5E] = Opcode(
        "LSR",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::LSR,
        uint8_t(7),
        false
    );
    
    table[0x6] = Opcode(
        "ASL",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::ASL,
        uint8_t(5),
        false
    );
    
    table[0x60] = Opcode(
        "RTS",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::RTS,
        uint8_t(6),
        false
    );
    
    table[0x61] = Opcode(
        "ADC",
        Opcode::AddrMode::IndirectX_addr, 
        &cpu::IndirectX_addr,
        &cpu::ADC,
        uint8_t(6),
        false
    );
    
    table[0x65] = Opcode(
        "ADC",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::ADC,
        uint8_t(3),
        false
    );
    
    table[0x66] = Opcode(
        "ROR",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::ROR,
        uint8_t(5),
        false
    );
    
    table[0x68] = Opcode(
        "PLA",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::PLA,
        uint8_t(4),
        false
    );
    
    table[0x69] = Opcode(
        "ADC",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::ADC,
        uint8_t(2),
        false
    );
    
    table[0x6A] = Opcode(
        "ROR",
        Opcode::AddrMode::Accumulator_addr, 
        &cpu::Accumulator_addr,
        &cpu::ROR,
        uint8_t(2),
        false
    );
    
    table[0x6C] = Opcode(
        "JMP",
        Opcode::AddrMode::Indirect_addr, 
        &cpu::Indirect_addr,
        &cpu::JMP,
        uint8_t(5),
        false
    );
    
    table[0x6D] = Opcode(
        "ADC",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::ADC,
        uint8_t(4),
        false
    );
    
    table[0x6E] = Opcode(
        "ROR",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::ROR,
        uint8_t(6),
        false
    );
    
    table[0x70] = Opcode(
        "BVS",
        Opcode::AddrMode::Relative_addr, 
        &cpu::Relative_addr,
        &cpu::BVS,
        uint8_t(2),
        true
    );
    
    table[0x71] = Opcode(
        "ADC",
        Opcode::AddrMode::IndirectY_addr, 
        &cpu::IndirectY_addr,
        &cpu::ADC,
        uint8_t(5),
        true
    );
    
    table[0x75] = Opcode(
        "ADC",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::ADC,
        uint8_t(4),
        false
    );
    
    table[0x76] = Opcode(
        "ROR",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::ROR,
        uint8_t(6),
        false
    );
    
    table[0x78] = Opcode(
        "SEI",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::SEI,
        uint8_t(2),
        false
    );
    
    table[0x79] = Opcode(
        "ADC",
        Opcode::AddrMode::AbsoluteY_addr, 
        &cpu::AbsoluteY_addr,
        &cpu::ADC,
        uint8_t(4),
        true
    );
    
    table[0x7D] = Opcode(
        "ADC",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::ADC,
        uint8_t(4),
        true
    );
    
    table[0x7E] = Opcode(
        "ROR",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::ROR,
        uint8_t(7),
        false
    );
    
    table[0x8] = Opcode(
        "PHP",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::PHP,
        uint8_t(3),
        false
    );
    
    table[0x81] = Opcode(
        "STA",
        Opcode::AddrMode::IndirectX_addr, 
        &cpu::IndirectX_addr,
        &cpu::STA,
        uint8_t(6),
        false
    );
    
    table[0x84] = Opcode(
        "STY",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::STY,
        uint8_t(3),
        false
    );
    
    table[0x85] = Opcode(
        "STA",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::STA,
        uint8_t(3),
        false
    );
    
    table[0x86] = Opcode(
        "STX",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::STX,
        uint8_t(3),
        false
    );
    
    table[0x88] = Opcode(
        "DEY",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::DEY,
        uint8_t(2),
        false
    );
    
    table[0x8A] = Opcode(
        "TXA",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::TXA,
        uint8_t(2),
        false
    );
    
    table[0x8C] = Opcode(
        "STY",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::STY,
        uint8_t(4),
        false
    );
    
    table[0x8D] = Opcode(
        "STA",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::STA,
        uint8_t(4),
        false
    );
    
    table[0x8E] = Opcode(
        "STX",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::STX,
        uint8_t(4),
        false
    );
    
    table[0x9] = Opcode(
        "ORA",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::ORA,
        uint8_t(2),
        false
    );
    
    table[0x90] = Opcode(
        "BCC",
        Opcode::AddrMode::Relative_addr, 
        &cpu::Relative_addr,
        &cpu::BCC,
        uint8_t(2),
        true
    );
    
    table[0x91] = Opcode(
        "STA",
        Opcode::AddrMode::IndirectY_addr, 
        &cpu::IndirectY_addr,
        &cpu::STA,
        uint8_t(6),
        false
    );
    
    table[0x94] = Opcode(
        "STY",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::STY,
        uint8_t(4),
        false
    );
    
    table[0x95] = Opcode(
        "STA",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::STA,
        uint8_t(4),
        false
    );
    
    table[0x96] = Opcode(
        "STX",
        Opcode::AddrMode::ZeroPageY_addr, 
        &cpu::ZeroPageY_addr,
        &cpu::STX,
        uint8_t(4),
        false
    );
    
    table[0x98] = Opcode(
        "TYA",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::TYA,
        uint8_t(2),
        false
    );
    
    table[0x99] = Opcode(
        "STA",
        Opcode::AddrMode::AbsoluteY_addr, 
        &cpu::AbsoluteY_addr,
        &cpu::STA,
        uint8_t(5),
        false
    );
    
    table[0x9A] = Opcode(
        "TXS",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::TXS,
        uint8_t(2),
        false
    );
    
    table[0x9D] = Opcode(
        "STA",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::STA,
        uint8_t(5),
        false
    );
    
    table[0xA0] = Opcode(
        "LDY",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::LDY,
        uint8_t(2),
        false
    );
    
    table[0xA1] = Opcode(
        "LDA",
        Opcode::AddrMode::IndirectX_addr, 
        &cpu::IndirectX_addr,
        &cpu::LDA,
        uint8_t(6),
        false
    );
    
    table[0xA2] = Opcode(
        "LDX",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::LDX,
        uint8_t(2),
        false
    );
    
    table[0xA4] = Opcode(
        "LDY",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::LDY,
        uint8_t(3),
        false
    );
    
    table[0xA5] = Opcode(
        "LDA",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::LDA,
        uint8_t(3),
        false
    );
    
    table[0xA6] = Opcode(
        "LDX",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::LDX,
        uint8_t(3),
        false
    );
    
    table[0xA8] = Opcode(
        "TAY",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::TAY,
        uint8_t(2),
        false
    );
    
    table[0xA9] = Opcode(
        "LDA",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::LDA,
        uint8_t(2),
        false
    );
    
    table[0xAA] = Opcode(
        "TAX",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::TAX,
        uint8_t(2),
        false
    );
    
    table[0xAC] = Opcode(
        "LDY",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::LDY,
        uint8_t(4),
        false
    );
    
    table[0xAD] = Opcode(
        "LDA",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::LDA,
        uint8_t(4),
        false
    );
    
    table[0xAE] = Opcode(
        "LDX",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::LDX,
        uint8_t(4),
        false
    );
    
    table[0xB0] = Opcode(
        "BCS",
        Opcode::AddrMode::Relative_addr, 
        &cpu::Relative_addr,
        &cpu::BCS,
        uint8_t(2),
        true
    );
    
    table[0xB1] = Opcode(
        "LDA",
        Opcode::AddrMode::IndirectY_addr, 
        &cpu::IndirectY_addr,
        &cpu::LDA,
        uint8_t(5),
        true
    );
    
    table[0xB4] = Opcode(
        "LDY",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::LDY,
        uint8_t(4),
        false
    );
    
    table[0xB5] = Opcode(
        "LDA",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::LDA,
        uint8_t(4),
        false
    );
    
    table[0xB6] = Opcode(
        "LDX",
        Opcode::AddrMode::ZeroPageY_addr, 
        &cpu::ZeroPageY_addr,
        &cpu::LDX,
        uint8_t(4),
        false
    );
    
    table[0xB8] = Opcode(
        "CLV",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::CLV,
        uint8_t(2),
        false
    );
    
    table[0xB9] = Opcode(
        "LDA",
        Opcode::AddrMode::AbsoluteY_addr, 
        &cpu::AbsoluteY_addr,
        &cpu::LDA,
        uint8_t(4),
        true
    );
    
    table[0xBA] = Opcode(
        "TSX",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::TSX,
        uint8_t(2),
        false
    );
    
    table[0xBC] = Opcode(
        "LDY",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::LDY,
        uint8_t(4),
        true
    );
    
    table[0xBD] = Opcode(
        "LDA",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::LDA,
        uint8_t(4),
        true
    );
    
    table[0xBE] = Opcode(
        "LDX",
        Opcode::AddrMode::AbsoluteY_addr, 
        &cpu::AbsoluteY_addr,
        &cpu::LDX,
        uint8_t(4),
        true
    );
    
    table[0xC0] = Opcode(
        "CPY",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::CPY,
        uint8_t(2),
        false
    );
    
    table[0xC1] = Opcode(
        "CMP",
        Opcode::AddrMode::IndirectX_addr, 
        &cpu::IndirectX_addr,
        &cpu::CMP,
        uint8_t(6),
        false
    );
    
    table[0xC4] = Opcode(
        "CPY",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::CPY,
        uint8_t(3),
        false
    );
    
    table[0xC5] = Opcode(
        "CMP",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::CMP,
        uint8_t(3),
        false
    );
    
    table[0xC6] = Opcode(
        "DEC",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::DEC,
        uint8_t(5),
        false
    );
    
    table[0xC8] = Opcode(
        "INY",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::INY,
        uint8_t(2),
        false
    );
    
    table[0xC9] = Opcode(
        "CMP",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::CMP,
        uint8_t(2),
        false
    );
    
    table[0xCA] = Opcode(
        "DEX",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::DEX,
        uint8_t(2),
        false
    );
    
    table[0xCC] = Opcode(
        "CPY",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::CPY,
        uint8_t(4),
        false
    );
    
    table[0xCD] = Opcode(
        "CMP",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::CMP,
        uint8_t(4),
        false
    );
    
    table[0xCE] = Opcode(
        "DEC",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::DEC,
        uint8_t(6),
        false
    );
    
    table[0xD0] = Opcode(
        "BNE",
        Opcode::AddrMode::Relative_addr, 
        &cpu::Relative_addr,
        &cpu::BNE,
        uint8_t(2),
        true
    );
    
    table[0xD1] = Opcode(
        "CMP",
        Opcode::AddrMode::IndirectY_addr, 
        &cpu::IndirectY_addr,
        &cpu::CMP,
        uint8_t(5),
        true
    );
    
    table[0xD5] = Opcode(
        "CMP",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::CMP,
        uint8_t(4),
        false
    );
    
    table[0xD6] = Opcode(
        "DEC",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::DEC,
        uint8_t(6),
        false
    );
    
    table[0xD8] = Opcode(
        "CLD",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::CLD,
        uint8_t(2),
        false
    );
    
    table[0xD9] = Opcode(
        "CMP",
        Opcode::AddrMode::AbsoluteY_addr, 
        &cpu::AbsoluteY_addr,
        &cpu::CMP,
        uint8_t(4),
        true
    );
    
    table[0xDD] = Opcode(
        "CMP",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::CMP,
        uint8_t(4),
        true
    );
    
    table[0xDE] = Opcode(
        "DEC",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::DEC,
        uint8_t(7),
        false
    );
    
    table[0xE0] = Opcode(
        "CPX",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::CPX,
        uint8_t(2),
        false
    );
    
    table[0xE1] = Opcode(
        "SBC",
        Opcode::AddrMode::IndirectX_addr, 
        &cpu::IndirectX_addr,
        &cpu::SBC,
        uint8_t(6),
        false
    );
    
    table[0xE4] = Opcode(
        "CPX",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::CPX,
        uint8_t(3),
        false
    );
    
    table[0xE5] = Opcode(
        "SBC",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::SBC,
        uint8_t(3),
        false
    );
    
    table[0xE6] = Opcode(
        "INC",
        Opcode::AddrMode::ZeroPage_addr, 
        &cpu::ZeroPage_addr,
        &cpu::INC,
        uint8_t(5),
        false
    );
    
    table[0xE8] = Opcode(
        "INX",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::INX,
        uint8_t(2),
        false
    );
    
    table[0xE9] = Opcode(
        "SBC",
        Opcode::AddrMode::Immediate_addr, 
        &cpu::Immediate_addr,
        &cpu::SBC,
        uint8_t(2),
        false
    );
    
    table[0xEA] = Opcode(
        "NOP",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::NOP,
        uint8_t(2),
        false
    );
    
    table[0xEC] = Opcode(
        "CPX",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::CPX,
        uint8_t(4),
        false
    );
    
    table[0xED] = Opcode(
        "SBC",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::SBC,
        uint8_t(4),
        false
    );
    
    table[0xEE] = Opcode(
        "INC",
        Opcode::AddrMode::Absolute_addr, 
        &cpu::Absolute_addr,
        &cpu::INC,
        uint8_t(6),
        false
    );
    
    table[0xF0] = Opcode(
        "BEQ",
        Opcode::AddrMode::Relative_addr, 
        &cpu::Relative_addr,
        &cpu::BEQ,
        uint8_t(2),
        true
    );
    
    table[0xF1] = Opcode(
        "SBC",
        Opcode::AddrMode::IndirectY_addr, 
        &cpu::IndirectY_addr,
        &cpu::SBC,
        uint8_t(5),
        true
    );
    
    table[0xF5] = Opcode(
        "SBC",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::SBC,
        uint8_t(4),
        false
    );
    
    table[0xF6] = Opcode(
        "INC",
        Opcode::AddrMode::ZeroPageX_addr, 
        &cpu::ZeroPageX_addr,
        &cpu::INC,
        uint8_t(6),
        false
    );
    
    table[0xF8] = Opcode(
        "SED",
        Opcode::AddrMode::Implicit_addr, 
        &cpu::Implicit_addr,
        &cpu::SED,
        uint8_t(2),
        false
    );
    
    table[0xF9] = Opcode(
        "SBC",
        Opcode::AddrMode::AbsoluteY_addr, 
        &cpu::AbsoluteY_addr,
        &cpu::SBC,
        uint8_t(4),
        true
    );
    
    table[0xFD] = Opcode(
        "SBC",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::SBC,
        uint8_t(4),
        true
    );
    
    table[0xFE] = Opcode(
        "INC",
        Opcode::AddrMode::AbsoluteX_addr, 
        &cpu::AbsoluteX_addr,
        &cpu::INC,
        uint8_t(7),
        false
    );
    
    return table;
}();