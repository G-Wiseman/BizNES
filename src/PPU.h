#pragma once
#include <cstdint>
#include "Cartridge.h"

class PPU
{
public:
	PPU();
	PPU(Cartridge* cart);
private: // exposed registers for the CPU to access
	uint8_t ppu_ctrl = 0; // register at $200
	uint8_t ppu_mask = 0; // $2001
	uint8_t ppu_status = 0; // $2002
	uint8_t oam_addr = 0; // $2003
	uint8_t oam_data = 0; // $2004
	uint8_t ppu_scroll = 0; // 2005
	uint8_t ppu_addr = 0; // 2006
	uint8_t ppu_data = 0; // 2007
	uint8_t oam_dma = 0; // $4014

	uint8_t latch_value = 0;

public: // methods by which the CPU can access the PPU
	void ppu_ctrl_write(uint8_t data);
	void ppu_mask_write(uint8_t data);
	uint8_t ppu_status_read();
	void oam_addr_write(uint8_t data);
	void oam_data_write(uint8_t data);
	uint8_t oam_data_read();
	void ppu_scroll_write(uint8_t data);
	void ppu_addr_write(uint8_t data);
	void ppu_data_write(uint8_t data);
	uint8_t ppu_data_read();

private: // internal convenience variables
	Cartridge* cartridge;
	uint16_t addr = 0;

private: // counting where the PPU is at in its cycle
	int16_t scanline = 0; 
	int16_t cycle = 0;

private: // Internal registers
	uint16_t v = 0; 
	uint16_t t = 0;
	uint16_t x = 0; 
	uint8_t write_toggle = 0;

public: 
	// a list of every pixel to render on screen. It will update one of these on a cycle? 
	uint8_t frame_buffer[256 * 240] = { 0 };

public:
	uint8_t read(uint16_t addr14bit);
	void write(uint16_t addr14bit, uint8_t value);

	uint8_t cpuRead(uint16_t addr);
	void cpuWrite(uint16_t addr, uint8_t value);

public:
	void clock();
	void visible_scanline();
};