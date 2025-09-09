#pragma once
#include <cstdint>
#include <string>
#include <array> 
#include <vector> 

class Bus;

class Cartridge
{
private:
	static constexpr size_t ROM_SIZE = 32 * 1024; // 32 KiB
	Bus* bus;
public:
	Cartridge();
	~Cartridge();
	void load_from_ines(const std::string filepath);
	uint8_t cpuRead(uint16_t addr);
	void cpuWrite(uint16_t addr, uint8_t value);
	uint8_t ppuRead(uint16_t addr);
	void ppuWrite(uint16_t addr, uint8_t value);

	struct InesHeader
	{
		char name[4];
		uint8_t prg;
		uint8_t chr;
		uint8_t flag6;
		uint8_t flag7;
		uint8_t flag8;
		uint8_t flag9;
		uint8_t flag10;
		uint8_t padding[5];

	} header = { 0 };
	std::vector<uint8_t> PRG_Vector = { 0 };
	std::vector<uint8_t> CHR_Vector = { 0 };
};