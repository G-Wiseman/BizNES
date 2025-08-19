#pragma once
#include <stdlib.h>
#include "opcode.h"
#include <stdio.h>
#include <vector>
#include <array>
#include <functional>
#include <stdint.h>
#include <string>


class Bus; // forward declaration that allows cpu to know that bus will exist

class mos6502
{
private:
    Bus* bus;
    // Registers
    uint8_t a = 0x00; // accumulator register
    uint8_t x = 0x00; // x register
    uint8_t y = 0x00; // y register
    uint8_t s = 0x00; // stack pointer register
    uint16_t pc = 0x0000; // program counter
    uint8_t status = 0x00; // status register (see Enum for bits)
    
    // internal variables 
    bool address_was_implied = false;

    Opcode* cur_opcode = nullptr; 
    uint8_t opcode_byte = 0x00;
    uint8_t cycles_remaining = 0;
    uint16_t convenience = 0x0000; // unsigned 16bit temporay variable solely for convenience
    uint16_t address = 0x0000; // internal variable to store addressing mode results in. 
    uint8_t value = 0x00; // Value returned after fetch  ing a byte from memory (use with ALU)

public:
    std::array<Opcode, 0xff> opcode_lookup;

    enum STATUS_FLAG_MASKS {
        // https://www.nesdev.org/wiki/Status_flags#The_B_flag
        C = 1 << 0, // Carry. See documentation for specifics. 
        Z = 1 << 1, // Zero. This is set or cleared based on if the last result was zero.
        I = 1 << 2, // Interrupt disabled.
        D = 1 << 3, // decimal. mode, which is disable on the NES variation of this CPU
        B = 1 << 4, // processing interrupts, just called the B flag for now
        U = 1 << 5, // Unused. 
        V = 1 << 6, // Overflow. See the docs for now. 
        N = 1 << 7, // Negative. after instructions that have a value result, this will contain bit 7
    };
    bool getFlag(STATUS_FLAG_MASKS flag);
    void setFlag(STATUS_FLAG_MASKS flag, bool value);
    uint8_t fetch_value();
    void opcode_table_gen();
    void branch(bool condition); // reuse most of the branching logic for each branch opcode
    void push_stack(uint8_t data);
    uint8_t pull_stack();

    uint16_t execute_addressing(const Opcode& op);
    uint8_t execute_instruction(const Opcode& op);


    mos6502(Bus* b);
    ~mos6502();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
    void internal_work(); // sometimes a full cycle never really accesses the bus, use this as a placeholder for timings
    void clock_cycle();
    void reset(); 
    void masked_interrupt();
    void non_masked_interrupt();
    void trigger_interrupt(uint16_t lo_vector, uint16_t hi_vector);

    // address modes
    uint16_t Accumulator_addr(); // accumulator address
    uint16_t Absolute_addr(); // absolute address no index 
    uint16_t AbsoluteX_addr(); // absolute indexed with X
    uint16_t AbsoluteY_addr(); // absolute indexed with Y 
    uint16_t Immediate_addr(); // immediate address 
    uint16_t Implicit_addr(); // implicit address 
    uint16_t Indirect_addr(); // indirect 16-bit used only for JMP.
    uint16_t IndirectX_addr(); // indirect indexed with X
    uint16_t IndirectY_addr(); // indirect indexed with Y
    uint16_t Relative_addr(); // Relative offset to the current pc. 
    uint16_t ZeroPage_addr(); // zero page no index
    uint16_t ZeroPageX_addr(); // zero paged indexed with X
    uint16_t ZeroPageY_addr(); // zero paged indexed with Y16
    uint16_t ILLEGAL_ADDRESS() {
        // default function to call for 
        return 0; 
    }

    // Instructions
    uint8_t ADC(); uint8_t AND(); uint8_t ASL();
    uint8_t BCC(); uint8_t BCS(); uint8_t BEQ();
    uint8_t BIT(); uint8_t BMI(); uint8_t BNE();
    uint8_t BPL(); uint8_t BRK(); uint8_t BVC();
    uint8_t BVS(); uint8_t CLC(); uint8_t CLD();
    uint8_t CLI(); uint8_t CLV(); uint8_t CMP();
    uint8_t CPX(); uint8_t CPY(); uint8_t DEC();
    uint8_t DEX(); uint8_t DEY(); uint8_t EOR();
    uint8_t INC(); uint8_t INX(); uint8_t INY();
    uint8_t JMP(); uint8_t JSR(); uint8_t LDA();
    uint8_t LDX(); uint8_t LDY(); uint8_t LSR();
    uint8_t NOP(); uint8_t ORA(); uint8_t PHA();
    uint8_t PHP(); uint8_t PLA(); uint8_t PLP();
    uint8_t ROL(); uint8_t ROR(); uint8_t RTI(); 
    uint8_t RTS(); uint8_t SBC(); uint8_t SEC();
    uint8_t SED(); uint8_t SEI(); uint8_t STA();
    uint8_t STX(); uint8_t STY(); uint8_t TAX();
    uint8_t TAY(); uint8_t TSX(); uint8_t TXA();
    uint8_t TXS(); uint8_t TYA(); 
};
 