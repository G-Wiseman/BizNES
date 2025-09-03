#pragma once 

#include <cstdint>


class Mapper
{
public:
	Mapper();
	~Mapper();

	virtual uint8_t cpuRead(uint16_t addr) = 0;
	virtual void cpuWrite(uint16_t addr, uint8_t data) = 0;

	virtual uint8_t ppuRead(uint16_t addr) = 0;
	virtual void ppuWrite(uint16_t addr, uint8_t data) = 0;

private:

};

Mapper::Mapper()
{
}

Mapper::~Mapper()
{
}