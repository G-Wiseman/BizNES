#pragma once
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <functional>

class Bus; // forward declaration that allows cpu to know that bus will exist

//class Opcode
//{
//public:
//    string name; 
//    function<uint16_t()> address_mode;
//    function<uint8_t()> instruction; 
//    uint8_t num_cycles;
//    Opcode(
//        string name,
//        function<uint16_t()> address_mode,
//        function<uint8_t()> instruction,
//        uint8_t num_cycles
//    ) : name(name),
//        address_mode(address_mode), 
//        instruction(instruction), 
//        num_cycles(num_cycles) {};
//    ~Opcode();
//
//private:
//
//};

class mos6502
{
private:
    Bus* bus;


    // Registers
    uint8_t a = 0x00;
    uint8_t x = 0x00;
    uint8_t y = 0x00;
    uint8_t s = 0x00;
    uint16_t pc = 0x0000;
    uint8_t status = 0x00;
    // internal variables 
    uint8_t cycles_remaining = 0;
    uint16_t fetch_address = 0x0000; // internal variable to store addressing mode results in. 
    uint8_t fetched = 0x00l; // internal variable to store value reads in


    // decode instructions
    //std::vector<Opcode> opcode_lookup; 
public:
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
    mos6502(Bus* b);
    ~mos6502();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
    void clock_cycle();
    void reset(); 
    void masked_interrupt();
    void non_masked_interrupt();
    void return_from_interrupt();

    // address modes
    //uint16_t zpx(); // zero paged indexed with X
    //uint16_t zpy(); // zero paged indexed with Y
    //uint16_t abx(); // absolute indexed with X
    //uint16_t aby(); // absolute indexed with Y 
    //uint16_t inx(); // indirect indexed with X
    //uint16_t iny(); // indirect indexed with Y
    //uint16_t imp(); // implicit address 
    //uint16_t imm(); // immediate address 
    //uint16_t zp0(); // zero page no index
    //uint16_t ab0(); // absolute address no index 
    //uint16_t in0(); // indirect 16-bit used only for JMP. 


    // Instructions
    uint8_t ADC(); 
    /*uint8_t AND(); uint8_t ASL();
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
    uint8_t TXS(); uint8_t TYA(); */
};
 