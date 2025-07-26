#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>

class mos6502; //forward declaration that allows Bus to know cpu will exist

class Bus
{
private:
    /* data */
    uint8_t ram[0x0800] = { 0x00 };

public:
    Bus(/* args */);
    ~Bus();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};