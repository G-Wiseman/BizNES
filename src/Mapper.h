#pragma once 
#include <cstdint>


class Mapper
{
public:
	Mapper(uint8_t num_PRG, uint8_t num_CHR);

	virtual size_t cpuReadMapped(uint16_t addr, bool& approved) = 0;
	virtual size_t cpuWriteMapped(uint16_t addr, bool& approved) = 0;

	virtual size_t ppuReadMapped(uint16_t addr, bool& approved) = 0;
	virtual size_t ppuWriteMapped(uint16_t addr, bool& approved) = 0;

	uint8_t num_PRG_banks; 
	uint8_t num_CHR_banks; 

private:

};

