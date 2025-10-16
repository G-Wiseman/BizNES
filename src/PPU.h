#pragma once
#include <cstdint>
#include "Cartridge.h"

class PPU
{
public:
	PPU();
	PPU(Cartridge* cart);
private:
	uint8_t ppu_ctrl; // register at $200
	uint8_t ppu_mask; // $2001
	uint8_t ppu_status; // $2002
	uint8_t oam_addr; // $2003
	uint8_t oam_data; // $2004
	uint8_t ppu_scroll; // 2005
	uint8_t ppu_addr; // 2006
	uint8_t ppu_data; // 2007
	uint8_t oam_dma; // $4014

	uint8_t latch_value;

private: // internal convenience variables
	Cartridge* cartridge;
	uint16_t addr;

private: // counting where the PPU is at in its cycle
	int16_t scanline = 0; 
	int16_t cycle = 0;
public: 
	// a list of every pixel to render on screen. It will update one of these on a cycle? 
	uint8_t frame_buffer[256 * 240]; 

public:
	uint8_t read(uint16_t addr14bit);
	void write(uint16_t addr14bit, uint8_t value);

	uint8_t cpuRead(uint16_t addr);
	void cpuWrite(uint16_t addr, uint8_t value);

public:
	void clock();
	void visible_scanline();
};